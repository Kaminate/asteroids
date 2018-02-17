/*!
\author Nathan Park
\brief
*/
#pragma once

#include "TacSystem.h"
#include "TacTweak.h"
#include "TacEntity.h"
#include "TacSelectedEntity.h"

namespace Tac
{
  class EditorSystem: public System
  {
  public:
    EditorSystem();
    virtual void Update(float dt);
    virtual void UpdateFakeTime(float dt);
    virtual bool Initialize();
    void SetCurrentLevelPath(const std::string & levelPath);
    const std::string & GetCurrentLevelPath();

    void CLearList();
    void AddEntityToList(SPEntity gameObj);
    void RemEntityFromList(SPEntity gameObj);
    void SelectEntity(SPEntity gameObj);
    //void DeselectEntity();

    Entity::ID GetIdToSelect();

  private:
    SPTweakBar mTweakMenu;
    std::map<Entity::ID, SPSelectedEntity> mSelectedEntities;
    std::string mLevel;
    Entity::ID mIDToSelect;
  };

} // Tac
