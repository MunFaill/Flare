#include "Renderer/Device/GL/GLBuffers.h"

#include <cstdint>
#include <glad/gl.h>
#include <print>

/* ===================================+
    Vertex Buffer
   ===================================+*/ 

GLVertexBuffer::GLVertexBuffer() {
    glGenBuffers(1, &VBO);
    std::println("Vertex buffer created: {}", VBO);
}

GLVertexBuffer::~GLVertexBuffer() {
    std::println("Vertex buffer deleted: {}", VBO);
    glDeleteBuffers(1, &VBO);
}

void GLVertexBuffer::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void GLVertexBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::SendData(void* Data, uint32_t Size) {
    this->Bind();
    glBufferData(GL_ARRAY_BUFFER, Size, Data, GL_STATIC_DRAW);
    m_Size = Size;
    std::println("Data send to vertex buffer: {}", VBO);
    this->Unbind();
}

/* ===================================+
    Index Buffer
   ===================================+*/ 

GLIndexBuffer::GLIndexBuffer() {
    glGenBuffers(1, &EBO);
    std::println("Index buffer created: {}", EBO);
}

GLIndexBuffer::~GLIndexBuffer() {
    std::println("Index buffer deleted: {}", EBO);
    glDeleteBuffers(1, &EBO);
}

void GLIndexBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void GLIndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::SendData(uint32_t* Data, uint32_t Count) {
    this->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(uint32_t), Data, GL_STATIC_DRAW);
    m_Count = Count;
    std::println("Data send to index buffer: {}", EBO);
    this->Unbind();
}

/* ===================================+
    Frame Buffer
   ===================================+*/ 

GLFrameBuffer::GLFrameBuffer() {
    glGenFramebuffers(1, &FBO);
    std::println("Frame buffer object created: {}", FBO);
}

GLFrameBuffer::~GLFrameBuffer() {
    std::println("Frame buffer object deleted: {}", FBO);
    glDeleteFramebuffers(1, &FBO);
}

void GLFrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void GLFrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::AttachTexture(uint32_t& Texture) {
    this->Bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);

    this->Unbind();
}

/* ===================================+
    Render Buffer
   ===================================+*/ 

GLRenderBuffer::GLRenderBuffer() {
    glGenRenderbuffers(1, &RBO);
    std::println("Render buffer created: {}", RBO);
}

GLRenderBuffer::~GLRenderBuffer() {
    std::println("Render buffer deleted: {}", RBO);
    glDeleteRenderbuffers(1, &RBO);
}

void GLRenderBuffer::Bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
}

void GLRenderBuffer::Unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GLRenderBuffer::SendData(uint16_t Width, uint16_t Height) {
    this->Bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
    this->Unbind();
}

void GLRenderBuffer::Attach() {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}