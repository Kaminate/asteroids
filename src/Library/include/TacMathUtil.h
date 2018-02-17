// Nathan Park
#pragma once

namespace Tac
{
	const float EPSILON = 0.0001f;
  const float PI = 3.14159f;
  const float TWO_PI = 6.283185307f;

  template<typename T>
  void Clamp( T & val, const T & mini, const T & maxi )
  {
    if (val < mini) 
      val = mini;
    else if (val > maxi )
      val = maxi;
  }

  bool IsAboutZero(float f);
  bool IsAboutOne(float f);
  float ToRadians(float deg);
  float ToDegrees(float rad);
  float RandFloat(float mini = 0, float maxi = 1);
  int RandInt(int mini, int maxi);

  template <typename T>
  T Lerp(const T & begin, const T & end, float percent)
  {
    return begin * (1 - percent) + end * percent;
  }

}

