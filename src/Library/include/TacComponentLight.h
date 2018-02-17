/*!
\author Nathan Park
*/
#ifdef TacDLL_Export
#define TacDLLAPI __declspec(dllexport)
#else
#define TacDLLAPI __declspec(dllimport)
#endif

#pragma once

#include "TacComponent.h"
#include "TacVector3.h"
#include "TacVector4.h"
#include "TacGLIncludes.h"
#include "TacCamera.h"

namespace Tac
{
  class LightComponent : public Comp
  {
  public:
    LightComponent();
    ~LightComponent();
    virtual void Load(Loader & serializer);
    virtual void Save(Saver & serializer);
    virtual bool Initialize();
    virtual void AddToTweakBar(TweakBar & bar);
    virtual void RemoveFromTweakBar(TweakBar & bar);

    Vector3 mDirection;
    Vector4 mColor;

    GLuint mFramebuffer;
    GLuint mDepthTexture;
    Camera mCamera;

  private:
  };
  DefineSharedPtr(LightComponent);

} // Tac
