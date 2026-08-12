#include "Renderer/Device/GL/GLBuffers.h"

#include <cstdint>
#include <glad/gl.h>

/* ===================================+
    Vertex Buffer
   ===================================+*/ 

GLVertexBuffer::GLVertexBuffer() {
    glGenBuffers(1, &VBO);
}

GLVertexBuffer::~GLVertexBuffer() {
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
    this->Unbind();
}

/* ===================================+
    Index Buffer
   ===================================+*/ 

GLIndexBuffer::GLIndexBuffer() {
    glGenBuffers(1, &EBO);
}

GLIndexBuffer::~GLIndexBuffer() {
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
    this->Unbind();
}
