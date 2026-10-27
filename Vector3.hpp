#ifndef VECTOR3_4620
#define VECTOR3_4620

#include<iostream>
#include<cmath>

struct Vector3
{
public:
  float x, y, z;

  Vector3 operator+=(const Vector3 &rhs);

  Vector3 operator-=(const Vector3 &rhs);

  Vector3 operator*=(const float rhs);

  float dot(const Vector3 &rhs);

  Vector3 cross(const Vector3 &rhs);

  float length();

  Vector3 normalize();
};

Vector3 operator+(Vector3 lhs, const Vector3 &rhs);

Vector3 operator-(Vector3 lhs, const Vector3 &rhs);

Vector3 operator*(Vector3 lhs, const float rhs);

/// Uncomment these lines if you happen to add a Vector3 and want to debug :)
std::ostream &operator<<(std::ostream &os, const Vector3 &v);

#endif