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
#include "TacSharedPtr.h"
#include "TacSerializer.h"
#include "TacTweak.h"

namespace Tac
{
  class ComponentManager;
  class System;
  class Entity;
  
  class Comp
  {
  public:
    enum Type
    {
      // Don't reorder the types because it will fuck the level files
      // just push in the end before eCompCount
      eCompTransform,
      eCompModel,
      eCompCamera,
      eCompPhysics,
      eCompLight,

      eCompCount
    };
    static const char * GetTypeString(Type compType);

    Comp();
    ~Comp();
    virtual bool Initialize() = 0;
    virtual void Load(Loader & serializer) = 0;
    virtual void Save(Saver & serializer) = 0;
    virtual void AddToTweakBar(TweakBar & bar) = 0;
    virtual void RemoveFromTweakBar(TweakBar & bar) = 0;

    // Can not be SPSystem because then circular dependency
    // would like to replace with System::Type, but now I need globals...
    void SetSystem(System * sys); 
    System * GetSystem();

    void SetEntity(Entity * gameObjet);
    Comp * GetSibling(Type compType);
    
    
  private:
    System * mSystem;
    Entity * mEntity;
  };
  DefineSharedPtr(Comp);

  // for Component::Type
  std::ifstream & operator >> (
    std::ifstream & ifs, 
    Comp::Type & compType);
  std::ofstream & operator << (
    std::ofstream & ofs, 
    Comp::Type compType);

} // Tac
