/*
Nathan Park
*/

#ifdef TacDLL_Export
#define TacDLLAPI __declspec(dllexport)
#else
#define TacDLLAPI __declspec(dllimport)
#endif

#pragma once

#include <iostream> // ostream

namespace Tac
{
  struct Vector3;
  struct Vector4
  {
    union
    {
      struct 
      {
        float x, y, z, w;
      };
      float v[4];
    };
    TacDLLAPI Vector4(
      float xx = 0.0f, 
      float yy = 0.0f, 
      float zz = 0.0f, 
      float ww = 0.0f);
    TacDLLAPI Vector4(const Vector3 & vec, float ww = 0.0f);
    TacDLLAPI Vector4 operator-(void) const;  
    TacDLLAPI Vector4 operator+(const Vector4& rhs) const;
    TacDLLAPI Vector4 operator-(const Vector4& rhs) const;
    TacDLLAPI Vector4 operator*(const float rhs) const;
    TacDLLAPI Vector4 operator/(const float rhs) const;
    TacDLLAPI Vector4& operator+=(const Vector4& rhs);
    TacDLLAPI Vector4& operator-=(const Vector4& rhs);
    TacDLLAPI Vector4& operator*=(const float rhs);
    TacDLLAPI Vector4& operator/=(const float rhs);
    TacDLLAPI float & operator[](unsigned index);
    TacDLLAPI const float & operator [] (unsigned index) const;
    TacDLLAPI bool operator==(const Vector4& rhs) const;
    TacDLLAPI bool operator!=(const Vector4& rhs) const;
    TacDLLAPI float Dot(const Vector4& rhs) const;
    TacDLLAPI float Length(void) const;
    TacDLLAPI float LengthSq(void) const;
    TacDLLAPI void Normalize(void);
    TacDLLAPI void Zero(void);
  };

  TacDLLAPI std::ostream & operator << (std::ostream & lhs, const Vector4 & vec);
  TacDLLAPI std::istream & operator >> (std::istream & lhs, Vector4 & vec);

}
