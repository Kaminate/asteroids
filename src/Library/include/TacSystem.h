/*!
\author Nathan Park
\brief
*/
#ifdef TacDLL_Export
#define TacDLLAPI __declspec(dllexport)
#else
#define TacDLLAPI __declspec(dllimport)
#endif

#ifndef __TAC_SYSTEM_H_
#define __TAC_SYSTEM_H_

#include <map>
#include <set>

#include "TacSharedPtr.h"
#include "TacComponent.h"

namespace Tac
{
  class Core;
  class System
  {
  public:

    enum Type
    {
      eSystemGraphics, 
      eSystemTransform, 
      eSystemEditor,
      eSystemPhysics,
      eSystemApplication,
      eSystemCount
    };
    TacDLLAPI static std::string toStr(Type sysType);
    TacDLLAPI System();
    TacDLLAPI virtual ~System();

    TacDLLAPI virtual void Update(float dt);
    TacDLLAPI void SetCore(Core * engine);
    TacDLLAPI virtual bool Initialize() {return true;}
    TacDLLAPI virtual void UpdateFakeTime(float dt);

    TacDLLAPI void AddComp(Comp::Type compType, SPComp comp);
    TacDLLAPI void RemComp(Comp::Type compType, SPComp comp);
    TacDLLAPI SPComp GetRandomComp(Comp::Type compType);
    
    TacDLLAPI Core * GetCore() const;
  protected:
    typedef std::set<SPComp, LessSharedPtr<Comp> > ComponentSet;

    const ComponentSet & GetComponentSet(Comp::Type compType) const;
    ComponentSet & GetComponentSet(Comp::Type compType);
    
  private:
    Core * mCore;
    ComponentSet mComponentMap[Comp::Type::eCompCount];
  };
  DefineSharedPtr(System);

} // Tac

#endif
