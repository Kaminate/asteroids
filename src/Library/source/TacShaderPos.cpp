// Nathan Park
#include "TacShaderPos.h"
namespace Tac
{
  bool ShaderPos::Init()
  {
    if (!LoadShader(GL_VERTEX_SHADER,   "resources/ShaderPos.v.glsl"))
      return false;
    if (!LoadShader(GL_FRAGMENT_SHADER, "resources/ShaderPos.f.glsl"))
      return false;
    if (!CompileAndLinkProgram())
      return false;

    // att
    mPos = BindAttribute("position");
    mNor = BindAttribute("normal");

    // uni
    mCol = BindUniform("color");
    mWorl = BindUniform("world");
    mView = BindUniform("view");
    mProj = BindUniform("proj");
    mLightDir = BindUniform("wsLightDir");
    mLightCol = BindUniform("lightCol");
    mInvTransWorld = BindUniform("invTransWorld");
    mShadowMap = BindUniform("shadowMap");
    mLightView = BindUniform("lightView");
    mLightProj = BindUniform("lightProj");



    return true;
  }
}