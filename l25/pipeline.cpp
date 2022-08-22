#include "pipeline.h"
#include <iostream>
void Pipeline::InitScaleTransform(mat4& m) const
{
    m[0][0] = m_scale.x; m[0][1] = 0.0f     ; m[0][2] = 0.0f     ; m[0][3] = 0.0f;
    m[1][0] = 0.0f     ; m[1][1] = m_scale.y; m[1][2] = 0.0f     ; m[1][3] = 0.0f;
    m[2][0] = 0.0f     ; m[2][1] = 0.0f     ; m[2][2] = m_scale.z; m[2][3] = 0.0f;
    m[3][0] = 0.0f     ; m[3][1] = 0.0f     ; m[3][2] = 0.0f     ; m[3][3] = 1.0f;
}

void Pipeline::InitRotateTransform(mat4& m) const
{
    mat4 rx, ry, rz;

    const float x = ToRadian(m_rotateInfo.x);
    const float y = ToRadian(m_rotateInfo.y);
    const float z = ToRadian(m_rotateInfo.z);

    rx[0][0] = 1.0f; rx[0][1] = 0.0f   ; rx[0][2] = 0.0f    ; rx[0][3] = 0.0f;
    rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
    rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x) ; rx[2][3] = 0.0f;
    rx[3][0] = 0.0f; rx[3][1] = 0.0f   ; rx[3][2] = 0.0f    ; rx[3][3] = 1.0f;

    ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
    ry[1][0] = 0.0f   ; ry[1][1] = 1.0f; ry[1][2] = 0.0f    ; ry[1][3] = 0.0f;
    ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y) ; ry[2][3] = 0.0f;
    ry[3][0] = 0.0f   ; ry[3][1] = 0.0f; ry[3][2] = 0.0f    ; ry[3][3] = 1.0f;

    rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
    rz[1][0] = sinf(z); rz[1][1] = cosf(z) ; rz[1][2] = 0.0f; rz[1][3] = 0.0f;
    rz[2][0] = 0.0f   ; rz[2][1] = 0.0f    ; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
    rz[3][0] = 0.0f   ; rz[3][1] = 0.0f    ; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

    m = rz * ry * rx;
}
void Pipeline::InitTranslationTransform(mat4& m) const
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = m_worldPos.x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = m_worldPos.y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = m_worldPos.z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}
void Pipeline::InitTranslationTransform(mat4& m, vec3 d) const
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = d.x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = d.y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = d.z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}
void Pipeline::InitIdentity(mat4& m) const 
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

}
void Pipeline::InitPerspectiveProj(mat4& m) const
{
    const float ar = m_persProj.Width / m_persProj.Height;
    const float zNear = m_persProj.zNear;
    const float zFar = m_persProj.zFar;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0));

    m[0][0] = 1.0f / (tanHalfFOV * ar);
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = 1.0f / tanHalfFOV;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = (-zNear - zFar) / zRange;
    m[2][3] = 2.0f * zFar * zNear / zRange;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 1.0f;
    m[3][3] = 0.0f;
}

void Pipeline::InitCameraTransform(mat4& m,const vec3& Target, const vec3& Up)
{
    vec3 N = Target;
    N = normalize(N);
    vec3 U = Up;
    U = normalize(U);
    U = cross(U, N);
    vec3 V = cross(N, U);

    m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
    m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
    m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
}

void Pipeline::InitPersProjTransform(mat4& m)
{
    const float ar = m_persProjInfo.Width / m_persProjInfo.Height;
    const float zRange = m_persProjInfo.zNear - m_persProjInfo.zFar;
    const float tanHalfFOV = tanf(ToRadian(m_persProjInfo.FOV / 2.0f));

    m[0][0] = 1.0f / (tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;          m[0][3] = 0.0;
    m[1][0] = 0.0f;                   m[1][1] = 1.0f / tanHalfFOV; m[1][2] = 0.0f;          m[1][3] = 0.0;
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-m_persProjInfo.zNear - m_persProjInfo.zFar) / zRange; m[2][3] = 2.0f * m_persProjInfo.zFar * m_persProjInfo.zNear / zRange;
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;          m[3][3] = 0.0;
    }
void Pipeline::GetVyvod(mat4 m) {
    std::cout << "===Matrix===" << std::endl;
    std::cout << m[0][0] << m[0][1] << m[0][2] << m[0][3] << std::endl;
    std::cout << m[1][0] << m[1][1] << m[1][2] << m[1][3] << std::endl;
    std::cout << m[2][0] << m[2][1] << m[2][2] << m[2][3] << std::endl;
    std::cout << m[3][0] << m[3][1] << m[3][2] << m[3][3] << std::endl;
    std::cout << "============" << std:: endl;
}
const mat4* Pipeline::GetWorldTrans()
{
    mat4 ScaleTrans, RotateTrans,
        TranslationTrans;
    InitScaleTransform(ScaleTrans);
    InitRotateTransform(RotateTrans);
    InitTranslationTransform(TranslationTrans);

    m_WorldTransformation = ScaleTrans * RotateTrans * TranslationTrans;

    return &m_WorldTransformation;
}
const mat4* Pipeline::GetWVPTrans()
{
    mat4 CameraTranslationTrans,
        CameraRotateTrans;
    mat4 ScaleTrans, RotateTrans, PersProjTrans,
        TranslationTrans;
    InitScaleTransform(ScaleTrans);
    InitRotateTransform(RotateTrans);
    InitTranslationTransform(TranslationTrans);

    InitTranslationTransform(CameraTranslationTrans, vec3(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z));
    //CameraTranslationTrans = transpose(translate(mat4(1), vec3(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z)));
    InitCameraTransform(CameraRotateTrans, m_camera.Target, m_camera.Up);
    InitPerspectiveProj(PersProjTrans);


       
    //m_WVPtransformation = CameraTranslationTrans * CameraRotateTrans * PersProjTrans;
    m_WVPtransformation = ScaleTrans * RotateTrans *TranslationTrans * CameraTranslationTrans * CameraRotateTrans * PersProjTrans;

    return &m_WVPtransformation;
}