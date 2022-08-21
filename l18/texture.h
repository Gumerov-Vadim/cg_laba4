#ifndef TEXTURE_H
#define	TEXTURE_H


#include <GL/glew.h>

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


#endif	/* TEXTURE_H */