#include <GL/freeglut.h>
#include "camera.h"
#include <iostream>
const static float STEP_SCALE = 0.1f;
const static int MARGIN = 10;

void v_out(vec3 v) {
    std::cout << "|" << v.x << "|" << v.y << "|" << v.z << "|" << std::endl;
}
void v_out(const char* mes,vec3 v) {
    std::cout << mes<<"|" << v.x << "|" << v.y << "|" << v.z << "|" << std::endl;
}
void mes_out(const char* mes) {
    std::cout << mes << std::endl;
}
void q_out(const char* mes, quat v) {
    std::cout << mes << "|" << v.x << "|" << v.y << "|" << v.z << "|" << v.w << "|" << std::endl;
}
void vrotate(vec3& Vrotated, float Angle, const vec3& Axe)
{
    const float SinHalfAngle = sinf(ToRadian(Angle / 2));
    const float CosHalfAngle = cosf(ToRadian(Angle / 2));

    const float Rx = Axe.x * SinHalfAngle;
    const float Ry = Axe.y * SinHalfAngle;
    const float Rz = Axe.z * SinHalfAngle;
    const float Rw = CosHalfAngle;
    quat RotationQ(Rw, Rx, Ry, Rz);
    quat ConjugateQ = conjugate(RotationQ);
    //  ConjugateQ.Normalize();
    quat W = RotationQ * Vrotated * ConjugateQ;
    Vrotated.x = W.x;
    Vrotated.y = W.y;
    Vrotated.z = W.z;

}
Camera::Camera()
{
    m_pos = vec3(0.0f, 0.0f, 0.0f);
    m_target = vec3(0.0f, 0.0f, 1.0f);
    m_up = vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(int WindowWidth, int WindowHeight)
{
    m_windowWidth = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = vec3(0.0f, 0.0f, 0.0f);
    m_target = vec3(0.0f, 0.0f, 1.0f);
    m_target = normalize(m_target);
    m_up = vec3(0.0f, 1.0f, 0.0f);

    Init();
}


Camera::Camera(int WindowWidth, int WindowHeight, const vec3& Pos, const vec3& Target, const vec3& Up)
{
    m_windowWidth = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = Pos;

    m_target = Target;
    m_target= normalize(m_target);

    m_up = Up;
    m_up = normalize(m_up);

    Init();
}
void Camera::Init()
{
    vec3 HTarget(m_target.x, 0.0, m_target.z);
    HTarget = normalize(HTarget);

    if (HTarget.z >= 0.0f) {
        if (HTarget.x >= 0.0f) {
            m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
        }
        else {
            m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
        }
    }
    else {
        if (HTarget.x >= 0.0f) {
            m_AngleH = ToDegree(asin(-HTarget.z));
        }
        else {
            m_AngleH = 90.0f + ToDegree(asin(-HTarget.z));
        }
    }

    m_AngleV = -ToDegree(asin(m_target.y));

    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge = false;
    m_OnRightEdge = false;
    m_mousePos.x = m_windowWidth / 2;
    m_mousePos.y = m_windowHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
}
void Camera::OnMouse(int x, int y)
{
    const int DeltaX = x - m_mousePos.x;
    const int DeltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_AngleH += (float)DeltaX  / 20.0f;
    m_AngleV += (float)DeltaY  / 20.0f;

    if (DeltaX == 0) {
        if (x <= MARGIN) {
            m_OnLeftEdge = true;
        }
        else if (x >= (m_windowWidth - MARGIN)) {
            m_OnRightEdge = true;
        }
    }
    else {
        m_OnLeftEdge = false;
        m_OnRightEdge = false;
    }

    if (DeltaY == 0) {
        if (y <= MARGIN) {
            m_OnUpperEdge = true;
        }
        else if (y >= (m_windowHeight - MARGIN)) {
            m_OnLowerEdge = true;
        }
    }
    else {
        m_OnUpperEdge = false;
        m_OnLowerEdge = false;
    }

    Update();
}
void Camera::OnRender()
{
    bool ShouldUpdate = false;

    if (m_OnLeftEdge) {
        m_AngleH -= 0.1f;
        ShouldUpdate = true;
    }
    else if (m_OnRightEdge) {
        m_AngleH += 0.1f;
        ShouldUpdate = true;
    }

    if (m_OnUpperEdge) {
        if (m_AngleV > -90.0f) {
            m_AngleV -= 0.1f;
            ShouldUpdate = true;
        }
    }
    else if (m_OnLowerEdge) {
        if (m_AngleV < 90.0f) {
            m_AngleV += 0.1f;
            ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}
void Camera::Update()
{
    const vec3 Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    vec3 View(1.0f, 0.0f, 0.0f);
    vrotate(View,m_AngleH, Vaxis);
    View = normalize(View);
   
    // Rotate the view vector by the vertical angle around the horizontal axis
    vec3 Haxis = cross(Vaxis,View);
    Haxis = normalize(Haxis);
    vrotate(View,m_AngleV, Haxis);
    View = normalize(View);
    m_target = View;
    m_target = normalize(m_target);
    m_up = cross(m_target,Haxis);
    m_up = normalize(m_up);
    }
const vec3& Camera::GetPos() const
{
    return m_pos;
}

const vec3& Camera::GetTarget() const
{
    return m_target;
}

const vec3& Camera::GetUp() const
{
    return m_up;
}


bool Camera::OnKeyboard(int Key)
{
    bool Ret = false;

    switch (Key) {

    case GLUT_KEY_UP:
    {
        m_pos += (m_target * STEP_SCALE);
        Ret = true;
    }
    break;

    case GLUT_KEY_DOWN:
    {
        m_pos -= (m_target * STEP_SCALE);
        Ret = true;
    }
    break;

    case GLUT_KEY_LEFT:
    {
        vec3 Left = cross(m_target,m_up);
        Left = normalize(Left);
        Left *= STEP_SCALE;
        m_pos += Left;
        Ret = true;
    }
    break;

    case GLUT_KEY_RIGHT:
    {
        vec3 Right = cross(m_up,m_target);
        Right = normalize(Right);
        Right *= STEP_SCALE;
        m_pos += Right;
        Ret = true;
    }
    break;
    }

    return Ret;
}