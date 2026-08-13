#include "Renderer/Device/GL/GLMesh.h"
#include "Renderer/Device/DeviceBuffers.h"

#include <cstddef>
#include <glad/gl.h>
#include <print>

GLMesh::GLMesh() {
    glGenVertexArrays(1, &VAO);
    std::println("Vertex array created: {}", VAO);
}

GLMesh::~GLMesh() {
    std::println("Vertex array deleted: {}", VAO);
    glDeleteVertexArrays(1, &VAO);
}

void GLMesh::Bind() {
    glBindVertexArray(VAO);
}

void GLMesh::Unbind() {
    glBindVertexArray(0);
}

void GLMesh::SendData(std::unique_ptr<VertexBuffer> Vertices, std::unique_ptr<IndexBuffer> Indices) {
    this->Bind();
    Vertices->Bind();
    Indices->Bind();

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // TexCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

    // Color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    IndexCount = Indices->GetCount();

    std::println("Data send to vertex arrat: {}", VAO);

    this->Unbind();
    
    Vertices->Unbind();
}
