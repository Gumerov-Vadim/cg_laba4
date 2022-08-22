#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include "const.h"
using namespace glm;
//class Camera
//{
//public:
//    Camera();
//    Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);
//    bool OnKeyboard(int Key);
//    const Vector3f& GetPos() const;
//    const Vector3f& GetTarget() const;
//    const Vector3f& GetUp() const;
//
//private:
//    Vector3f m_pos;
//    Vector3f m_target;
//    Vector3f m_up;
//};

class Camera
{
public:
    Camera();
    Camera(int WindowWidth, int WindowHeight);
    Camera(int WindowWidth, int WindowHeight,const vec3& Pos);
    Camera(int WindowWidth, int WindowHeight, const vec3& Pos, const vec3& Target, const vec3& Up);
    bool OnKeyboard(int Key);
    const vec3& GetPos() const;
    const vec3& GetTarget() const;
    const vec3& GetUp() const;
    void OnMouse(int x, int y);
    void OnRender();
private:
    vec3 m_pos;
    vec3 m_target;
    vec3 m_up;

    int m_windowWidth;
    int m_windowHeight;

    float m_AngleH;
    float m_AngleV;

    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

    ivec2 m_mousePos;
    void Init();
    void Update();
};
