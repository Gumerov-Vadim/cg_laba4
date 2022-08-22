#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>
#include <GL/glew.h>

using namespace std;
class Texture
{
public:
    Texture(GLenum TextureTarget, const char* FileName);
    ~Texture();
    bool Load();

    void Bind(GLenum TextureUnit);

private:
    GLenum m_textureTarget;
    GLuint m_textureObj;
    int width, height, cnt;
    unsigned char* data;
};


class CubemapTexture
{
public:

    CubemapTexture(const string& Directory,
        const string& PosXFilename,
        const string& NegXFilename,
        const string& PosYFilename,
        const string& NegYFilename,
        const string& PosZFilename,
        const string& NegZFilename);

    ~CubemapTexture();

    bool Load();

    void Bind(GLenum TextureUnit);

private:
    string m_fileNames[6];
    GLuint m_textureObj;
    int width[6], height[6], cnt[6];
    unsigned char* data[6];
};


#endif	/* TEXTURE_H */