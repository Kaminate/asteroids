// Nathan Park
#include "TacShaderShadow.h"
namespace Tac
{
  bool ShaderShadow::Init()
  {
    if (!LoadShader(GL_VERTEX_SHADER,   "resources/ShaderShadow.v.glsl"))
      return false;
    if (!LoadShader(GL_FRAGMENT_SHADER, "resources/ShaderShadow.f.glsl"))
      return false;
    if (!CompileAndLinkProgram())
      return false;

    // att
    mPos = BindAttribute("position");

    // uni
    mWorl = BindUniform("world");
    mView = BindUniform("view");
    mProj = BindUniform("proj");

    return true;
  }
}