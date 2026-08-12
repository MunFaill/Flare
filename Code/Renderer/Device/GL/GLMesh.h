#pragma once

#include "Renderer/Device/DeviceMesh.h"
#include <cstdint>

class GLMesh : public Mesh {
    public:
        GLMesh();
        ~GLMesh() override;

        void Bind() override;
        void Unbind() override;
        void SendData(std::unique_ptr<VertexBuffer> Vertices, std::unique_ptr<IndexBuffer> Indices) override;
        uint32_t GetIndexCount() override {return IndexCount;}
    private:
        uint32_t VAO = 0;
        uint32_t IndexCount = 0;
};
