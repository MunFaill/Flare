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
	private:
		uint32_t VS = 0;
		uint32_t FS = 0;
		uint32_t SP = 0;
};

