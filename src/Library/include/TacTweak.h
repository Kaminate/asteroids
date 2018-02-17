/*!
\author Nathan Park
*/
#ifdef TacDLL_Export
#define TacDLLAPI __declspec(dllexport)
#else
#define TacDLLAPI __declspec(dllimport)
#endif


#pragma once

// http://sourceforge.net/p/glfw/discussion/247562/thread/26ea1c55/
//#define GLFW_CDECL 
#include <AntTweakBar.h>
#include "TacSharedPtr.h"
#include "TacVector2.h"


namespace Tac
{
  class Tweak;
  class TweakBar
  {
  private:
    TacDLLAPI TweakBar(); friend class Tweak;
  public:
    TacDLLAPI ~TweakBar();
    TacDLLAPI void Init(const std::string & name);
    TacDLLAPI void ClearBar();
    TacDLLAPI void SetTweak(Tweak * tweak) { mTweak = tweak;}

    enum Type{ReadWrite, ReadOnly};

    TacDLLAPI void AddVar(
      const std::string & name, 
      const std::string & params, 
      TwType dataType, 
      void * data,
      Type varType = ReadWrite);

    TacDLLAPI void AddComment(
      const std::string & name,
      const std::string & comment);
    
    TacDLLAPI void AddButton(
      const std::string & name,
      const std::string & description,
      TwButtonCallback onClick,
      void * data,
      const std::string & group="");

    TacDLLAPI void RemoveVar(const std::string & name);

    TacDLLAPI Tweak * GetTweak(){return mTweak;}
    TacDLLAPI void SetPosition( int x, int y );
    TacDLLAPI void GetPosition( int & x, int & y );
    TacDLLAPI void SetSize(int w, int h);
    TacDLLAPI void SetVisible(bool visible);
  private:
    TwBar * mBar;
    Tweak * mTweak;
    std::string mName;
  };
  DefineSharedPtr(TweakBar);

  class Tweak
  {
  public:
    TacDLLAPI bool Init(unsigned w, unsigned h);
    TacDLLAPI ~Tweak();
    TacDLLAPI void Render();
    TacDLLAPI void Uninit();
    TacDLLAPI const TwType& GetTypeVector2() const;
    TacDLLAPI const TwType& GetTypeVector3() const;
    TacDLLAPI const TwType& GetTypeVector4() const;
    TacDLLAPI const TwType& GetTypeComponent() const;
    TacDLLAPI SPTweakBar Create();

  private:
    void RegisterVec2();
    TwType mTypeVector2;
    void RegisterVec3();
    TwType mTypeVector3;
    void RegisterVec4();
    TwType mTypeVector4;
    void RegesterCompoent();
    TwType mTypeComponent;
  };

  
  void TW_CALL TweakErrorHandler(const char *errorMessage);
}
