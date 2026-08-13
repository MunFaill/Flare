#pragma once

#include <cstdint>

#include "Renderer/Device/DeviceShader.h"

class GLShader : public Shader {
    public:
        GLShader();
        ~GLShader();

        void Bind() override;
        void Unbind() override;
        void SendData(const char* VertexShaderSource, const char* FragmentShaderSource) override;
        void SetInt(const std::string& name, const int& value) override;
        void SetFloat(const std::string& name, const float& value) override;
        void SetVec2(const std::string& name, const glm::vec2& value) override;
        void SetVec3(const std::string& name, const glm::vec3& value) override;
        void SetVec4(const std::string& name, const glm::vec4& value) override;
        void SetMat4(const std::string& name, const glm::mat4& value) override;
	private:
		uint32_t VS = 0;
		uint32_t FS = 0;
		uint32_t SP = 0;
};

