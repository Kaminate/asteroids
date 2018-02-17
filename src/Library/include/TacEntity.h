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

#include "TacComponent.h"

#include <map>

namespace Tac
{
  class Entity
  {
  public:
    typedef int ID;
    TacDLLAPI const static ID InvalidId = -1;
    TacDLLAPI Entity(ID id);
    TacDLLAPI ~Entity();
    TacDLLAPI ID GetID() const;
    TacDLLAPI virtual bool Initialize();

    TacDLLAPI void AddComponent(Comp::Type compType, SPComp comp);
    TacDLLAPI void RemComponent(Comp::Type compType);
    TacDLLAPI bool HasComponent(Comp::Type compType);
    TacDLLAPI Comp * GetComponent(Comp::Type compType);
    TacDLLAPI unsigned GetNumComps() const;

    TacDLLAPI void AddToTweakBar(TweakBar & bar);

  private:
    SPComp mComponents[Comp::Type::eCompCount];
    ID mId;
  };
  DefineSharedPtr(Entity);

} // Tac
