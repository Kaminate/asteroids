/*!
\author Nathan Park
*/
#pragma once

#include "TacShader.h"

namespace Tac
{
  class ShaderPos : public Shader
  {
  public:
    bool Init();

    // attributes
    GLuint mPos;
    GLuint mNor;

    // uniforms
    GLuint mCol;
    GLuint mWorl;
    GLuint mView;
    GLuint mProj;
    GLuint mLightCol;
    GLuint mLightDir;
    GLuint mInvTransWorld;
    GLuint mShadowMap;
    GLuint mLightView;
    GLuint mLightProj;
  };

} // Tac
