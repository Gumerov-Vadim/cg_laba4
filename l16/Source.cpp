#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <assert.h>
#include <math.h>
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include <iostream>

GLuint VBO;
GLuint IBO;
GLuint gScaleLocation;
GLuint gWVPLocation;
GLuint gSampler;

Texture* pTexture = NULL;
Camera* pGameCamera = NULL;

//#define debug
#define fullscreenmode
#define mypyramid
struct Vertex
{
    vec3 m_pos;
    vec2 m_tex;

    Vertex() {}

    Vertex(vec3 pos, vec2 tex)
    {
        m_pos = pos;
        m_tex = tex;
    }
};

#ifdef debug
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);                                   \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec4 Color;                                                                      \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = Color;                                                              \n\
}";
#else
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
layout(location = 1) in vec2 TexCoord;                                              \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
out vec2 TexCoord0;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
    TexCoord0 = TexCoord;                                                           \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
in vec2 TexCoord0;                                                                  \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
uniform sampler2D gSampler;                                                         \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = texture2D(gSampler, TexCoord0.xy);                                  \n\
}";
#endif
static void RenderSceneCB()
{
    pGameCamera->OnRender();
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;
    
    Scale += 0.1f;

    Pipeline p;
    p.Rotate(0.0f,Scale,0.0f);//p.Rotate(0.0f, Scale, 0.0f);
    p.WorldPos(-3.0f, 0.0f, 0.0f);
    p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

    p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
    //pGameCamera->OnMouse(Scale, 0.0f);
    // std::cout << sinf(Scale) << " " << 0.0f << std::endl;

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

    glUniform1f(gScaleLocation, sinf(Scale));

    glEnableVertexAttribArray(0);
#ifndef debug
    glEnableVertexAttribArray(1);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
#ifndef debug
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
#ifndef debug
    pTexture->Bind(GL_TEXTURE0);
#endif
#ifdef mypyramid
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
#else
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
#endif


    glDisableVertexAttribArray(0);
#ifndef debug
    glDisableVertexAttribArray(1);
#endif
    glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y) {
    pGameCamera->OnKeyboard(Key);
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
    switch (Key) {
    case 'q':
        exit(0);
        break;
    }
}
static void PassiveMouseCB(int x, int y)
{
    pGameCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks() {
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
}

static void CreateVertexBuffer()
{
#ifndef mypyramid
    Vertex Vertices[4] = { Vertex(vec3(-1.0f, -1.0f, 0.5773f), vec2(0.0f, 0.0f)),
                           Vertex(vec3(0.0f, -1.0f, -1.0f),    vec2(0.5f, 0.0f)),
                           Vertex(vec3(1.0f, -1.0f, 0.5773f),  vec2(1.0f, 0.0f)),
                           Vertex(vec3(0.0f, 1.0f, 0.0f),      vec2(0.5f, 1.0f)) };
#else
    Vertex Vertices[5] = {
    Vertex(vec3(-1.0f, -1.0f, 0.0f),vec2(0.0f,0.0f)),
    Vertex(vec3(0.0f, -1.0f, 1.0f),vec2(0.5f,0.0f)),
    Vertex(vec3(1.0f, -1.0f, 0.0f),vec2(0.75f,0.0f)),
    Vertex(vec3(0.0f, -1.0f, -1.0f),vec2(1.0f,0.0f)),
    Vertex(vec3(0.0f, 1.0f, 0.0f),vec2(0.5f,1.0f)) };
#endif
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}
static void CreateIndexBuffer() {

#ifdef mypyramid
    unsigned int Indices[] = {
                           1, 4, 0,
                           2, 4, 1,
                           3, 4, 2,
                           0, 4 ,3,
                           2, 1, 0,
                           0, 3, 2
    };
#else
    unsigned int Indices[] = { 0, 3, 1,
                              1, 3, 2,
                              2, 3, 0,
                              1, 2, 0 };
#endif
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    assert(gScaleLocation != 0xFFFFFFFF);
#ifndef debug
    gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
    assert(gSampler != 0xFFFFFFFF);
#endif
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 05");
    glutGameModeString("1280x1024:32");
    InitializeGlutCallbacks();
#ifdef fullscreenmode
    glutEnterGameMode();
    InitializeGlutCallbacks();
#endif
    pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    CreateVertexBuffer();
    CreateIndexBuffer();

    CompileShaders();
#ifndef debug
    glUniform1i(gSampler, 0);
    pTexture = new Texture(GL_TEXTURE_2D, "test.png");

    if (!pTexture->Load()) {
        return 1;
    }
#endif

    glutMainLoop();

    return 0;
}