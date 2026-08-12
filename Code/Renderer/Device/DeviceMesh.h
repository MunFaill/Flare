#pragma once

#include <cstdint>
#include <memory>

class VertexBuffer;
class IndexBuffer;

class Mesh {
    public:
        virtual ~Mesh() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void SendData(std::unique_ptr<VertexBuffer> Vertices, std::unique_ptr<IndexBuffer> Indices) = 0;
        virtual uint32_t GetIndexCount() = 0;

        static std::unique_ptr<Mesh> Create();
};
