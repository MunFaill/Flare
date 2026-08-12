#pragma once

#include "Renderer/Device/DeviceBuffers.h"

#include <cstdint>

class GLVertexBuffer : public VertexBuffer {
    public:
        GLVertexBuffer();
        ~GLVertexBuffer() override;

        void Bind() override;
        void Unbind() override;
        void SendData(void* Data, uint32_t Size) override;
        inline uint32_t GetSize() override {return m_Size;}
    private:
        uint32_t VBO = 0;
        uint32_t m_Size = 0;
};

class GLIndexBuffer : public IndexBuffer {
    public:
        GLIndexBuffer();
        ~GLIndexBuffer() override;

        void Bind() override;
        void Unbind() override;
        void SendData(uint32_t* Data, uint32_t Count) override;
        uint32_t GetCount() override {return m_Count;}
    private:
        uint32_t EBO;
        uint32_t m_Count;
};
