/*
Nathan Park
*/
#include "TacVector4.h"
#include "TacVector3.h"
#include "TacMathUtil.h"
#include <iomanip>
namespace Tac
{
  Vector4::Vector4(float xx, float yy, float zz, float ww) 
    : x(xx),y(yy),z(zz),w(ww)
  {
  }

  Vector4::Vector4( const Vector3 & vec, float ww /*= 0.0f*/ )
    : x(vec.x), y(vec.y), z(vec.z), w(ww)
  {

  }

  Vector4 Vector4::operator-(void) const
  {
    return Vector4 (
      -x,
      -y,
      -z,
      -w);
  }

  Vector4 Vector4::operator+(const Vector4& rhs) const
  {
    return Vector4 (
      x + rhs.x,
      y + rhs.y,
      z + rhs.z,
      w + rhs.w);
  }
  Vector4 Vector4::operator-(const Vector4& rhs) const
  {
    return Vector4 (
      x - rhs.x,
      y - rhs.y,
      z - rhs.z,
      w - rhs.w);
  }
  Vector4 Vector4::operator*(const float rhs) const
  {
    return Vector4 (
      x * rhs,
      y * rhs,
      z * rhs,
      w * rhs);
  }

  Vector4 Vector4::operator/(const float rhs) const
  {
    return Vector4(
      x / rhs,
      y / rhs,
      z / rhs,
      w / rhs);
  }
  Vector4& Vector4::operator+=(const Vector4& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
  }
  Vector4& Vector4::operator-=(const Vector4& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
  }
  Vector4& Vector4::operator*=(const float rhs)
  {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
  }
  Vector4& Vector4::operator/=(const float rhs)
  {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
  }
  bool Vector4::operator==(const Vector4& rhs) const
  {
    if (abs(x - rhs.x) > EPSILON)
      return false;
    if (abs(y - rhs.y) > EPSILON)
      return false;
    if (abs(z - rhs.z) > EPSILON)
      return false;
    if (abs(w - rhs.w) > EPSILON)
      return false;
    return true;
  }
  bool Vector4::operator!=(const Vector4& rhs) const
  {
    return !(*this == rhs);
  }
  float Vector4::Dot(const Vector4& rhs) const
  {
    return 
    x * rhs.x+
    y * rhs.y+
    z * rhs.z+
    w * rhs.w;
  }
  float Vector4::Length(void) const
  {
    return sqrt(x * x + y * y + z * z + w * w); 
  }
  float Vector4::LengthSq(void) const
  {
    return x * x + y * y + z * z + w * w; 
  }

  void Vector4::Normalize(void)
  {
    *this /= Length();
  }
  void Vector4::Zero(void)
  {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
  }

  float & Vector4::operator[]( unsigned index )
  {
    return v[index];
  }

  const float & Vector4::operator[]( unsigned index ) const
  {
    return v[index];
  }

  std::ostream & operator<<( std::ostream & lhs, const Vector4 & vec )
  {
    return lhs
      << vec.x << " " 
      << vec.y << " "
      << vec.z << " "
      << vec.w;
  }
  std::istream & operator >> (std::istream & lhs, Vector4 & vec)
  {
    return lhs >> vec.x >> vec.y >> vec.z >> vec.w;
  }
} // Tac
