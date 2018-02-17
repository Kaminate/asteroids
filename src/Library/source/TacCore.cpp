/*!
\file Core.cpp
\brief
\author Nathan Park
\par
*/

#include "TacCore.h"
#include "TacSystem.h"
#include "TacVeryUtil.h"
#include <TacSerializer.h>
#include <TacErrorIf.h>

#include <thread>

namespace Tac
{

  const float Core::sTimestep = 1/60.0f;

  bool Core::InitSystems()
  {
    for (SPSystem sys : mSystems)
    {
      if (sys && !sys->Initialize())
        return false;
    }
    return true;
  }

  Core::Core(const std::string & gameName, unsigned w, unsigned h) 
    : mGameName(gameName)
    , mRunning(false) 
    , mPaused(false)
    , mElapsedTime(0) 
    , sID(Entity::InvalidId)
    , mLastFPS(0)
  {
    int size = sizeof(mMiliseconds);
    memset(mMiliseconds, 0, sizeof(size));

    if (mGameName.empty())
      mGameName = "Tac Engine";

    RemoveSpaces(mGameName);


    if (!mGlfw.Init())
      throw CoreException("glfw failed");

    if (!mWinsock.Ok())
      throw CoreException("winsock failed");

    if (w != 0 && h != 0)
    {
      mWindow.Open(w, h, mGameName.c_str());

      if (!mTweak.Init(w, h))
        throw CoreException("tweakbar init failed");
    }


    mEngineBar = mTweak.Create();
    mEngineBar->Init("core"+gameName);
    mEngineBar->AddVar("",
      "label='fps'", TW_TYPE_FLOAT, 
      &mLastFPS);
    mEngineBar->SetVisible(false);

    for (unsigned i = 0; i < System::eSystemCount; ++i)
    {
      System::Type curtype = (System::Type) i;
      mEngineBar->AddVar("",
        "label = '" + System::toStr(curtype) + "_ms'",
        TW_TYPE_FLOAT,
        &mMiliseconds[i]
      );
    }
    mEngineBar->AddVar("", "label='FakeUpdate_ms'",
      TW_TYPE_FLOAT, &mFakeUpdateMS);

    mEngineBar->AddVar("", "label='Render_ms'",
      TW_TYPE_FLOAT, &mRenderMS);

    mEngineBar->AddVar("", "label='anttweakbar_ms'",
      TW_TYPE_FLOAT, &mTweakRenderMS);

    mEngineBar->AddVar("", "label='swapbuff_ms'",
      TW_TYPE_FLOAT, &mSwapBufferMS);

    glfwSwapInterval(0); // <-- do this

  }

  Core::~Core()
  {
    // remove entities from systems
    mEntities.clear();

    // remove systems in reverse order
    unsigned count = System::Type::eSystemCount;
    for (unsigned i = 0 ; i < count; ++i)
    {
      mSystems[count - i - 1].reset();
    }
  }

