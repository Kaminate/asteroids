// Nathan Park
#include "TacSystemApplication.h"
namespace Tac
{
  AppSystem::AppSystem()
  {

  }

  AppSystem::~AppSystem()
  {

  }

  void AppSystem::Update( float dt )
  {
    if (mMiniApp)
      mMiniApp->Update(dt);
  }

  bool AppSystem::Initialize()
  {
    return true;
  }

  void AppSystem::UpdateFakeTime( float dt )
  {

  }

  void AppSystem::HandleClientMessages( float dt )
  {

  }

  void AppSystem::SetMiniApp( SPMiniApp app )
  {
    mMiniApp = app;
  }

  MiniApp::MiniApp(AppSystem * owner) : mOwner(owner)
  {

  }

  AppSystem * MiniApp::GetOwner()
  {
    return mOwner;
  }

}