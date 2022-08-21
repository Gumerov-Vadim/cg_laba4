#ifndef MATH_3D_H
#define	MATH_3D_H
#include <math.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

#define M_PI 3.1415926535
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)
//
//struct Vector3f
//{
//    float x;
//    float y;
//    float z;
//
//    Vector3f()
//    {
//    }
//
//    Vector3f(float _x, float _y, float _z)
//    {
//        x = _x;
//        y = _y;
//        z = _z;
//    }
//    bool operator ==(const Vector3f& v) {
//        return (x == v.x && y == v.y && z == v.z) ? 1 : 0;
//    }  
//    Vector3f Cross(const Vector3f& v) const;
//
//    Vector3f& Normalize();
//    Vector3f& operator+=(const Vector3f& r)
//    {
//        x += r.x;
//        y += r.y;
//        z += r.z;
//
//        return *this;
//    }
//
//    Vector3f& operator-=(const Vector3f& r)
//    {
//        x -= r.x;
//        y -= r.y;
//        z -= r.z;
//
//        return *this;
//    }
//
//    Vector3f& operator*=(float f)
//    {
//        x *= f;
//        y *= f;
//        z *= f;
//
//        return *this;
//    }
//};
//
//inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
//{
//    Vector3f Ret(l.x - r.x,
//        l.y - r.y,
//        l.z - r.z);
//
//    return Ret;
//}
//
//inline Vector3f operator*(const Vector3f& l, float f)
//{
//    Vector3f Ret(l.x * f,
//        l.y * f,
//        l.z * f);
//
//    return Ret;
//}

class Matrix4f
{
public:
    float m[4][4];

    Matrix4f()
    {
    }


    inline void InitIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& Right) const
    {
        Matrix4f Ret;

        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                    m[i][1] * Right.m[1][j] +
                    m[i][2] * Right.m[2][j] +
                    m[i][3] * Right.m[3][j];
            }
        }

        return Ret;
    }
    void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
    void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
    void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const vec3& Target, const vec3& Up);
    void InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar);

};


#endif	/* MATH_3D_H */