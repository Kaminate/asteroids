/*!
\author Nathan Park
   Holds the vertex buffer objects for the mesh geometry
*/
#ifdef TacDLL_Export
#define TacDLLAPI __declspec(dllexport)
#else
#define TacDLLAPI __declspec(dllimport)
#endif

#pragma once

#include "TacGLFWIncludes.h"
#include "TacVector3.h"
#include "TacVector2.h"
#include "TacSharedPtr.h"
#include "TacBoundingSphere.h"

#include <map>

namespace Tac
{
  class GeometryData;
  class Geometry
  {    
  public:
    enum Attribute
    {
      Position, Color, TexCoord, Normal, BoneId, BoneWeight, Count
    };

    TacDLLAPI Geometry(const GeometryData & data);
    TacDLLAPI ~Geometry();

    TacDLLAPI bool HasAttrib(Attribute attrib) const;
    TacDLLAPI GLuint GetAttribute(Attribute attrib) const;
    TacDLLAPI void SendAttribute(Attribute attrib, GLuint location) const;
    TacDLLAPI void Draw() const;

    const static int BonePerVert = 4;
    const BoundingSphere & GetBoundingSphere() const;

  private:
    BoundingSphere mBoundingSphere;
    unsigned mVertexCount;
    std::map<Attribute, GLuint> mBufferObjects; 

    void LoadVertexAttrib(
      Attribute attrib,
      unsigned entireBufferSize,
      void * data);
  };
  DefineSharedPtr(Geometry);
} // Tac