  void Core::Run()
  {
    mTimer.Start();
    mRunning = true;

    while (mRunning)
    {
      if (mWindow.IsOpen() && mWindow.ShouldClose())
        mRunning = false;

      mGlfw.PollEvents();
      mTimer.Stop();

      // time since last frame
      float timeSinceLastFrame = mTimer.GetElapsedTime();
      if (timeSinceLastFrame >= sTimestep)
      {
        static int frames;
        ++frames;
        static float sec = 0;
        sec += timeSinceLastFrame;
        if (sec >= 1)
        {
          mLastFPS = (float)frames;
          //mfps.Push(frames);
          frames = 0;
          sec -= 1;
        }

        //mfps.Push(timeSinceLastFrame * 1000);
        mElapsedTime += sTimestep;

        mTimer.Reset();
        mTimer.Start();

        // update each system(timestep)
        for (unsigned i = 0; i < System::eSystemCount; ++i)
        {
          Timer sysTime;
          sysTime.Start();
          SPSystem currSystem = mSystems[i];
          if (currSystem)
          {
            currSystem->Update(sTimestep);
          }

          sysTime.Stop();
          mMiliseconds[(System::Type)i] = 1000 * sysTime.GetElapsedTime();
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
      } // if (timestep has passed)
      else
      {
        float fakeTime = mTimer.GetElapsedTime();
        mTimer.Start();

        Timer fakeTimer;
        fakeTimer.Start();
        for (unsigned i = 0; i < System::eSystemCount; ++i)
        {
          SPSystem currSystem = mSystems[i];
          if (currSystem)
          {
            currSystem->UpdateFakeTime(fakeTime);
          }
        }
        fakeTimer.Stop();
        mFakeUpdateMS = 1000 * fakeTimer.GetElapsedTime();

        if(mWindow.IsOpen() && mRenderCallback)
        {
          Timer renderTimer;
          renderTimer.Start();
          mRenderCallback->Execute();
          renderTimer.Stop();
          mRenderMS = 1000 * renderTimer.GetElapsedTime();

          Timer tweakRender;
          tweakRender.Start();
          mTweak.Render();
          tweakRender.Stop();
          mTweakRenderMS = 1000 * tweakRender.GetElapsedTime();

          Timer swapTimer;
          swapTimer.Start();
          mWindow.SwapBuffers();
          swapTimer.Stop();
          mSwapBufferMS = 1000 * swapTimer.GetElapsedTime();
        }
      } // else timestep has not passed
    } // while (running)
  } // Core::Run()

  void Core::SetRenderCallback(SPCallback render)
  {
    mRenderCallback = render;
  }

  Window & Core::GetWindow()
  {
    return mWindow;
  }

  float Core::GetElapsedTime() const
  {
    return mElapsedTime;
  }

  SPEntity Core::CreateEntity()
  {
    SPEntity newEntity (new Entity(++sID));
    mEntities[sID] = newEntity;
    return newEntity;
  }

  TacDLLAPI void Core::DeleteEntity( SPEntity gameObject )
  {
    mEntities.erase(gameObject->GetID());
  }

  SPComp Core::CreateComponent(Comp::Type compType)
  {
    SPComp comp = mCreator.Create(compType);
    System::Type creatorType = mCreatorMap.GetCreator(compType);
    SPSystem sys = mSystems[creatorType];
    ErrorIf(!sys, "System never created");
    sys->AddComp(compType, comp);
    comp->SetSystem(sys.get());
    return comp;
  }

  bool Core::Load( const std::string & filename )
  {
    Loader serializer(filename);
    bool success = serializer.Ok();
    if (success)
    {
      serializer.Load(mGameName);
      unsigned numEntities;
      serializer.Load(numEntities);
      while (numEntities--)
      {
        SPEntity iEntity = CreateEntity(); 
        unsigned numComponents;
        std::string entityName;
        serializer.Load(entityName);
        serializer.Load(numComponents);
        while(numComponents--)
        {
          Comp::Type compType;
          serializer.LoadEnum((unsigned&)compType);
          SPComp comp = CreateComponent(compType);
          comp->Load(serializer);
          iEntity->AddComponent(compType, comp);
        }
        if (!iEntity->Initialize())
          DeleteEntity(iEntity);
      }
    }
    return success;
  }

  bool Core::Save( const std::string & filename ) const 
  {
    Saver fileSaver(filename);
    bool success = fileSaver.Ok();
    if (success)
    {
      fileSaver.SaveFormatted("game", mGameName);
      fileSaver.Tab();
      fileSaver.SaveFormatted("numEntities", mEntities.size());
      unsigned entityCounter = 0;
      for (auto & entityPair : mEntities)
      {
        char buffer[6];
        _itoa_s(entityCounter++, buffer, 10);
        std::string number(buffer);

        fileSaver.SaveFormatted("entity", "entity" + number);
        fileSaver.Tab();
        SPEntity iEntity = entityPair.second;
        fileSaver.SaveFormatted("numComponents", iEntity->GetNumComps());
        for (
          unsigned iComp = 0; 
          iComp < Comp::Type::eCompCount; 
        ++iComp)
        {
          Comp::Type compType = (Comp::Type) iComp;
          Comp * comp = iEntity->GetComponent(compType);
          if (comp)
          {
            fileSaver.SaveFormatted("componentType", compType);
            fileSaver.Tab();
            comp->Save(fileSaver);
            fileSaver.DeTab();
          }
        }
        fileSaver.DeTab();
      }
    }
    return success;
  }

  TacDLLAPI void Core::AddSystem( System::Type sysType, SPSystem sys )
  {
    mSystems[sysType] = sys;
    sys->SetCore(this);
  }

  TacDLLAPI System * Core::GetSystem( System::Type sysType )
  {
    return mSystems[sysType].get();
  }

  SPTweakBar Core::CreateTweakBar()
  {
    return mTweak.Create();

  }

  TacDLLAPI void Core::ClearEntities()
  {
    sID = Entity::InvalidId;
    mEntities.clear();
  }

  TacDLLAPI Tweak & Core::GetTweak()
  {
    return mTweak;

  }

  TacDLLAPI SPEntity Core::GetEntity( Entity::ID entityId )
  {
    SPEntity gameObj;
    auto it = mEntities.find(entityId);
    if (it != mEntities.end())
    {
      gameObj = (*it).second;
    }
    return gameObj;
  }

  TacDLLAPI const Core::EntityContainer & Core::GetEntities()
  {
    return mEntities;
  }

  TacDLLAPI void Core::StopRunning()
  {
    mRunning = false;
  }

} // Tac

