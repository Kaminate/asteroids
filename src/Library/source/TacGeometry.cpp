/*!
\author Nathan Park
Holds the vertex buffer objects for the mesh geometry
*/
#include "TacGeometry.h"
#include "TacGeometryData.h"
#include "TacGraphicsErrors.h"
#include "TacErrorIf.h"

namespace Tac
{
  Geometry::Geometry(const GeometryData & data)
    : mVertexCount ( data.mCounts[Attribute::Position])
    , mBoundingSphere(data.mBoundingSphere)
  {
    LoadVertexAttrib(
      Position,
      sizeof(Vector3) * data.mCounts[Attribute::Position],
      data.mPositions);

    LoadVertexAttrib(
      Color,
      sizeof(Vector3) * data.mCounts[Attribute::Color],
      data.mColors);

    LoadVertexAttrib(
      TexCoord,
      sizeof(Vector2) * data.mCounts[Attribute::TexCoord],
      data.mTexCoords);
    
    LoadVertexAttrib(
      Normal,
      sizeof(Vector3) * data.mCounts[Attribute::Normal],
      data.mNormals);

    LoadVertexAttrib(
     BoneId,
     sizeof(unsigned) * data.mCounts[Attribute::BoneId],
     data.mBoneIds);

    LoadVertexAttrib(BoneWeight, 
      sizeof(float) * data.mCounts[Attribute::BoneWeight], 
      data.mBoneWeights);
  }

  Geometry::~Geometry()
  {
    for (auto pair : mBufferObjects)
    {
      GLuint buffer = pair.second;
      glDeleteBuffers(1, &buffer);
    }
  }

  bool Geometry::HasAttrib(Attribute attrib) const
  {
    return mBufferObjects.find(attrib) != mBufferObjects.end();
  }

  GLuint Geometry::GetAttribute( Attribute attrib ) const
  {
    ErrorIf(!HasAttrib(attrib), "Please call HasAttribute() first");
    return mBufferObjects.at(attrib);
  }

  void Geometry::SendAttribute( Attribute attrib, GLuint loc ) const
  {
    glBindBuffer(GL_ARRAY_BUFFER, GetAttribute(attrib));

    switch (attrib)
    {
    case Tac::Geometry::Position:
      glVertexAttribPointer(loc,3,GL_FLOAT, GL_FALSE, 0, 0);
      break;
    case Tac::Geometry::Color:
      glVertexAttribPointer(loc,3,GL_FLOAT, GL_FALSE, 0, 0);
      break;
    case Tac::Geometry::TexCoord:
      glVertexAttribPointer(loc,2,GL_FLOAT, GL_FALSE, 0, 0);
      break;
    case Tac::Geometry::Normal:
      glVertexAttribPointer(loc,3,GL_FLOAT, GL_FALSE, 0, 0);
      break;
    case Tac::Geometry::BoneId:
      glVertexAttribIPointer(loc, BonePerVert,GL_UNSIGNED_INT,0,0);
      break;
    case Tac::Geometry::BoneWeight:
      glVertexAttribPointer(loc,BonePerVert,GL_FLOAT,GL_FALSE,0,0);
      break;
    default:
      ErrorIf(true, "Don't know how to send this attribute");
      break;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CheckOpenGLErrors();
  }

  void Geometry::Draw() const
  {
    glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
    CheckOpenGLErrors();
  }

  void Geometry::LoadVertexAttrib( 
    Attribute attrib, 
    unsigned entireBufferSize,
    void * data )
  {
    if (entireBufferSize == 0)
      return;

    ErrorIf(HasAttrib(attrib), "This attribute has already been loaded");

    GLuint buffer;

    // it will crash here if glew is uninitialized
    glGenBuffers(1, &buffer); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      entireBufferSize,
      data,
      GL_STATIC_DRAW);

    mBufferObjects[attrib] = buffer;

    CheckOpenGLErrors();
  }

  const BoundingSphere & Geometry::GetBoundingSphere() const
  {
    return mBoundingSphere;
  }

} // Tac
