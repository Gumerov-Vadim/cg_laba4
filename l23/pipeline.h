#ifndef PIPELINE_H
#define	PIPELINE_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include "const.h"
using namespace glm;
class Pipeline
{
public:
    Pipeline()
    {
        m_scale      = vec3(1.0f, 1.0f, 1.0f);
        m_worldPos   = vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }
    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar){
        m_persProj.FOV = FOV;
        m_persProj.Width = Width;
        m_persProj.Height = Height;
        m_persProj.zNear = zNear;
        m_persProj.zFar = zFar;
    }
    void SetCamera(const vec3& Pos, const vec3& Target, const vec3& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }
    const mat4* GetWVPTrans();

    const mat4* GetWorldTrans();


private:
    void InitScaleTransform(mat4& m) const;
    void InitRotateTransform(mat4& m) const;
    void InitTranslationTransform(mat4& m) const;
    void InitTranslationTransform(mat4& m,vec3 d) const;
    void InitIdentity(mat4& m) const;
    void InitPerspectiveProj(mat4& m) const;

    void InitCameraTransform(mat4& m, const vec3& Target, const vec3& Up);
    void InitPersProjTransform(mat4& m, float FOV, float Width, float Height, float zNear, float zFar);
    void GetVyvod(mat4 m);
    vec3 m_scale;
    vec3 m_worldPos;
    vec3 m_rotateInfo;
    struct {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    } m_persProj;

    struct {
        vec3 Pos;
        vec3 Target;
        vec3 Up;
    } m_camera;

    mat4 m_WVPtransformation;
    mat4 m_WorldTransformation;
};
#endif	/* PIPELINE_H */