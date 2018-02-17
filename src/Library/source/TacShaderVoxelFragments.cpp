// Nathan Park
#include "TacShaderVoxelFragments.h"
namespace Tac
{
  bool ShaderVoxelFragments::Init()
  {
    if (!LoadShader(GL_VERTEX_SHADER, 
      "resources/ShaderVoxelFragments.v.glsl"))
      return false;
    if (!LoadShader(GL_GEOMETRY_SHADER,
      "resources/ShaderVoxelFragments.g.glsl"))
      return false;
    if (!LoadShader(GL_FRAGMENT_SHADER, 
      "resources/ShaderVoxelFragments.f.glsl"))
      return false;
    if (!CompileAndLinkProgram())
      return false;

    // att
    mPos = BindAttribute("position");
    //mNor = BindAttribute("normal"); todo

    // uni
    mCol = BindUniform("color");
    mWorl = BindUniform("world");
    mView = BindUniform("view");
    mProj = BindUniform("proj");
    //mInvTransWorld = BindUniform("invTransWorld"); todo
    mMaxFrags = BindUniform("maxFrags");

    return true;
  }
}