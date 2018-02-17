/*
\author Nathan Park
*/
#pragma once
#include "TacVector3.h"
#include "TacMatrix4.h"

namespace Tac
{
  class Camera
  {
    public:
      Camera();

      Matrix4 GetViewMatrix();
      Matrix4 GetInverseViewMatrix();
      Matrix4 GetPerspectiveProjMatrix();
      Matrix4 GetOrthographicProjMatrix();

      void GetCamDirections(Vector3 & right, Vector3 & up) const;

      float mNear;
      float mFar;
      float mFieldOfViewYRad;
      float mAspectRatio;
      float mWidth;
      float mHeight;
    
      Vector3 mPosition;
      Vector3 mWorldSpaceUp;
      Vector3 mViewDirection;
  };
}
