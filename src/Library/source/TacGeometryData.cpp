/*!
\author Nathan Park
Holds the vertex buffer objects for the mesh geometry
*/

#include "TacGeometryData.h"
#include "TacGLFWIncludes.h"
#include "TacVeryUtil.h"
#include <TacErrorIf.h>
namespace Tac
{

  bool GeometryData::Load( const std::string & fileName )
  {
    Clear();


    std::string curDir;
    TacGetCurrentDir(curDir);

    std::ifstream ifs(fileName, std::ifstream::binary);
    bool success = ifs.is_open();
    if(success)
    {
      ifs.read(reinterpret_cast<char*>(mCounts), sizeof(mCounts));
      Load(ifs, mCounts[Geometry::Attribute::Position], mPositions);
      Load(ifs, mCounts[Geometry::Attribute::Color], mColors);
      Load(ifs, mCounts[Geometry::Attribute::TexCoord], mTexCoords);
      Load(ifs, mCounts[Geometry::Attribute::Normal], mNormals);
      Load(ifs, mCounts[Geometry::Attribute::BoneId], mBoneIds);
      Load(ifs, mCounts[Geometry::Attribute::BoneWeight], mBoneWeights);
      ifs.read(reinterpret_cast<char*>(&mBoundingSphere.mPos), 
        sizeof(mBoundingSphere.mPos));
      ifs.read(reinterpret_cast<char*>(&mBoundingSphere.mRadius), 
        sizeof(mBoundingSphere.mRadius));

      ifs.close();
    }
    else
    {
      std::string error;

      char buffer[100];
      strerror_s(buffer,  sizeof(buffer), errno);
    }
    return success;
  }

  bool GeometryData::Save( const std::string & fileName )
  {
    std::ofstream ofs(fileName, std::ofstream::binary);
    bool success = ofs.is_open();
    if (success)
    {
      ofs.write(reinterpret_cast<const char*>(mCounts), sizeof(mCounts));
      Save(ofs, mCounts[Geometry::Attribute::Position], mPositions);
      Save(ofs, mCounts[Geometry::Attribute::Color], mColors);
      Save(ofs, mCounts[Geometry::Attribute::TexCoord], mTexCoords);
      Save(ofs, mCounts[Geometry::Attribute::Normal], mNormals);
      Save(ofs, mCounts[Geometry::Attribute::BoneId], mBoneIds);
      Save(ofs, mCounts[Geometry::Attribute::BoneWeight], mBoneWeights);
      mBoundingSphere.Load(*this);
      ofs.write(reinterpret_cast<const char*>(&mBoundingSphere.mPos), 
        sizeof(mBoundingSphere.mPos));
      ofs.write(reinterpret_cast<const char*>(&mBoundingSphere.mRadius), 
        sizeof(mBoundingSphere.mRadius));
    }
    return success;
  }

  GeometryData::~GeometryData()
  {
    Clear();
  }

  void GeometryData::Clear()
  {
    memset(mCounts, 0, sizeof(mCounts));

    delete mPositions;   mPositions = nullptr;
    delete mColors;      mColors= nullptr;   
    delete mTexCoords;   mTexCoords= nullptr;
    delete mNormals;     mNormals= nullptr; 
    delete mBoneIds;     mBoneIds= nullptr;
    delete mBoneWeights; mBoneWeights= nullptr;

    mBoundingSphere = BoundingSphere();
  }

  GeometryData::GeometryData()
    : mPositions(nullptr)
    , mColors(nullptr)
    , mTexCoords(nullptr)
    , mNormals(nullptr)
    , mBoneIds(nullptr)
    , mBoneWeights(nullptr)
  {
    Clear();
  }

  template<typename T>
  struct Add
  {
    Add(T * data) : mData(data), mIndex(-1){}
    Add &Push(const T & point)
    {
      mData[++mIndex] = point;
      return *this;
    }
    int mIndex;
    T * mData;
  };
  typedef Add<Vector2> Add2;
  typedef Add<Vector3> Add3;


