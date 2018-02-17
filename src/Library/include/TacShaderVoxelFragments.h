/*!
\author Nathan Park
*/
#pragma once

#include "TacShader.h"

namespace Tac
{
  class ShaderVoxelFragments: public Shader
  {
  public:
    bool Init();

    // attributes
    GLuint mPos;
    //GLuint mNor; // todo

    // uniforms
    GLuint mCol;
    GLuint mWorl; // I assume all we want is world matrix?
    GLuint mView; // temp temp to draw scene from the buffer
    GLuint mProj; // temp temp to draw scene from the buffer

    //GLuint mInvTransWorld; // todo
    
    // NOTE:
    // this shader uses a shader storage buffer
    // for the fragments and another for the fragCount
    GLuint mMaxFrags;
    static const unsigned mFragBufferBinding = 0;
    static const unsigned mFragCountBinding = 1;
  };

} // Tac
