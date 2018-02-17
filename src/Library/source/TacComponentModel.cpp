// Nathan Park
#include "TacComponentModel.h"
#include "TacSystemGraphics.h"
#include "TacGeometryData.h"
#include "TacCore.h"
#include <list>
#include <TacVeryUtil.h>
namespace Tac
{
  void ModelComponent::Load( Loader & serializer )
  {
    serializer.Load(mMeshFilepath);
    serializer.Load(mColor);
  }

  void ModelComponent::Save( Saver & serializer )
  {
    serializer.SaveFormatted("meshFilepath", mMeshFilepath);
    serializer.SaveFormatted("color", mColor);
  }

  bool ModelComponent::Initialize()
  {
    GraphicsSystem * gSys = (GraphicsSystem*) GetSystem();

    GeometryData data;
    if (mMeshFilepath.empty())
      return true;

    bool success = data.Load(mMeshFilepath);

    if (success)
    {
      mGeom = SPGeometry(new Geometry(data));
    }

    return success;
  }

  Geometry * ModelComponent::GetGeometry()
  {
    return mGeom.get();
  }

  void TW_CALL LoadMesh(void * data)
  {
    ModelComponent* model= (ModelComponent*) data;
    Core * engine = model->GetSystem()->GetCore();

    std::vector<Window::FileTypeThing> things;
    things.push_back(Window::FileTypeThing(
      "level (.tacmodel)", "tacmesh"));

    std::string selectedMeshFilepath;
    if(engine->GetWindow().OpenFile(selectedMeshFilepath, things))
    {
      // make it a relative path
      std::string relativePath;
      TacGetPathToFileFromWorkingDir(selectedMeshFilepath, relativePath);

      model->SetMeshFilepath(relativePath);
      
      if (!model->Initialize())
      {
        model->SetMeshFilepath("Load Failed");
      }
    }

    // old VVV
    //ModelComponent * model = (ModelComponent*) data;
    //model->Initialize();
  }

  void ModelComponent::AddToTweakBar( TweakBar & bar )
  {
    Tweak & tactweak = GetSystem()->GetCore()->GetTweak();
    TwType twvec3 = tactweak.GetTypeVector3();
    TwType twvec4 = tactweak.GetTypeVector4();
    bar.AddVar("Model Filepath", 
      "group=Model "
      "label='Mesh Filepath' ",
      TW_TYPE_STDSTRING, &mMeshFilepath,
      Tac::TweakBar::ReadOnly);
    // button to callback load the mesh
    bar.AddButton("Model Load", "Load Mesh", LoadMesh, this, "Model");
    bar.AddVar("Model Color", 
      "group=Model "
      "label='Color' ",
      TW_TYPE_COLOR4F,
      &mColor);
  }

  void ModelComponent::RemoveFromTweakBar( TweakBar & bar )
  {
    bar.RemoveVar("Model Filepath");
    bar.RemoveVar("Model Load");
    bar.RemoveVar("Model Color");
  }

  ModelComponent::ModelComponent() :mColor(1,1,1,1)
  {

  }

  void ModelComponent::SetMeshFilepath( const std::string & filepath )
  {
    mMeshFilepath = filepath;
  }

} // Tac