  TacDLLAPI void GeometryData::LoadCube( float halfWidth)
  {
    Clear();
    /*
    *
    *    p6_________p7        z
    *   /|         /|         ^   y
    *p2/__________/p3         | ^
    * |  |        | |         |/
    * |  |        | |         +--> x
    * |  p4_______|_|p5
    * | /         | /
    * |/__________|/
    * p0          p1 
    *
    */

    // i could make it a unit cube, then scale it by halfwidth...
    Vector3 p0(-halfWidth, -halfWidth, -halfWidth);
    Vector3 p1( halfWidth, -halfWidth, -halfWidth);
    Vector3 p2(-halfWidth, -halfWidth,  halfWidth);
    Vector3 p3( halfWidth, -halfWidth,  halfWidth);
    Vector3 p4(-halfWidth,  halfWidth, -halfWidth);
    Vector3 p5( halfWidth,  halfWidth, -halfWidth);
    Vector3 p6(-halfWidth,  halfWidth,  halfWidth);
    Vector3 p7( halfWidth,  halfWidth,  halfWidth);
    ;
    mPositions = new Vector3[mCounts[Geometry::Attribute::Position] = 36];
    Add<Vector3> a(mPositions);
    // front
    a.Push(p0).Push(p1).Push(p2).Push(p2).Push(p1).Push(p3);
    // right
    a.Push(p1).Push(p5).Push(p3).Push(p3).Push(p5).Push(p7);
    // top 
    a.Push(p2).Push(p3).Push(p6).Push(p3).Push(p7).Push(p6);
    // left
    a.Push(p0).Push(p2).Push(p4).Push(p2).Push(p6).Push(p4);
    // bottom
    a.Push(p1).Push(p0).Push(p4).Push(p5).Push(p1).Push(p4);
    // back
    a.Push(p6).Push(p7).Push(p5).Push(p6).Push(p5).Push(p4);
    ;
    mNormals = new Vector3[mCounts[Geometry::Attribute::Normal] = 36];
    Vector3 n0(0,-1,0);
    Vector3 n1(1,0,0);
    Vector3 n2(0,0,1);
    Vector3 n3(-1,0,0);
    Vector3 n4(0,0,-1);
    Vector3 n5(0,1,0);
    Add<Vector3> b(mNormals);
    b.Push(n0).Push(n0).Push(n0).Push(n0).Push(n0).Push(n0);
    b.Push(n1).Push(n1).Push(n1).Push(n1).Push(n1).Push(n1);
    b.Push(n2).Push(n2).Push(n2).Push(n2).Push(n2).Push(n2);
    b.Push(n3).Push(n3).Push(n3).Push(n3).Push(n3).Push(n3);
    b.Push(n4).Push(n4).Push(n4).Push(n4).Push(n4).Push(n4);
    b.Push(n5).Push(n5).Push(n5).Push(n5).Push(n5).Push(n5);
  }

  TacDLLAPI void GeometryData::LoadTriange( float length )
  {
    Vector3 p0( length, 0, 0);
    Vector3 p1( 0,0,length);
    Vector3 p2( -length,0,0);
    
    mPositions = new Vector3[mCounts[Geometry::Attribute::Position] = 3];
    Add<Vector3> a(mPositions);
    a.Push(p0).Push(p1).Push(p2);

    ;
    mNormals = new Vector3[mCounts[Geometry::Attribute::Normal] = 3];
    Vector3 n0(0,-1,0);
    Add<Vector3> b(mNormals);
    b.Push(n0).Push(n0).Push(n0);
  }

  TacDLLAPI GeometryData & GeometryData::LoadNDCQuad()
  {
    /*
    2__3
    |\ |
    |_\|
    0  1
    */
    Vector3 p0(-1,-1, 0);
    Vector3 p1( 1,-1, 0);
    Vector3 p2(-1, 1, 0);
    Vector3 p3( 1, 1, 0);
    mPositions = new Vector3[mCounts[Geometry::Attribute::Position] = 6];
    Add<Vector3> a(mPositions);
    a.Push(p0).Push(p1).Push(p2);
    a.Push(p1).Push(p3).Push(p2);
    
    mNormals = new Vector3[mCounts[Geometry::Attribute::Normal] = 6];
    Vector3 n0 (0,-1,0);
    Add<Vector3> b(mNormals);
    b.Push(n0).Push(n0).Push(n0);
    b.Push(n0).Push(n0).Push(n0);

    Vector2 t0(0, 0);
    Vector2 t1(1, 0);
    Vector2 t2(0, 1);
    Vector2 t3(1, 1);
    mTexCoords = new Vector2[mCounts[Geometry::Attribute::TexCoord] = 6];
    Add<Vector2> c(mTexCoords);
    c.Push(t0).Push(t1).Push(t2);
    c.Push(t1).Push(t3).Push(t2);
    return *this;
  }

} // Tac

