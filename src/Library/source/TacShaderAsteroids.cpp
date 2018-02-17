// Nathan Park
#include "TacShaderAsteroids.h"
namespace Tac
{
  bool ShaderAsteroids::Init()
  {
    if (!LoadShader(GL_VERTEX_SHADER,   "resources/ShaderAsteroids.v.glsl"))
      return false;
    if (!LoadShader(GL_FRAGMENT_SHADER, "resources/ShaderAsteroids.f.glsl"))
      return false;
    if (!CompileAndLinkProgram())
      return false;

    // att
    mPos = BindAttribute("position");
    mNor = BindAttribute("normal");

    // uni
    mCol = BindUniform("color");
    mWorl = BindUniform("world");
    //mInvTransWorld = BindUniform("invTransWorld");
    mView = BindUniform("view");
    mProj = BindUniform("proj");

    return true;
  }
}