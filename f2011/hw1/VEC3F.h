#ifndef _VEC3F_H_
#define _VEC3F_H_

//////////////////////////////////////////////////////////////////////
// Based on the VEC3F.h class from libgfx by Michael Garland
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

class VEC3F {
public:
  // Standard constructors
  VEC3F(float s = 0) { *this = s; }
  VEC3F(float x, float y, float z) { element[0]=x; element[1]=y; element[2]=z; }

  // Copy constructors & assignment operators
  VEC3F(const VEC3F& v)             { *this = v; }
  VEC3F(const float v[3])           { element[0]=v[0]; element[1]=v[1]; element[2]=v[2]; }
  VEC3F& operator=(const VEC3F& v)  { element[0]=v[0]; element[1]=v[1]; element[2]=v[2];  return *this; }
  VEC3F& operator=(float s)         { element[0]=element[1]=element[2]=s; return *this; }

  // Access methods
  operator       float*()       { return element; }
  operator const float*() const { return element; }

  float& operator[](int i)       { return element[i]; }
  float  operator[](int i) const { return element[i]; }
  operator const float*()        { return element; }

  // Assignment and in-place arithmetic methods
  inline VEC3F& operator+=(const VEC3F& v);
  inline VEC3F& operator-=(const VEC3F& v);
  inline VEC3F& operator*=(float s);
  inline VEC3F& operator/=(float s);

  void normalize() {
    float l = element[0] * element[0] + element[1] * element[1] + element[2] * element[2];
    if( l!=1.0 && l!=0.0 )  *this /= sqrt(l);
  };
  void clear() {
    float zero = 0.0;
    element[0] = zero;
    element[1] = zero;
    element[2] = zero;
  };

  float maxElement() {
    float foundMax = element[0];
    if (element[1] > foundMax) foundMax = element[1];
    if (element[2] > foundMax) foundMax = element[2];
    return foundMax;
  };

private:
  float element[3];

};

inline VEC3F fabs(const VEC3F& v)   
  { return VEC3F(fabs(v[0]), fabs(v[1]), fabs(v[2])); } 

inline VEC3F cross(const VEC3F& u, const VEC3F& v)
  { return VEC3F( u[1]*v[2] - v[1]*u[2], -u[0]*v[2] + v[0]*u[2], u[0]*v[1] - v[0]*u[1] ); }

////////////////////////////////////////////////////////////////////////
// Operator definitions
////////////////////////////////////////////////////////////////////////

inline VEC3F& VEC3F::operator+=(const VEC3F& v)
  { element[0] += v[0];   element[1] += v[1];   element[2] += v[2];  return *this; }

inline VEC3F& VEC3F::operator-=(const VEC3F& v)
  { element[0] -= v[0];   element[1] -= v[1];   element[2] -= v[2];  return *this; }

inline VEC3F& VEC3F::operator*=(float s)
  { element[0] *= s;   element[1] *= s;   element[2] *= s;  return *this; }

inline VEC3F& VEC3F::operator/=(float s)
  { element[0] /= s;   element[1] /= s;   element[2] /= s;  return *this; }

inline VEC3F operator+(const VEC3F &u, const VEC3F& v)
  { return VEC3F(u[0]+v[0], u[1]+v[1], u[2]+v[2]); }

inline VEC3F operator-(const VEC3F &u, const VEC3F& v)
  { return VEC3F(u[0]-v[0], u[1]-v[1], u[2]-v[2]); }

inline VEC3F operator-(const VEC3F &v)
  { return VEC3F(-v[0], -v[1], -v[2]); }

inline VEC3F operator*(float s, const VEC3F &v)
  { return VEC3F(v[0]*s, v[1]*s, v[2]*s); }

inline VEC3F operator*(const VEC3F &v, float s)
  { return s*v; }

inline VEC3F operator/(const VEC3F &v, float s)
  { return VEC3F(v[0]/s, v[1]/s, v[2]/s); }

inline float operator*(const VEC3F &u, const VEC3F& v)
  { return u[0]*v[0] + u[1]*v[1] + u[2]*v[2]; }

inline VEC3F operator^(const VEC3F& u, const VEC3F& v)
  { return cross(u, v); }

inline std::ostream &operator<<(std::ostream &out, const VEC3F& v)
  { return out << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")"; }

inline std::istream &operator>>(std::istream &in, VEC3F& v)
  { return in >> v[0] >> v[1] >> v[2]; }

#endif
