/*!
\author Nathan Park
*/
#pragma once

#include "TacShader.h"

namespace Tac
{
  class ShaderAsteroids: public Shader
  {
  public:
    bool Init();

    // attributes
    GLuint mPos;
    GLuint mNor;

    // uniforms
    GLuint mCol;
    GLuint mWorl;
    //GLuint mInvTransWorld;
    GLuint mView;
    GLuint mProj;
  };

} // Tac
