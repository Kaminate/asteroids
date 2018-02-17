// Nathan Park
#include "TacShaderPixels.h"
namespace Tac
{
  bool ShaderPixels::Init()
  {
    if (!LoadShader(GL_VERTEX_SHADER,   "resources/ShaderPixels.v.glsl"))
      return false;
    if (!LoadShader(GL_FRAGMENT_SHADER, "resources/ShaderPixels.f.glsl"))
      return false;
    if (!CompileAndLinkProgram())
      return false;

    // att
    mPos = BindAttribute("position");
    mCol = BindAttribute("color");
    mView = BindUniform("view");
    mProj = BindUniform("proj");
    return true;
  }
}