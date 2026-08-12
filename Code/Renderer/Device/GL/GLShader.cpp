#include "Renderer/Device/GL/GLShader.h"

#include <glad/gl.h>
#include <print>

GLShader::GLShader() {
	VS = glCreateShader(GL_VERTEX_SHADER);
	FS = glCreateShader(GL_FRAGMENT_SHADER);
	SP = glCreateProgram();
}

GLShader::~GLShader() {
	glDeleteProgram(SP);
}

void GLShader::Bind() {
	glUseProgram(SP);
}

void GLShader::Unbind() {
	glUseProgram(0);
}

void GLShader::SendData(const char* VertexShaderSource, const char* FragmentShaderSource) {
	int success;
	char infoLog[512];
	// Vertex shader
	glShaderSource(VS, 1, &VertexShaderSource, nullptr);
	glCompileShader(VS);
	glGetShaderiv(VS, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(VS, 512, nullptr, infoLog);
		std::println("Vertex shader error: {}", infoLog);
	}
	// Fragment shader
	glShaderSource(FS, 1, &FragmentShaderSource, nullptr);
	glCompileShader(FS);
	glGetShaderiv(FS, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(FS, 512, nullptr, infoLog);
		std::println("Fragment shader error: {}", infoLog);
	}
	// Attach both shaders to shader program
	glAttachShader(SP, VS);
	glAttachShader(SP, FS);
	glLinkProgram(SP);
	glGetProgramiv(SP, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(SP, 512, nullptr, infoLog);
		std::println("Shader program error: {}", infoLog);
	}
	glDeleteShader(VS);
	glDeleteShader(FS);
}
