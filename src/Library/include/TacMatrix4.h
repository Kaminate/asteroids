/*
Nathan Park
*/
#pragma once
#include <iostream> // output operator
#include "TacVector4.h"

namespace Tac
{
  // this matrix can be thought of as an array of 4 row vectors.
  // Since the default OpenGL matrix is column major, when you pass the matrix
  // ie: using glUniformMatrix4f(), pass GL_TRUE to the transpose argument
  struct Vector3;
  struct Matrix4
  {
    union
    {
      struct  
      {
        float 
          m00, m01, m02, m03,
          m10, m11, m12, m13,
          m20, m21, m22, m23,
          m30, m31, m32, m33;
      };

      float m[4][4];
      float v[16];
    };
    Matrix4(
      float mm00 = 1, float mm01 = 0, float mm02 = 0, float mm03 = 0,
      float mm10 = 0, float mm11 = 1, float mm12 = 0, float mm13 = 0,
      float mm20 = 0, float mm21 = 0, float mm22 = 1, float mm23 = 0,
      float mm30 = 0, float mm31 = 0, float mm32 = 0, float mm33 = 1);

    Vector4 operator*(const Vector4& rhs) const;

    Matrix4 operator+(const Matrix4& rhs) const;
    Matrix4 operator*(const Matrix4& rhs) const;

    Matrix4& operator*=(const Matrix4& rhs);

    float * operator [](unsigned row);
    const float * operator [] (unsigned row)const;

    void Zero(void);
    void Identity(void);
    void Transpose(void);
    static Matrix4 Transform(
      const Vector3 & scale,
      const Vector3 & rotate,
      const Vector3 & translate);

    //Tensor Product
    static Matrix4 Tensor(const Vector4& vec);
    //Tilda over n
    static Matrix4 Tilda(const Vector4& vec);
    void Invert();
    void InvertAffine();
    // returns the inverse of the projection matrix
    static Matrix4 InvertProjectionMatrix(const Matrix4 & projMatrix);
    //Rotates a vector to an angle
    Matrix4 VectorAlignment(const Vector4& vector,float rads);
  };

  std::ostream & operator << (std::ostream & lhs, const Matrix4 & mat);
}

