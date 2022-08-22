#include <iostream>
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture(GLenum TextureTarget, const char* FileName)
{
    m_textureTarget = TextureTarget;
    data = stbi_load(FileName, &width, &height, &cnt, 0);
}

bool Texture::Load()
{
    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}
Texture::~Texture() {
    stbi_image_free(data);
}
void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
