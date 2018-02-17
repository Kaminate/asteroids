/*!
\author Nathan Park
\brief
*/
#ifdef TacDLL_Export
#define TacDLLAPI __declspec(dllexport)
#else
#define TacDLLAPI __declspec(dllimport)
#endif

#ifndef __TAC_SYSTEM_GRAPHICS_H_
#define __TAC_SYSTEM_GRAPHICS_H_

#include "TacSystem.h"
#include "TacCallback.h"

#include "TacShaderPos.h"
#include "TacShaderShadow.h"

#include "TacVector3.h"
#include "TacVector4.h"
#include "TacBoundingSphere.h"
#include "TacShaderFullscreenTexture.h"
#include "TacGeometry.h"
#include "TacShaderVoxelFragments.h"
#include "TacShaderPixels.h"


// temp
#include "TacCS260AsteroidsCommon.h"
#include "TacShaderAsteroids.h"



namespace Tac
{
  // temp
  extern std::vector<Asteroids::Object> gAsteroidsObjects;

  class CameraComponent;
  class LightComponent;

  class GraphicsSystem : public System
  {
  public:
    TacDLLAPI GraphicsSystem();
    TacDLLAPI virtual ~GraphicsSystem();
    TacDLLAPI virtual void Update(float dt);
    TacDLLAPI virtual bool Initialize();
    TacDLLAPI virtual void UpdateFakeTime(float dt);
    TacDLLAPI void Render() const;

    // you can use System::GetRandomComponent(type) to get these
    TacDLLAPI void SetCurrentCamera(CameraComponent * cameraComp);
    TacDLLAPI void SetCurrentLight(LightComponent * lightComp);

    // assumes there are objects in the scene
    TacDLLAPI BoundingSphere GetSceneBoundingSphere() const;

    //void TEMP_RENDER_ASTEROIDS_GAME() const;
    void TEMP_GENERATE_SHADOW_MAP() const;
    void TEMP_RENDER_ASTEROIDS_GAME_SHADOWED() const;

    
  private:
    SPGeometry tempCubeGeom;


    // maybe these should be weakpointers
    CameraComponent * mCurrentCamera;
    LightComponent * mCurrentLight;
    ShaderPos mShaderPos;
    ShaderShadow mShaderShadow;

    void GenerateShadowMap() const;
    void RenderDepthBuffer() const;
    void RenderObjectsShadowed() const;
    void VoxelizeScene() const;
    ShaderFullscreenTexture mShaderFSTex;
    SPGeometry mNDCQuad;
    BoundingSphere mSceneBounds;

    // voxelizing the scene
    ShaderVoxelFragments mShaderVoxelFrags;
    static const unsigned mMaxNumFragments = 1000000;

    GLuint mFragmentBuffer;
    GLuint mCounterBuffer;

    ShaderPixels mShaderPixels;

    //temp
    ShaderAsteroids mShaderAsteroids;
  };

  // base this from ShaderVoxelFragments.f.glsl
  struct Frag
  {
    Vector3 fragPos;
    unsigned padding;
    // fragCol, a vec4 must be aligned on sizeof(vec4) boundaries
    Vector4 fragCol;
  }; // total size: 32 bytes (a multiple of vec4)

  struct Octree
  {

  };
  // OctreeNodes are grouped in tiles of 8
  struct OctreeNode
  {
    unsigned used; // 0 unused 1 used; (bitflags, but only 1 bit)
    unsigned childTileIndex; // 0 is nullptr
    unsigned padding[2];
    Vector4 color;
  };
  
} // Tac

#endif
