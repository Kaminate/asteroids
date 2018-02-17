/*
Nathan Park
*/

#pragma once

#include <iostream> // output operator
#include "TacVector3.h"
namespace Tac
{
  // this matrix can be thought of as an array of 3 row vectors.
  // Since the default OpenGL matrix is column major, when you pass the matrix
  // ie: using glUniformMatrix3f(), pass GL_TRUE to the transpose argument

  struct Matrix3
  {
    union
    {
      struct  
      {
        float 
          m00, m01, m02,
          m10, m11, m12,
          m20, m21, m22;
      };
      float v[9];
      float m[3][3];
    };
    // identity
    Matrix3(
      float mm00 = 1.0f, float mm01 = 0.0f, float mm02 = 0.0f,
      float mm10 = 0.0f, float mm11 = 1.0f, float mm12 = 0.0f,
      float mm20 = 0.0f, float mm21 = 0.0f, float mm22 = 1.0f);
    Matrix3 operator+(const Matrix3 & rhs) const;
    Vector3 operator*(const Vector3& rhs) const;
    Matrix3 operator*(const Matrix3& rhs) const;
    Matrix3& operator*=(const Matrix3& rhs);

    void Zero(void);
    void Identity(void);
    void Transpose(void);
    void Invert();

    // returns a row vector[3]
    float * operator [](unsigned row);
    const float * operator [](unsigned row) const;

    static Matrix3 RotRadX(const float rotRad);
    static Matrix3 RotRadY(const float rotRad);
    static Matrix3 RotRadZ(const float rotRad);

    //Tensor Product
    Matrix3 Tensor(const Vector3& vec);
    //Tilda over n
    Matrix3 Tilda(const Vector3& vec);
    //Rotates a vector to an angle
    Matrix3 VectorAlignment(const Vector3& vector,float rad);
  };

  std::ostream & operator << (std::ostream & lhs, const Matrix3 & mat);
}

