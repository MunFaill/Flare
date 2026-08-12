#include "Renderer/Device/GL/GLTexture.h"

#include <glad/gl.h>

GLTexture::GLTexture() {
    glGenTextures(1, &TextureObject);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &TextureObject);
}

void GLTexture::Bind(uint32_t Slot) {
    LocalSlot = Slot;
    glBindTexture(GL_TEXTURE_2D, TextureObject);
}

void GLTexture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::SendData(unsigned char* data, uint32_t Width, uint32_t Height) {
    this->Bind(LocalSlot);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    this->Unbind();
}

void GLTexture::Mipmaps(bool state) {
    if (state) {
        this->Bind(LocalSlot);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        this->Unbind();
    }
}
