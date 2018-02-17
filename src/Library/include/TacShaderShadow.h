/*!
\author Nathan Park
*/
#pragma once

#include "TacShader.h"

namespace Tac
{
  class ShaderShadow : public Shader
  {
  public:
    bool Init();

    // attributes
    GLuint mPos;

    // uniforms
    GLuint mWorl;
    GLuint mView;
    GLuint mProj;
  };

} // Tac
