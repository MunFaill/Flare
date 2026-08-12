#pragma once

#include <memory>

class Shader {
    public:
        virtual ~Shader() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void SendData(const char* VertexShaderSource, const char* FragmentShaderSource) = 0;

        std::unique_ptr<Shader> Create();
};
