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
