// Nathan Park
#include "TacSystemEditor.h"
#include "TacSystemGraphics.h"
#include "TacCore.h"
namespace Tac
{
  EditorSystem::EditorSystem()
    : mIDToSelect(Entity::InvalidId)
  {

  }

  void EditorSystem::Update( float dt )
  {

  }

  void EditorSystem::UpdateFakeTime( float dt )
  {

  }

  void TW_CALL OpenLevel(void * data)
  {
    EditorSystem * editor = (EditorSystem*) data;
    std::string filename;
    Core * engine = editor->GetCore();

    std::vector<Window::FileTypeThing> things;
    things.push_back(Window::FileTypeThing(
      "level (.taclevel)", "taclevel"));
    if(engine->GetWindow().OpenFile(filename, things))
    {
      // order of these calls is semi important
      editor->CLearList();
      engine->ClearEntities();
      engine->Load(filename);
      editor->SetCurrentLevelPath(filename);
      for (auto pair: engine->GetEntities())
      {
        SPEntity entity = pair.second;
        editor->AddEntityToList(entity);
      }

      // will set camera to null if no cam
      GraphicsSystem * graphics
        =  (GraphicsSystem*) engine->GetSystem(System::eSystemGraphics);

      graphics->SetCurrentCamera(
        (CameraComponent*)graphics->GetRandomComp(
        Comp::Type::eCompCamera).get());

      graphics->SetCurrentLight(
        (LightComponent*)graphics->GetRandomComp(
        Comp::Type::eCompLight).get());

    }
  }
  void TW_CALL SaveLevel(void * data);
  void TW_CALL SaveLevelAs(void * data);
  void TW_CALL SaveLevelAs(void * data)
  {
    EditorSystem * editor = (EditorSystem*) data;
    std::string filename;
    Core * engine = editor->GetCore();
    std::vector<Window::FileTypeThing> things;
    things.push_back(Window::FileTypeThing(
      "level (.taclevel)", "taclevel"));
    things.push_back(Window::FileTypeThing(
      "text (.txt)", "txt"));
    if(engine->GetWindow().SaveFile(filename, things))
    {
      editor->SetCurrentLevelPath(filename);
      SaveLevel(data);
    }
  }
  void TW_CALL SaveLevel(void * data)
  {
    EditorSystem * editor = (EditorSystem*) data;
    const std::string & path = editor->GetCurrentLevelPath();
    if (path.empty())
    {
      SaveLevelAs(data);
    }
    else
    {
      Core * engine = editor->GetCore();
      engine->Save(path);
    }
  }
  void TW_CALL CreateEntity(void * data)
  {
    EditorSystem * editor = (EditorSystem *) data;
    SPEntity gameObj = editor->GetCore()->CreateEntity();
    editor->AddEntityToList(gameObj);
    editor->SelectEntity(gameObj);
  }
  void TW_CALL OnSelectEntity(void * data)
  {
    EditorSystem * editor = (EditorSystem *) data;
    SPEntity gameObj 
      = editor->GetCore()->GetEntity(editor->GetIdToSelect());
    if (gameObj)
      editor->SelectEntity(gameObj);
  }

  bool EditorSystem::Initialize()
  {
    mTweakMenu = GetCore()->CreateTweakBar();
    mTweakMenu->Init("Menu");
    mTweakMenu->SetPosition(50,50);
    mTweakMenu->SetSize(270,200);
    mTweakMenu->AddButton(
      "OpenLevel", "Open Level", OpenLevel, this);
    mTweakMenu->AddButton(
      "SaveLevel", "Save Level", SaveLevel, this);
    mTweakMenu->AddButton(
      "SaveLevelAS", "Save Level As", SaveLevelAs, this);
    mTweakMenu->AddButton(
      "CreateEntity", "Create Entity", CreateEntity, this);
    mTweakMenu->AddVar(
      "Entity to select", "label='Entity to select'", 
      TW_TYPE_INT32, &mIDToSelect);
    mTweakMenu->AddButton(
      "Select Entity", "Select Entity", OnSelectEntity, this);
    mTweakMenu->AddComment("Entities", "Entities");

    return true;
  }

  void EditorSystem::SetCurrentLevelPath( const std::string & levelPath )
  {
    mLevel = levelPath;
  }

  const std::string & EditorSystem::GetCurrentLevelPath()
  {
    return mLevel;
  }

  void EditorSystem::SelectEntity( SPEntity gameObj )
  {
    Entity::ID eid = gameObj->GetID();
    if (mSelectedEntities.find(eid) == mSelectedEntities.end())
    {
      auto selected = SPSelectedEntity(new SelectedEntity(this, gameObj));

      mSelectedEntities[eid] = selected;
    }
  }

  void EditorSystem::AddEntityToList( SPEntity gameObj )
  {
    std::string name = std::to_string(gameObj->GetID());
    mTweakMenu->AddComment(name, name);
  }

  void EditorSystem::RemEntityFromList( SPEntity gameObj )
  {
    std::string name = std::to_string(gameObj->GetID());
    mTweakMenu->RemoveVar(name);
  }

  Entity::ID EditorSystem::GetIdToSelect()
  {
    return mIDToSelect;
  }

  void EditorSystem::CLearList()
  {
    mSelectedEntities.clear();
    for (auto pair : GetCore()->GetEntities())
    {
      SPEntity gameObj = pair.second;
      RemEntityFromList(gameObj);
    }
  }

} // Tac