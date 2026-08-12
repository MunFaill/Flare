#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <sys/stat.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec4 Color;
};

class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void SendData(void* Data, uint32_t Size) = 0;
        virtual uint32_t GetSize() = 0;

        static std::unique_ptr<VertexBuffer> Create();
};

class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void SendData(uint32_t* Data, uint32_t Count) = 0;
        virtual uint32_t GetCount() = 0;

        static std::unique_ptr<IndexBuffer> Create();
};
