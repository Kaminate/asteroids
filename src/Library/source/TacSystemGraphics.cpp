/*!
\author Nathan Park
\par Project: Tac Engine
\brief
*/
#include "TacCore.h"
#include "TacErrorIf.h"
#include "TacGLFWIncludes.h"
#include "TacSystemGraphics.h"
#include "TacGraphicsErrors.h"
#include "TacComponentModel.h"
#include "TacComponentLight.h"
#include "TacComponentCamera.h"
#include "TacComponentTransform.h"
#include <TacBoundingSphere.h>
#include "TacGeometryData.h"
#include "TacMathUtil.h"


namespace Tac
{

  std::vector<Asteroids::Object> gAsteroidsObjects;


  class RenderCallback : public Callback
  {
    GraphicsSystem * mGraphics;
  public:
    RenderCallback::RenderCallback( 
      GraphicsSystem * graphicsSys ) 
      : mGraphics(graphicsSys)
    {

    }
    virtual void RenderCallback::Execute()
    {
      mGraphics->Render();
    }
  };

  GraphicsSystem::GraphicsSystem()
    : mCurrentCamera(nullptr)
    , mCurrentLight(nullptr)
  {

  }

  GraphicsSystem::~GraphicsSystem()
  {
    // todo:
    // delete the shader storage buffers
    // (if it was init)
    // (which it might not be if the things before it failed to init
  }

  void GraphicsSystem::Update( float dt )
  {


  }

  bool GraphicsSystem::Initialize()
  {
    bool success = true;
    do 
    {
      // set up system callback
      GetCore()->SetRenderCallback(SPCallback(new RenderCallback(this)));

      // init glew
      GLenum err = glewInit();
      if(err != GLEW_OK)
      {
        std::string errorString("glew init failed: ");
        errorString.append((const char*)glewGetErrorString(err));
        success = false; 
        break;
      }

      const GLubyte * glewVersion = glewGetString(GLEW_VERSION);

      const unsigned char * glVersion
        = glGetString(GL_VERSION);
      std::cout << "gl version " << glVersion << std::endl;


      const unsigned char * glslVersion 
        = glGetString(GL_SHADING_LANGUAGE_VERSION);
      std::cout << "glsl version " << glslVersion << std::endl;



      // for drawing the depth buffer to the screen
      mNDCQuad = SPGeometry(new Geometry(GeometryData().LoadNDCQuad()));

      std::vector<Shader*> shaders;
      shaders.push_back(&mShaderPos);
      shaders.push_back(&mShaderShadow);
      shaders.push_back(&mShaderFSTex);
      shaders.push_back(&mShaderVoxelFrags);
      shaders.push_back(&mShaderPixels);
      shaders.push_back(&mShaderAsteroids);
      for (Shader * shader : shaders)
      {
        if (!shader->Init())
          return false;
      }

      // voxelization
      glGenBuffers(1, &mFragmentBuffer);
      glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER, 
        ShaderVoxelFragments::mFragBufferBinding, 
        mFragmentBuffer);
      glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        mMaxNumFragments * sizeof(Frag),
        nullptr,
        GL_DYNAMIC_DRAW);

      // i think just calling glBindBufferBase is enough to set the 
      // binding point of the buffer
      glGenBuffers(1, &mCounterBuffer);
      glBindBufferBase(
        GL_ATOMIC_COUNTER_BUFFER, 
        ShaderVoxelFragments::mFragCountBinding, 
        mCounterBuffer);

