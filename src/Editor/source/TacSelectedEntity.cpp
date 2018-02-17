// Nathan Park
#include "TacSelectedEntity.h"
#include "TacSystemEditor.h"
#include "TacCore.h"
namespace Tac
{
  void TW_CALL AddComponent(void * data)
  {
    SelectedEntity * selected = (SelectedEntity*) data;
    SPEntity entity = selected->GetSelected();

    Comp::Type compType =selected->GetCurrentType() ;
    if (entity->HasComponent(compType))
      return;

    SPComp comp 
      = selected->GetEditor()->GetCore()->CreateComponent(compType);

    entity->AddComponent(compType, comp);
    if (comp->Initialize())
    {
      comp->AddToTweakBar(*selected->GetTweak());
    }
    else
    {
      entity->RemComponent(compType);
    }
  }
  void TW_CALL RemoveComponent(void * data)
  {
    SelectedEntity * selected = (SelectedEntity*) data;
    SPEntity entity = selected->GetSelected();
    Comp::Type compType =selected->GetCurrentType() ;
    if (entity->HasComponent(compType))
    {
      Comp * comp = entity->GetComponent(compType);
      comp->RemoveFromTweakBar(*selected->GetTweak());
      entity->RemComponent(compType);
      // after removal (aka NOW), comp is an invalid pointer
    }
  }
  SelectedEntity::SelectedEntity( EditorSystem * editor, SPEntity gameObj)
    : mEditor(editor)
    , mComponentType(Comp::Type::eCompTransform)
    , mSelectedEntity(gameObj)
  {
    // create a new tweak bar
    mTweakSelected.reset();
    mTweakSelected = editor->GetCore()->CreateTweakBar();
    mTweakSelected->Init("Entity_" + std::to_string(gameObj->GetID()));
    mTweakSelected->SetSize(270, 200);
    mTweakSelected->SetPosition(50,350);
    mTweakSelected->AddButton(
      "Add Component", 
      "Add Component", 
      AddComponent, this);
    mTweakSelected->AddButton(
      "Remove Component",
      "Remove Component",
      RemoveComponent, this);
    mTweakSelected->AddVar(
      "Component Type",
      "",
      editor->GetCore()->GetTweak().GetTypeComponent(),
      &mComponentType);

    // add entity too bar
    mSelectedEntity->AddToTweakBar(*mTweakSelected);
  }

  Tac::SPEntity SelectedEntity::GetSelected()
  {
    return mSelectedEntity;
  }

  Comp::Type SelectedEntity::GetCurrentType()
  {
    return mComponentType;
  }

  EditorSystem * SelectedEntity::GetEditor()
  {
    return mEditor;
  }

  Tac::SPTweakBar SelectedEntity::GetTweak()
  {
    return mTweakSelected;

  }

}