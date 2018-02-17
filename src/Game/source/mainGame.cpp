#include "TacCore.h"
#include "TacSystemGraphics.h"
#include "TacSystemTransform.h"
#include "TacSystemPhysics.h"
#include <TacSystemApplication.h>
#include "TacCS260AsteroidsClient.h"
//#include <vld.h>
#include <time.h>
#include <math.h>

// remove console window
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argc, char ** argv)
{
  using namespace Tac;
  using namespace std;

  SPCore engine(new Core("Client"));
  engine->AddSystem(System::eSystemTransform, SPSystem(new TransformSystem()));
  engine->AddSystem(System::eSystemGraphics, SPSystem(new GraphicsSystem()));
  engine->AddSystem(System::eSystemApplication, SPSystem(new AppSystem()));
  engine->AddSystem(System::eSystemPhysics, SPSystem(new PhysicsSystem()));
  AppSystem * app = (AppSystem*)engine->GetSystem(System::eSystemApplication);
  app->SetMiniApp(SPMiniApp(new Asteroids::Client(app)));

  if (engine->InitSystems())
  {
    engine->Load("resources/asteroids.taclevel");

    GraphicsSystem * graphics
      =  (GraphicsSystem*) engine->GetSystem(System::eSystemGraphics);

    graphics->SetCurrentCamera(
      (CameraComponent*)graphics->GetRandomComp(
      Comp::Type::eCompCamera).get());

    graphics->SetCurrentLight(
      (LightComponent*)graphics->GetRandomComp(
      Comp::Type::eCompLight).get());

    engine->Run();
  }

  return 0;
}
