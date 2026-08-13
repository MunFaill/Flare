#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include <memory>
#include <string>

class Shader {
    public:
        virtual ~Shader() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void SendData(const char* VertexShaderSource, const char* FragmentShaderSource) = 0;

        virtual void SetInt(const std::string& name, const int& value) = 0;
        virtual void SetFloat(const std::string& name, const float& value) = 0;
        virtual void SetVec2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

        static std::unique_ptr<Shader> Create();
};
