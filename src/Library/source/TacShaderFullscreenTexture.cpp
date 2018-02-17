// Nathan Park
#include "TacShaderFullscreenTexture.h"
namespace Tac
{
  bool ShaderFullscreenTexture::Init()
  {
    if (!LoadShader(GL_VERTEX_SHADER, 
      "resources/ShaderFullscreenTexture.v.glsl"))
      return false;
    if (!LoadShader(GL_FRAGMENT_SHADER, 
      "resources/ShaderFullscreenTexture.f.glsl"))
      return false;
    if (!CompileAndLinkProgram())
      return false;

    // att
    mPos = BindAttribute("position");
    mTexCoords = BindAttribute("texcoord");

    // uni
    mTexture = BindUniform("textureMap");
    return true;
  }
}