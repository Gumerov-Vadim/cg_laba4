#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"

//#define debug
#define fullscreenmode
//#define mypyramid
struct Vertex
{
    vec3 m_pos;
    vec2 m_tex;
    vec3 m_normal;

    Vertex() {}

    Vertex(vec3 pos, vec2 tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = vec3(0.0f, 0.0f, 0.0f);
    }
};

class Main : public ICallbacks
{
public:

    Main()
    {
        m_pGameCamera = NULL;
        m_pTexture = NULL;
        m_pEffect = NULL;
        m_scale = 0.0f;
        m_directionalLight.Color = vec3(1.0f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = 0.5f;
        m_directionalLight.DiffuseIntensity = 0.75f;
        m_directionalLight.Direction = vec3(1.0f, 0.0, 0.0);
    }

    ~Main()
    {
        delete m_pEffect;
        delete m_pGameCamera;
        delete m_pTexture;
    }

    bool Init()
    {
        vec3 Pos(3.0f, 0.0f, 0.0f);
        vec3 Target(0.0f, 0.0f, 1.0f);
        vec3 Up(0.0, 1.0f, 0.0f);
        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

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
        unsigned int Indices[] = { 0, 2, 1,
                                   0, 3, 2 };
#endif
        CreateIndexBuffer(Indices, sizeof(Indices));

        CreateVertexBuffer(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices));

        m_pEffect = new LightingTechnique();

        if (!m_pEffect->Init())
        {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pEffect->Enable();

        m_pEffect->SetTextureUnit(0);

        m_pTexture = new Texture(GL_TEXTURE_2D, "test.png");

        if (!m_pTexture->Load()) {
            return false;
        }

        return true;
    }

    void Run()
    {
        GLUTBackendRun(this);
    }

    virtual void RenderSceneCB()
    {
        m_pGameCamera->OnRender();

        glClear(GL_COLOR_BUFFER_BIT);

        m_scale += 0.1f;

        SpotLight sl[2];
        sl[0].DiffuseIntensity = 15.0f;
        sl[0].Color = vec3(1.0f, 1.0f, 0.7f);
        sl[0].Position = vec3(-0.0f, -1.9f, -0.0f);
        sl[0].Direction = vec3(sinf(m_scale), 0.0f, cosf(m_scale));
        sl[0].Attenuation.Linear = 0.1f;
        sl[0].Cutoff = 20.0f;

        sl[1].DiffuseIntensity = 5.0f;
        sl[1].Color = vec3(0.0f, 1.0f, 1.0f);
        sl[1].Position = m_pGameCamera->GetPos();
        sl[1].Direction = m_pGameCamera->GetTarget();
        sl[1].Attenuation.Linear = 0.1f;
        sl[1].Cutoff = 10.0f;

        m_pEffect->SetSpotLights(2, sl);


        Pipeline p;
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 0.0f);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
        m_pEffect->SetWVP(p.GetWVPTrans());
        const mat4 *WorldTransformation = p.GetWorldTrans();
        m_pEffect->SetWorldMatrix(WorldTransformation);
        m_pEffect->SetDirectionalLight(m_directionalLight);

        m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
        m_pEffect->SetMatSpecularIntensity(1.0f);
        m_pEffect->SetMatSpecularPower(32);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        m_pTexture->Bind(GL_TEXTURE0);
#ifndef mypyramid
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#else
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
#endif
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glutSwapBuffers();
    }

    virtual void IdleCB()
    {
        RenderSceneCB();
    }

    virtual void SpecialKeyboardCB(int Key, int x, int y)
    {
        m_pGameCamera->OnKeyboard(Key);
    }


    virtual void KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
        case 'q':
            glutLeaveMainLoop();
            break;

        case 'a':
            m_directionalLight.AmbientIntensity += 0.05f;
            break;

        case 's':
            m_directionalLight.AmbientIntensity -= 0.05f;
            break;
        case 'z':
            m_directionalLight.DiffuseIntensity += 0.05f;
            break;

        case 'x':
            m_directionalLight.DiffuseIntensity -= 0.05f;
            break;
        }
    }


    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

private:
    void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
        Vertex* pVertices, unsigned int VertexCount) {
        for (unsigned int i = 0; i < IndexCount; i += 3) {
            unsigned int Index0 = pIndices[i];
            unsigned int Index1 = pIndices[i + 1];
            unsigned int Index2 = pIndices[i + 2];
            vec3 v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
            vec3 v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
            vec3 Normal = normalize(cross(v1,v2));

            pVertices[Index0].m_normal += Normal;
            pVertices[Index1].m_normal += Normal;
            pVertices[Index2].m_normal += Normal;
        }

        for (unsigned int i = 0; i < VertexCount; i++) {
            pVertices[i].m_normal =  normalize(pVertices[i].m_normal);
        }
    }

    void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
    {
#ifndef mypyramid
   /* Vertex Vertices[4] = { Vertex(vec3(-1.0f, -1.0f, 0.5773f), vec2(0.0f, 0.0f)),
                           Vertex(vec3(0.0f, -1.0f, -1.0f),    vec2(0.5f, 0.0f)),
                           Vertex(vec3(1.0f, -1.0f, 0.5773f),  vec2(1.0f, 0.0f)),
                           Vertex(vec3(0.0f, 1.0f, 0.0f),      vec2(0.5f, 1.0f)) };*/
    Vertex Vertices[4] = { Vertex(vec3(-10.0f, -2.0f, -10.0f), vec2(0.0f, 0.0f)),
                               Vertex(vec3(10.0f, -2.0f, -10.0f), vec2(1.0f, 0.0f)),
                               Vertex(vec3(10.0f, -2.0f, 10.0f), vec2(1.0f, 1.0f)),
                               Vertex(vec3(-10.0f, -2.0f, 10.0f), vec2(0.0f, 1.0f)) };


#else
    Vertex Vertices[5] = {
    Vertex(vec3(-1.0f, -1.0f, 0.0f),vec2(0.0f,0.0f)),
    Vertex(vec3(0.0f, -1.0f, 1.0f),vec2(0.5f,0.0f)),
    Vertex(vec3(1.0f, -1.0f, 0.0f),vec2(0.75f,0.0f)),
    Vertex(vec3(0.0f, -1.0f, -1.0f),vec2(1.0f,0.0f)),
    Vertex(vec3(0.0f, 1.0f, 0.0f),vec2(0.5f,1.0f)) };
#endif        
    unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

    CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}
void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes) {

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
} 
GLuint m_VBO;
GLuint m_IBO;
LightingTechnique* m_pEffect;
Texture* m_pTexture;
Camera* m_pGameCamera;
float m_scale;
DirectionalLight m_directionalLight;
};


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

int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv);
#ifdef fullscreenmode
    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "OpenGL tutors")) {
        return 1;
    }
#endif

    Main* pApp = new Main();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;

    return 0;
}