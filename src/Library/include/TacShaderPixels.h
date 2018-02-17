/*!
\author Nathan Park
*/
#pragma once

#include "TacShader.h"

namespace Tac
{
  // most useful shader na
  class ShaderPixels: public Shader
  {
  public:
    bool Init();

    GLuint mPos; // screenspace. rly. <-- ADDENUDM: NOW WORLDSPACE
    GLuint mCol;
    GLuint mView;
    GLuint mProj;
  };

} // Tac
