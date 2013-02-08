#ifndef COMMON_H
#define COMMON_H

#include <limits>

const double MinDouble = (std::numeric_limits<double>::min)();
const double MaxDouble = (std::numeric_limits<double>::max)();
const double EpsilonDouble = (std::numeric_limits<double>::epsilon)();

const float MinFloat = (std::numeric_limits<float>::min)();
const float MaxFloat = (std::numeric_limits<float>::max)();
const float EpsilonFloat = (std::numeric_limits<float>::epsilon)();

const double Pi = 3.14159;
const double TwoPi = Pi * 2.0;
const double HalfPi = Pi * 0.5;
const double QuarterPi = Pi * 0.25;

inline bool IsZero(double value)
{
  return ((-MinDouble < value) && (value < MinDouble));
}

inline double DegreesToRadians(double degrees)
{
  return degrees * 0.0174532925;
  //return TwoPi * (degrees / 360.0);
}

inline double RadiansToDegrees(double radians)
{
  return radians * 57.2957795;
  //return TwoPi * (degrees / 360.0);
}

#endif