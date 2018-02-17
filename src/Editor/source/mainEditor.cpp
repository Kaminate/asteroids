#include "TacCore.h"
#include "TacSystemGraphics.h"
#include "TacSystemTransform.h"
#include "TacSystemPhysics.h"
#include "TacSystemEditor.h"
//#include <vld.h>
#include "TacComponentTransform.h"
#include <TacComponentModel.h>

// remove console window
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "TacGuicon.h"

// Editor Main
int main(int argc, char ** argv)
{
  using namespace Tac;
  using namespace std;

  SPCore engine(new Core("Editor"));
  engine->AddSystem(System::eSystemTransform, SPSystem(new TransformSystem()));
  engine->AddSystem(System::eSystemPhysics, SPSystem(new PhysicsSystem()));
  engine->AddSystem(System::eSystemGraphics, SPSystem(new GraphicsSystem()));
  engine->AddSystem(System::eSystemEditor, SPSystem(new EditorSystem()));

  if(engine->InitSystems())
  {
    /*
    engine->Load("resources/test2.taclevel");
    GraphicsSystem * g = (GraphicsSystem*)engine->GetSystem(System::eSystemGraphics);
    g->SetCurrentCamera((CameraComponent*)g->GetRandomComp(Comp::eCompCamera).get());
    g->SetCurrentLight((LightComponent*)g->GetRandomComp(Comp::eCompLight).get());
    for (unsigned i = 0; i < 10; ++i)
    {
      for (unsigned j = 0; j < 10; ++j)
      {
        SPEntity e = engine->CreateEntity();
        e->AddComponent(Comp::eCompPhysics, engine->CreateComponent(Comp::eCompPhysics));
        e->AddComponent(Comp::eCompTransform, engine->CreateComponent(Comp::eCompTransform));
        e->AddComponent(Comp::eCompModel, engine->CreateComponent(Comp::eCompModel));
        TransformComp * trans = (TransformComp*) e->GetComponent(Comp::eCompTransform);
        trans->mPos.z = (float)(rand() % 10 + 10);
        trans->mPos.x = (float)i;
        trans->mPos.y = (float)j;
        ModelComponent * modl = (ModelComponent*) e->GetComponent(Comp::eCompModel);
        modl->SetMeshFilepath("resources/cube.tacmesh");
        e->Initialize();
      }
    }

    engine->Save("resources/testsf.taclevel");
    */


    engine->Run();
  }

  return 0;
}