/*!
\author Nathan Park
How to use:
1. Call Shader::LoadShader once for GL_VERTEX_SHADER 
and once for GL_FRAGMENT_SHADER
2. Call Shader::CompileAndLinkProgram()
3. Bind a bunch of attributes and uniforms
4. When you're going to render a thing, call 
Shader::Activate()
5. Send your uniforms to your shader via
gluniform(... shader("uniform name"))

You should derive the shader to make your own shader classes
and keep the GLuint attributes and uniforms as member variabes
for quick lookup
*/
#pragma once

#include "TacGLIncludes.h"
#include "TacSharedPtr.h"

#include <map>
#include <string>
#include <vector>

namespace Tac
{
  class Shader
  {
  public:
    Shader();
    ~Shader();
    virtual bool Init() = 0;
    // data should be null terminated
    bool LoadFromData(GLenum shaderType, const char * data);

    // example: Load(GL_VERTEX_SHADER, "resources/shaders/shader.vert");
    bool LoadShader(GLenum shaderType, const std::string & filePath); 

    bool CompileAndLinkProgram();

    void Activate() const;
    void Deactivate() const;

    GLuint BindAttribute (const std::string & attrib);
    GLuint BindUniform (const std::string & unifor);

  private:
    std::map<GLenum, GLuint> mShaders;
    GLuint mProgram;

    // used for glEnableVertexAttribArray during Shader::Active();
    std::vector<GLuint> mAttributes; 
  };

} // Tac
