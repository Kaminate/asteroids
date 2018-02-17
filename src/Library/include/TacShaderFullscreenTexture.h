/*!
\author Nathan Park
*/
#pragma once

#include "TacShader.h"

namespace Tac
{
  class ShaderFullscreenTexture : public Shader
  {
  public:
    bool Init();

    // attributes
    GLuint mPos;
    GLuint mTexCoords;

    // uniforms
    GLuint mTexture;
  };

} // Tac
