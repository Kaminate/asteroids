/*!
\author Nathan Park
\brief
*/
#pragma once

#include "TacTweak.h"
#include "TacEntity.h"
#include "TacSharedPtr.h"

namespace Tac
{
  class EditorSystem;
  class SelectedEntity
  {
  public:
    SelectedEntity(EditorSystem * editor, SPEntity gameObject);

    SPTweakBar GetTweak();
    EditorSystem * GetEditor();
    SPEntity GetSelected();
    Comp::Type GetCurrentType();
    
  private:
    SPTweakBar mTweakSelected;
    EditorSystem * mEditor; 
    SPEntity mSelectedEntity;
    Comp::Type mComponentType;
  };
  DefineSharedPtr(SelectedEntity);

} // Tac
