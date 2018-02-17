/*!
\author Nathan Park
*/
#include "TacComponentLight.h"

namespace Tac
{
  // Todo: make it so that this needs to be passed an opengl context
  // in the constructor which initializs the framebuffer and depth texture
  // > not just raii, = good library design
  // how to do that in this engine takes a lot of thought...
  // I think ill just acknowledge that fact that it can be improved
  // because im writing a demo, not an engine
  LightComponent::LightComponent()
    : mDirection(0,0,-1)
    , mColor(1,1,1,1)
    , mFramebuffer(0)
    , mDepthTexture(0)
  {

  }

  LightComponent::~LightComponent()
  {
    if (mDepthTexture != 0)
    {
      glDeleteTextures(1, &mDepthTexture);
    }
    if (mFramebuffer != 0)
    {
      glDeleteFramebuffers(1, &mFramebuffer);
    }
  }

  void LightComponent::Load( Loader & serializer )
  {
    serializer.Load(mDirection);
    serializer.Load(mColor);
  }

  void LightComponent::Save( Saver & serializer )
  {
    serializer.SaveFormatted("direction", mDirection);
    serializer.SaveFormatted("color", mColor);
  }

  bool LightComponent::Initialize()
  {

    glGenFramebuffers(1, &mFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);

    glGenTextures(1, &mDepthTexture);
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
    glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      GL_DEPTH_COMPONENT16, 
      1024, 
      1024,
      0,
      GL_DEPTH_COMPONENT,
      GL_FLOAT,
      0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(
      GL_FRAMEBUFFER, 
      GL_DEPTH_ATTACHMENT, 
      mDepthTexture, 
      0); // mipmap level
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
    {
      return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mCamera.mAspectRatio = 1;
    mCamera.mFar = 1000.0f;
    mCamera.mFieldOfViewYRad;
    mCamera.mNear = 0.01f;
    mCamera.mPosition;
    mCamera.mViewDirection;
    mCamera.mWorldSpaceUp = Vector3(0,0,1);
    return true;
  }

  void LightComponent::AddToTweakBar( TweakBar & bar )
  {
    bar.AddVar("Light Direction", 
      "group=Light "
      "label='wsDirection '",
      TW_TYPE_DIR3F, &mDirection);
    bar.AddVar("Light Color", 
      "group=Light "
      "label='Color '",
      TW_TYPE_COLOR4F, &mColor);
  }

  void LightComponent::RemoveFromTweakBar( TweakBar & bar )
  {
    bar.RemoveVar("Light Direction");
    bar.RemoveVar("Light Color");
  }

} // Tac
