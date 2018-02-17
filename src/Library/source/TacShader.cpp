/*!
\author Nathan Park
*/
#include "TacShader.h"
#include "TacFileReader.h"
#include "TacErrorIf.h"
#include "TacGraphicsErrors.h"

namespace Tac
{
  Shader::Shader() : mProgram(-1)
  {

  }


  Shader::~Shader()
  {
    // todo: for each shader
    // glDeleteShader(shader)

    if (mProgram != -1)
    {
      glDeleteProgram(mProgram);
    }
  }


  bool Shader::LoadFromData( GLenum shaderType,const char * data )
  {
    GLuint shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, &data, nullptr);

    glCompileShader(shader);
    GLint result = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    bool success = result != GL_FALSE;
    if (success)
    {
      mShaders[shaderType] = shader; 
    }
    else
    {
      GLint errorLogLength;
      glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &errorLogLength);
      std::unique_ptr<GLchar> errorLog(new GLchar[errorLogLength]);
      glGetShaderInfoLog (shader, errorLogLength, NULL, errorLog.get());
      std::string errorString(errorLog.get());
      std::vector<std::string> errors;
      std::string curr;
      for (char c : errorString)
      {
        if (c == '\n')
        {
          errors.push_back(curr);
          curr.clear();
        }
        else
        {
          curr += c;
        }
      }
      ErrorIf(true, "%s", errorLog.get());
    }
    return success;
  }

  bool Shader::LoadShader(
    GLenum shaderType, 
    const std::string & shaderPath) 
  {
    auto it = mShaders.find(shaderType);
    bool inMap = it != mShaders.end();
    ErrorIf(inMap, "There already exists a shader of this type");

    FileReader reader;
    if (!reader.ReadFile(shaderPath))
      return false;

    return LoadFromData(shaderType, reader.GetSource());
  }


  bool Shader::CompileAndLinkProgram()
  {
    mProgram = glCreateProgram();
    for (auto it = mShaders.begin(); it != mShaders.end(); ++it)
    {
      auto pair = *it; //GLenum, GLuint
      GLuint shader = pair.second;
      glAttachShader(mProgram, shader);
    }
    glLinkProgram(mProgram);
    GLint link_ok;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &link_ok);

    bool success = link_ok != GL_FALSE;
    if (success)
    {
      // Done linking program.
      // We don't need the shaders anymore, so lets delete them
      for (auto it = mShaders.begin(); it != mShaders.end(); ++it)
      {
        auto pair = *it; //GLenum, GLuint
        GLuint shader = pair.second;
        glDeleteShader(shader);
      }
      mShaders.clear();
    }
    else
    {
      GLint errorLogLength;
      glGetProgramiv (mProgram, GL_INFO_LOG_LENGTH, &errorLogLength);
      std::unique_ptr<GLchar> errorLog(new GLchar[errorLogLength]);
      glGetProgramInfoLog (mProgram, errorLogLength, NULL, errorLog.get());
      ErrorIf(true, "%s", errorLog.get());
    }
    return success;
  }

  void Shader::Activate() const
  {
    ErrorIf(mProgram == -1, 
      "This shader doesn't have a program."
      "Double check to see that it was actually "
      "compiled in the first place");
    glUseProgram(mProgram);
    for (auto attrib : mAttributes)
    {
      glEnableVertexAttribArray(attrib);
    }
  }

  void Shader::Deactivate() const
  {
    glUseProgram(0);
    for (unsigned i = 0; i < mAttributes.size(); ++i)
    {
      GLuint attrib = mAttributes[i];
      glDisableVertexAttribArray(attrib);
    }

  }

  GLuint Shader::BindAttribute(const std::string & attrib)
  {
    GLuint location =  glGetAttribLocation(mProgram, attrib.c_str());
    CheckOpenGLErrors();
    ErrorIf(location == -1, 
      "The shader does not have attribute %s." 
      "Make sure you've called Shader::CompileAndLinkProgram()"
      "And make sure that you dont actually want to bind a UNIFORM",
      attrib.c_str());
    mAttributes.push_back(location);
    return location;
  }

  GLuint Shader::BindUniform(const std::string & unifor)
  {
    GLuint location = glGetUniformLocation(mProgram, unifor.c_str());
    CheckOpenGLErrors();
    ErrorIf(location == -1, 
      " > The shader does not have uniform %s \n"
      " Possible Reason: You called Shader::CompileAndLinkProgram()? \n"
      " Possible Reason: BindUniform(), not BindAttribute()? \n"
      " Possible Reason: The uniform is unused, and was optimized out\n",
      unifor.c_str());
    return location;
  }

} // Tac