      glBufferData(
        GL_ATOMIC_COUNTER_BUFFER, 
        sizeof(GLuint), 
        nullptr,
        GL_DYNAMIC_DRAW);
      CheckOpenGLErrors();
    } while (false);


    // TEMP
    GeometryData data;
    if (!data.Load("resources/cube.tacmesh"))
      ErrorIf(true, "");
    tempCubeGeom = SPGeometry(new Geometry(data));

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);
    glClear(
      GL_COLOR_BUFFER_BIT | 
      GL_DEPTH_BUFFER_BIT | 
      GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    CheckOpenGLErrors();

    return success;
  }

  void GraphicsSystem::UpdateFakeTime( float dt )
  {
    if (!mCurrentCamera)
      return;

    if (!mCurrentLight)
      return;

    ComponentSet models = GetComponentSet(Comp::eCompModel); 
    if (models.empty())
      return;

    // update boundingsphere
    mSceneBounds = GetSceneBoundingSphere();

    // Update the camera
    TransformComp * trans = (TransformComp*) mCurrentCamera->GetSibling(
      Comp::eCompTransform);
    Camera & cam = mCurrentCamera->mCamera;
    cam.mPosition = trans->mPos;

    // update the lightCam
    float diameter = mSceneBounds.mRadius * 2;
    Camera & lightCam = mCurrentLight->mCamera;
    lightCam.mPosition = mSceneBounds.mPos;
    lightCam.mViewDirection = mCurrentLight->mDirection;
    lightCam.mNear = -mSceneBounds.mRadius;
    lightCam.mFar = mSceneBounds.mRadius;
    lightCam.mWidth = diameter;
    lightCam.mHeight = diameter;
  }

  void GraphicsSystem::Render() const
  {
    glClear(
      GL_COLOR_BUFFER_BIT | 
      GL_DEPTH_BUFFER_BIT | 
      GL_STENCIL_BUFFER_BIT);

    //*
    //GenerateShadowMap();
    //RenderObjectsShadowed();
    //RenderDepthBuffer();
    /*/
    VoxelizeScene();
    //*/

    TEMP_GENERATE_SHADOW_MAP();
    TEMP_RENDER_ASTEROIDS_GAME_SHADOWED();
    

    //TEMP_RENDER_ASTEROIDS_GAME();
  }

  void GraphicsSystem::SetCurrentCamera( 
    CameraComponent * cameraComp )
  {
    mCurrentCamera = cameraComp;
  }

  void GraphicsSystem::SetCurrentLight( 
    LightComponent * lightComp )
  {
    mCurrentLight = lightComp;
  }

  // assumes there are objects in the scene
  BoundingSphere GraphicsSystem::GetSceneBoundingSphere() const
  {
    BoundingSphere sceneBounds;
    bool initialized = false;
    for (SPComp comp : GetComponentSet(Comp::eCompModel))
    {
      ModelComponent * iModel = (ModelComponent*) comp.get();
      TransformComp * trans 
        = (TransformComp*) iModel->GetSibling(Comp::Type::eCompTransform);
      Geometry * geom = iModel->GetGeometry();
      if (!geom)
        continue;
      BoundingSphere sphere = geom->GetBoundingSphere();
      if (trans)
      {
        sphere.mPos += trans->mFakePos;
        const Vector3 & scale = trans->mSca;

        using std::max;
        float maxScale = max(max(scale.x, scale.y), scale.z);
        sphere.mRadius *= maxScale;
      }

      if (initialized)
      {
        sceneBounds.AddSphere(sphere);
      }
      else if (!initialized)
      {
        sceneBounds = sphere;
        initialized = true;
      }
    }
    return sceneBounds;
  }

  void GraphicsSystem::RenderDepthBuffer() const
  {
    if (!mCurrentLight)
      return;

    Window & window = GetCore()->GetWindow();
    int depthWidth = 200;
    int depthHeight = (int)((float)depthWidth / window.GetAspect());
    glViewport(window.GetWidth() - depthWidth, 0, depthWidth, depthHeight);

    mShaderFSTex.Activate();

    // sampler 0 to texture 0
    glUniform1i(mShaderFSTex.mTexture, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, mCurrentLight->mDepthTexture);

    mNDCQuad->SendAttribute(Geometry::Position, mShaderFSTex.mPos);
    mNDCQuad->SendAttribute(Geometry::TexCoord, mShaderFSTex.mTexCoords);
    mNDCQuad->Draw();

    mShaderFSTex.Deactivate();
  }

  void GraphicsSystem::RenderObjectsShadowed() const
  {
    CheckOpenGLErrors();
    if (!mCurrentCamera)
      return;

    if (!mCurrentLight)
      return;

    TransformComp * trans = (TransformComp*) mCurrentCamera->GetSibling(
      Comp::eCompTransform);
    Camera & cam = mCurrentCamera->mCamera;
    Camera & lightCam = mCurrentLight->mCamera;
    Matrix4 lightview = lightCam.GetViewMatrix();
    Matrix4 lightproj = lightCam.GetOrthographicProjMatrix();

    glClear(
      GL_DEPTH_BUFFER_BIT | 
      GL_COLOR_BUFFER_BIT | 
      GL_STENCIL_BUFFER_BIT);
    Window & window = GetCore()->GetWindow();
    glViewport(0,0,window.GetWidth(), window.GetHeight());

    mShaderPos.Activate();
    Matrix4 view = cam.GetViewMatrix();
    Matrix4 proj = cam.GetPerspectiveProjMatrix();
    glUniformMatrix4fv(mShaderPos.mView, 1, GL_TRUE, view.v);
    glUniformMatrix4fv(mShaderPos.mProj, 1, GL_TRUE, proj.v);

    const Vector4 & c = mCurrentLight->mColor;
    glUniform4f(mShaderPos.mLightCol, c.x, c.y, c.z, c.w);
    const Vector3 & d = mCurrentLight->mDirection;
    glUniform3f(mShaderPos.mLightDir, d.x, d.y, d.z);

    glUniformMatrix4fv(mShaderPos.mLightView, 1, GL_TRUE, lightview.v);
    glUniformMatrix4fv(mShaderPos.mLightProj, 1, GL_TRUE, lightproj.v);

    // sampler 0 to texture 0
    glUniform1i(mShaderPos.mShadowMap, 0);
    glActiveTexture(GL_TEXTURE0 + 0);

    glBindTexture(GL_TEXTURE_2D, mCurrentLight->mDepthTexture);

    for (SPComp comp: GetComponentSet(Comp::eCompModel))
    {
      ModelComponent * iModel = (ModelComponent*) comp.get();

      // color
      const Vector4 & c = iModel->mColor;
      glUniform4f(mShaderPos.mCol, c.x, c.y, c.z, c.w);

      // world matrix
      Matrix4 world;
      TransformComp * iTransform 
        = (TransformComp*) comp->GetSibling(Comp::Type::eCompTransform);
      if (iTransform)
      {
        // inversing the matrix will errorif if theres a zero row
        if (IsAboutZero(iTransform->mSca.x)||
          IsAboutZero(iTransform->mSca.y)||
          IsAboutZero(iTransform->mSca.z))
          continue;

        world = iTransform->mFakeTransform;
      }
      glUniformMatrix4fv(mShaderPos.mWorl, 1, GL_TRUE, world.v);
      // normal matrix
      Matrix4 invtransWorld = world;
      invtransWorld.Invert();
      invtransWorld.Transpose();
      glUniformMatrix4fv(
        mShaderPos.mInvTransWorld, 1, GL_TRUE, invtransWorld.v);

      if(Geometry * geom = iModel->GetGeometry())
      {
        geom->SendAttribute(Geometry::Position, mShaderPos.mPos);
        geom->SendAttribute(Geometry::Normal, mShaderPos.mNor);
        geom->Draw();
      }
    }
    mShaderPos.Deactivate();
  }

  void GraphicsSystem::VoxelizeScene() const
  {
    auto compSet = GetComponentSet(Comp::eCompModel);
    if (compSet.empty())
      return;

    if (!mCurrentCamera)
      return;

    // voxelize scene
    //glViewport(0,0,1024,1024); // resolution of voxel grid
    const Window & wind = GetCore()->GetWindow();
    glViewport(0, 0, wind.GetWidth(), wind.GetHeight());

    // www.opengl.org/registry/specs/ARB/shader_storage_buffer_object.txt
    // has an example of using a shader storage buffer object to record
    // a list of rasterized fragments (x,y ) and colors
    // 
    // its under "sample code"

    mShaderVoxelFrags.Activate();
    glDrawBuffer(GL_NONE);
    glEnable(GL_DEPTH_TEST);


    GLuint zero = 0;
    glBufferSubData(
      GL_ATOMIC_COUNTER_BUFFER, 
      0,
      sizeof(GLuint), 
      &zero);

    glUniform1ui(mShaderVoxelFrags.mMaxFrags, mMaxNumFragments);

    Matrix4 view = mCurrentCamera->mCamera.GetViewMatrix();
    Matrix4 proj = mCurrentCamera->mCamera.GetPerspectiveProjMatrix();
    glUniformMatrix4fv(mShaderVoxelFrags.mView, 1, GL_TRUE, view.v);
    glUniformMatrix4fv(mShaderVoxelFrags.mProj, 1, GL_TRUE, proj.v);

    for (SPComp comp: compSet)
    {
      ModelComponent * iModel = (ModelComponent*) comp.get();

      Matrix4 world;
      TransformComp * iTransform 
        = (TransformComp*) comp->GetSibling(Comp::Type::eCompTransform);
      if (iTransform)
        world = iTransform->mFakeTransform;
      glUniformMatrix4fv(mShaderVoxelFrags.mWorl, 1, GL_TRUE, world.v);

      // color
      const Vector4 & c = iModel->mColor;
      glUniform4f(mShaderVoxelFrags.mCol, c.x, c.y, c.z, c.w);

      if(Geometry * geom = iModel->GetGeometry())
      {
        geom->SendAttribute(Geometry::Position, mShaderVoxelFrags.mPos);
        geom->Draw();
      }
    } // draw each model

    mShaderVoxelFrags.Deactivate();


    glDrawBuffer(GL_BACK);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
    void * ptr = glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_ONLY);
    GLuint numIncrements = *reinterpret_cast<GLuint*>(ptr);
    glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

    unsigned numPixelsToDraw = std::min(numIncrements, mMaxNumFragments);

    // just inspectin
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    void * vPtr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    Frag * fragPtr = reinterpret_cast<Frag*>(vPtr);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    mShaderPixels.Activate();

    // You could also use the storage buffer contents for vertex pulling.
    // The glMemoryBarrier() command ensures that the data writes to the
    // storage buffer complete prior to vertex pulling.

    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, mFragmentBuffer);
    glVertexAttribPointer(
      mShaderPixels.mPos,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Frag),
      (void*)offsetof(Frag, Frag::fragPos));

    glVertexAttribPointer(
      mShaderPixels.mCol,
      4, // 4 components in vec4 
      GL_FLOAT,
      GL_FALSE, 
      sizeof(Frag),
      (void*)offsetof(Frag, Frag::fragCol));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // temp:
    // create a buffer to set just the center pixel red
    /*
    static GLuint tempBuffer;
    static bool once = true;
    if (once)
    {
    once = false;

    Frag data;
    data.col.r = 1;
    data.col.g = 0;
    data.col.b = 0;
    data.col.a = 1;
    data.padding[0] = 0;
    data.padding[1] = 0;
    data.padding[2] = 0;
    data.padding[3] = 0;
    data.padding[4] = 0;
    data.padding[5] = 0;
    data.padding[6] = 0;
    data.padding[7] = 0;
    data.pos.x = 400;
    data.pos.y = 300;
    glGenBuffers(1, &tempBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tempBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Frag), &data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    */

    /*
    unsigned posOffset = offsetof(Frag, Frag::pos);
    unsigned colOffset = offsetof(Frag, Frag::col);
    unsigned fraOffset = sizeof(Frag);
    std::cout << posOffset << colOffset << fraOffset;

    glBindBuffer(GL_ARRAY_BUFFER, tempBuffer);
    glVertexAttribIPointer(
    mShaderPixels.mPos,
    2,
    GL_UNSIGNED_INT,
    mFragmentSizeInBytes,
    (void*) 0);
    glVertexAttribPointer(
    mShaderPixels.mCol,
    4,
    GL_FLOAT,
    GL_FALSE,
    mFragmentSizeInBytes,
    (void *) 16);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    */

    //glUniform1ui(mShaderPixels.mScreenWidth, wind.GetWidth());
    //glUniform1ui(mShaderPixels.mScreenHeight, wind.GetHeight());
    glUniformMatrix4fv(mShaderPixels.mView, 1, GL_TRUE, view.v);
    glUniformMatrix4fv(mShaderPixels.mProj, 1, GL_TRUE, proj.v);

    glPointSize(1.0f);
    glDrawArrays(GL_POINTS, 0, numPixelsToDraw);

    mShaderPixels.Deactivate();

    CheckOpenGLErrors();
  }

  void GraphicsSystem::GenerateShadowMap() const
  {
    if (!mCurrentLight)
      return;

    Camera & lightCam = mCurrentLight->mCamera;
    Matrix4 lightview = lightCam.GetViewMatrix();
    Matrix4 lightproj = lightCam.GetOrthographicProjMatrix();
    CheckOpenGLErrors();


    glViewport(0,0,1024,1024);
    mShaderShadow.Activate();
    glUniformMatrix4fv(mShaderShadow.mView, 1, GL_TRUE, lightview.v);
    glUniformMatrix4fv(mShaderShadow.mProj, 1, GL_TRUE, lightproj.v);
    CheckOpenGLErrors();

    glBindFramebuffer(GL_FRAMEBUFFER, mCurrentLight->mFramebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    // disable drawing
    glDrawBuffer(GL_NONE);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    CheckOpenGLErrors();
    for (SPComp comp: GetComponentSet(Comp::eCompModel))
    {
      ModelComponent * iModel = (ModelComponent*) comp.get();

      // world matrix
      Matrix4 world;
      if (TransformComp * iTransform 
        = (TransformComp*) comp->GetSibling(Comp::Type::eCompTransform))
        world = iTransform->mFakeTransform;
      glUniformMatrix4fv(mShaderShadow.mWorl, 1, GL_TRUE, world.v);

      CheckOpenGLErrors();
      if(Geometry * geom = iModel->GetGeometry())
      {
        geom->SendAttribute(Geometry::Position, mShaderShadow.mPos);
        CheckOpenGLErrors();
        geom->Draw();
        CheckOpenGLErrors();
      }
    }
    mShaderShadow.Deactivate();
    CheckOpenGLErrors();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CheckOpenGLErrors();

    glDrawBuffer(GL_BACK);
    CheckOpenGLErrors();
  }

  /*

  void GraphicsSystem::TEMP_RENDER_ASTEROIDS_GAME() const
  {
    using namespace Asteroids;

    CheckOpenGLErrors();
    if (!mCurrentCamera)
      return;

    TransformComp * trans 
      = (TransformComp*) mCurrentCamera->GetSibling(Comp::eCompTransform);

    Camera & cam = mCurrentCamera->mCamera;

    Window & window = GetCore()->GetWindow();
    glViewport(0,0,window.GetWidth(), window.GetHeight());

    mShaderAsteroids.Activate();
    static Matrix4 view = cam.GetViewMatrix();
    static Matrix4 proj = cam.GetPerspectiveProjMatrix();
    glUniformMatrix4fv(mShaderAsteroids.mView, 1, GL_TRUE, view.v);
    glUniformMatrix4fv(mShaderAsteroids.mProj, 1, GL_TRUE, proj.v);

    Geometry * geom = tempCubeGeom.get();
    geom->SendAttribute(Geometry::Position, mShaderAsteroids.mPos);
    geom->SendAttribute(Geometry::Normal, mShaderAsteroids.mNor);

    for (Object & obj : gAsteroidsObjects)
    {
      Object::Type objType = obj.mType;

      Vector4 color = GetColor(obj);
      glUniform4f(mShaderAsteroids.mCol, color.x, color.y, color.z, color.w);

      float rotationAngle = atan2(obj.mVelY, obj.mVelX);
      float scale = 1;

      Matrix4 world = Matrix4::Transform(
        Vector3(scale, scale, scale),
        Vector3(0,0,rotationAngle), 
        Vector3(obj.mPosX, obj.mPosY, 0));

      glUniformMatrix4fv(mShaderAsteroids.mWorl, 1, GL_TRUE, world.v);

      geom->Draw();
    }
    mShaderAsteroids.Deactivate();
    CheckOpenGLErrors();
  }

  */

  void GraphicsSystem::TEMP_RENDER_ASTEROIDS_GAME_SHADOWED() const
  {
    CheckOpenGLErrors();
    if (!mCurrentCamera)
      return;

    if (!mCurrentLight)
      return;

    TransformComp * trans = (TransformComp*) mCurrentCamera->GetSibling(
      Comp::eCompTransform);
    Camera & cam = mCurrentCamera->mCamera;
    Camera & lightCam = mCurrentLight->mCamera;
    Matrix4 lightview = lightCam.GetViewMatrix();
    Matrix4 lightproj = lightCam.GetOrthographicProjMatrix();

    glClear(
      GL_DEPTH_BUFFER_BIT | 
      GL_COLOR_BUFFER_BIT | 
      GL_STENCIL_BUFFER_BIT);
    Window & window = GetCore()->GetWindow();
    glViewport(0,0,window.GetWidth(), window.GetHeight());

    mShaderPos.Activate();
    Matrix4 view = cam.GetViewMatrix();
    Matrix4 proj = cam.GetPerspectiveProjMatrix();
    glUniformMatrix4fv(mShaderPos.mView, 1, GL_TRUE, view.v);
    glUniformMatrix4fv(mShaderPos.mProj, 1, GL_TRUE, proj.v);

    const Vector4 & c = mCurrentLight->mColor;
    glUniform4f(mShaderPos.mLightCol, c.x, c.y, c.z, c.w);
    const Vector3 & d = mCurrentLight->mDirection;
    glUniform3f(mShaderPos.mLightDir, d.x, d.y, d.z);

    glUniformMatrix4fv(mShaderPos.mLightView, 1, GL_TRUE, lightview.v);
    glUniformMatrix4fv(mShaderPos.mLightProj, 1, GL_TRUE, lightproj.v);

    // sampler 0 to texture 0
    glUniform1i(mShaderPos.mShadowMap, 0);
    glActiveTexture(GL_TEXTURE0 + 0);

    glBindTexture(GL_TEXTURE_2D, mCurrentLight->mDepthTexture);

    for (SPComp comp: GetComponentSet(Comp::eCompModel))
    {
      ModelComponent * iModel = (ModelComponent*) comp.get();

      // color
      const Vector4 & c = iModel->mColor;
      glUniform4f(mShaderPos.mCol, c.x, c.y, c.z, c.w);

      // world matrix
      Matrix4 world;
      TransformComp * iTransform 
        = (TransformComp*) comp->GetSibling(Comp::Type::eCompTransform);
      if (iTransform)
      {
        // inversing the matrix will errorif if theres a zero row
        if (IsAboutZero(iTransform->mSca.x)||
          IsAboutZero(iTransform->mSca.y)||
          IsAboutZero(iTransform->mSca.z))
          continue;

        world = iTransform->mFakeTransform;
      }
      glUniformMatrix4fv(mShaderPos.mWorl, 1, GL_TRUE, world.v);
      // normal matrix
      Matrix4 invtransWorld = world;
      invtransWorld.Invert();
      invtransWorld.Transpose();
      glUniformMatrix4fv(
        mShaderPos.mInvTransWorld, 1, GL_TRUE, invtransWorld.v);

      if(Geometry * geom = iModel->GetGeometry())
      {
        geom->SendAttribute(Geometry::Position, mShaderPos.mPos);
        geom->SendAttribute(Geometry::Normal, mShaderPos.mNor);
        geom->Draw();
      }
    }
    using namespace Asteroids;
    for (Object & obj : gAsteroidsObjects)
    {
      Object::Type objType = obj.mType;

      Vector4 color = GetColor(obj);
      glUniform4f(mShaderPos.mCol, color.x, color.y, color.z, color.w);

      float rotationAngle = atan2(obj.mVelY, obj.mVelX);

      float scale = GetScale(objType);
      Matrix4 world = Matrix4::Transform(
        Vector3(scale),
        Vector3(0,0,rotationAngle), 
        Vector3(obj.mPosX, obj.mPosY, 0));
      glUniformMatrix4fv(mShaderPos.mWorl, 1, GL_TRUE, world.v);

      Matrix4 invtransWorld = world;
      invtransWorld.Invert();
      invtransWorld.Transpose();
      glUniformMatrix4fv(
        mShaderPos.mInvTransWorld, 1, GL_TRUE, invtransWorld.v);

      Geometry * geom = tempCubeGeom.get();
      geom->SendAttribute(Geometry::Position, mShaderPos.mPos);
      geom->SendAttribute(Geometry::Normal, mShaderPos.mNor);
      geom->Draw();
    }
    mShaderPos.Deactivate();

  }

  void GraphicsSystem::TEMP_GENERATE_SHADOW_MAP() const
  {
    if (!mCurrentLight)
      return;

    Camera & lightCam = mCurrentLight->mCamera;
    Matrix4 lightview = lightCam.GetViewMatrix();
    Matrix4 lightproj = lightCam.GetOrthographicProjMatrix();
    CheckOpenGLErrors();


    glViewport(0,0,1024,1024);
    mShaderShadow.Activate();
    glUniformMatrix4fv(mShaderShadow.mView, 1, GL_TRUE, lightview.v);
    glUniformMatrix4fv(mShaderShadow.mProj, 1, GL_TRUE, lightproj.v);
    CheckOpenGLErrors();

    glBindFramebuffer(GL_FRAMEBUFFER, mCurrentLight->mFramebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    // disable drawing
    glDrawBuffer(GL_NONE);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    CheckOpenGLErrors();
    for (SPComp comp: GetComponentSet(Comp::eCompModel))
    {
      ModelComponent * iModel = (ModelComponent*) comp.get();

      // world matrix
      Matrix4 world;
      if (TransformComp * iTransform 
        = (TransformComp*) comp->GetSibling(Comp::Type::eCompTransform))
        world = iTransform->mFakeTransform;
      glUniformMatrix4fv(mShaderShadow.mWorl, 1, GL_TRUE, world.v);

      CheckOpenGLErrors();
      if(Geometry * geom = iModel->GetGeometry())
      {
        geom->SendAttribute(Geometry::Position, mShaderShadow.mPos);
        CheckOpenGLErrors();
        geom->Draw();
        CheckOpenGLErrors();
      }
    }

    using namespace Asteroids;
    for (Object & obj : gAsteroidsObjects)
    {
      Object::Type objType = obj.mType;

      float rotationAngle = atan2(obj.mVelY, obj.mVelX);

      float scale = GetScale(objType);
      Matrix4 world = Matrix4::Transform(
        Vector3(scale),
        Vector3(0,0,rotationAngle), 
        Vector3(obj.mPosX, obj.mPosY, 0));

      glUniformMatrix4fv(mShaderShadow.mWorl, 1, GL_TRUE, world.v);
      Geometry * geom = tempCubeGeom.get();
      geom->SendAttribute(Geometry::Position, mShaderShadow.mPos);
      geom->Draw();
    }
    mShaderShadow.Deactivate();
    CheckOpenGLErrors();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CheckOpenGLErrors();

    glDrawBuffer(GL_BACK);
    CheckOpenGLErrors();
  }

} // Tac