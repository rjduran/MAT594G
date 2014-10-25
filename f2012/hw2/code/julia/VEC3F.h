#ifndef VEC3F_H
#define VEC3F_H

//////////////////////////////////////////////////////////////////////
// A three element vector for a point, direction, or color
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

class VEC3F {
public:

  // constructors
  VEC3F(float scalar = 0);
  VEC3F(float x, float y, float z);
  VEC3F(const VEC3F& v) { *this = v; }
  VEC3F(const float v[3]);

  // accessors
  operator       float*()        { return data; }
  operator const float*() const  { return data; }
  float& operator[](int i)       { return data[i]; }
  float  operator[](int i) const { return data[i]; }
  operator const float*()        { return data; }

  // overloaded operators
  VEC3F& operator=(const VEC3F& v);
  VEC3F& operator=(const float scalar);
  VEC3F& operator+=(const VEC3F& v);
  VEC3F& operator-=(const VEC3F& v);
  VEC3F& operator*=(const float scalar);
  VEC3F& operator/=(const float scalar);
  VEC3F& operator*=(const VEC3F& v)     { x *= v.x; y *= v.y; z *= v.z; return *this; }

  // support functions
  float magnitude();
  float magnitudeSq();
  void normalize();
  void clear() { (*this) = 0; };
  static VEC3F fabs(const VEC3F& input);

  void read(FILE* file) { fscanf(file, "%f %f %f", &x, &y, &z); };
  double maxVal() { return x > y && x > z ? x : y > z ? y : z; };

  static VEC3F cross(const VEC3F& u, const VEC3F& v) { return VEC3F(u[1]*v[2] - v[1]*u[2], -u[0]*v[2] + v[0]*u[2], u[0]*v[1] - v[0]*u[1] ); };

  // the actual data
  union {
     struct { float x,y,z; };
     struct { float r,g,b; };
     float data[3];
  };
};

VEC3F operator+(const VEC3F& u, const VEC3F& v);
VEC3F operator-(const VEC3F& u, const VEC3F& v);
VEC3F operator-(const VEC3F& v);
float operator*(const VEC3F& u, const VEC3F& v);
VEC3F operator*(const float scalar, const VEC3F& v);
VEC3F operator*(const VEC3F &v, const float scalar);
VEC3F operator/(const VEC3F &v, float scalar);
std::ostream &operator<<(std::ostream &out, const VEC3F& v);

#endif
