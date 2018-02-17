/*!
\author Nathan Park

\par Project: Tac Engine

\brief

*/
#ifdef TacDLL_Export
#define TacDLLAPI __declspec(dllexport)
#else
#define TacDLLAPI __declspec(dllimport)
#endif

#pragma once

#include "TacSystem.h"
#include "TacRunningAverage.h"
#include "TacTimer.h"
#include "TacGLFW.h"
#include "TacCallback.h"
#include "TacEntity.h"
#include "TacComponentCreator.h"
#include "TacTweak.h" // AntTweakBar
#include "TacNetwork.h" // Winsock

#include <map>
#include <exception>
//#include <vld.h> // For the dll

namespace Tac
{
  class CoreException : public std::exception
  {
    std::string mStr;
  public:
    TacDLLAPI CoreException(const std::string & str) 
      : mStr(str)
    {
      __debugbreak();
    }
    TacDLLAPI virtual const char * what() const override
    {
      return mStr.c_str();
    }
  };

  class Core
  {
  public:
    typedef std::map<Entity::ID, SPEntity> EntityContainer;

    TacDLLAPI Core(
      const std::string & gameName = "Tac", 
      unsigned w = 800, 
      unsigned h = 600);
    TacDLLAPI ~Core();
    TacDLLAPI void AddSystem(System::Type sysType, SPSystem sys);
    TacDLLAPI System * GetSystem(System::Type sysType);
    TacDLLAPI bool InitSystems();

    TacDLLAPI void Run();
    TacDLLAPI float GetElapsedTime() const;
    TacDLLAPI void SetRenderCallback(SPCallback callback);

    TacDLLAPI Window & GetWindow();
    TacDLLAPI Tweak & GetTweak();
    TacDLLAPI SPTweakBar CreateTweakBar();


    TacDLLAPI SPEntity CreateEntity();
    TacDLLAPI void DeleteEntity(SPEntity gameObject);
    TacDLLAPI void ClearEntities();
    TacDLLAPI SPEntity GetEntity(Entity::ID entityId);
    TacDLLAPI SPComp CreateComponent(Comp::Type compType);
    TacDLLAPI const EntityContainer & GetEntities();

    TacDLLAPI bool Load(const std::string & filename);
    TacDLLAPI bool Save(const std::string & filename) const;
    TacDLLAPI void StopRunning();

  private:
    Entity::ID sID;
    std::string mGameName;

    static const float sTimestep;

    SPSystem mSystems[System::Type::eSystemCount];
    bool mRunning;
    bool mPaused;
    float mElapsedTime;
    Timer mTimer;

    SPCallback mRenderCallback;

    GLFW mGlfw;
    Window mWindow;
    Tweak mTweak;
    Winsock mWinsock; // maybe todo: move this into pimpl

    SPTweakBar mEngineBar;
    //RunningAverage mfps;
    float mLastFPS;
    float mMiliseconds[System::Type::eSystemCount];
    float mFakeUpdateMS;
    float mRenderMS;
    float mTweakRenderMS;
    float mSwapBufferMS;

    EntityContainer mEntities;
    ComponentCreator mCreator;
    CreatorMap mCreatorMap;
  };

  DefineSharedPtr(Core);
} // Tac
