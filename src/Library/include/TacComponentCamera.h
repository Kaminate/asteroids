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
#include "TacCamera.h"

namespace Tac
{
  class CameraComponent : public Comp
  {
  public:
    virtual ~CameraComponent();
    virtual void Load(Loader & serializer);
    virtual void Save(Saver & serializer);
    virtual bool Initialize();
    virtual void AddToTweakBar(TweakBar & bar);
    virtual void RemoveFromTweakBar(TweakBar & bar);

    Camera mCamera;
  };
  DefineSharedPtr(CameraComponent);

} // Tac
