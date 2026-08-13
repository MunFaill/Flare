#include "Renderer/Device/GL/GLShader.h"
#include "glm/gtc/type_ptr.hpp"

#include <glad/gl.h>
#include <print>

GLShader::GLShader() {
	VS = glCreateShader(GL_VERTEX_SHADER);
	FS = glCreateShader(GL_FRAGMENT_SHADER);
	SP = glCreateProgram();
}

GLShader::~GLShader() {
	if (SP != 0)
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

void GLShader::SetInt(const std::string& name, const int& value) {
	glUniform1i(glGetUniformLocation(SP, name.c_str()), value);
}

void GLShader::SetFloat(const std::string& name, const float& value) {
	glUniform1f(glGetUniformLocation(SP, name.c_str()), value);
}

void GLShader::SetVec2(const std::string& name, const glm::vec2& value) {
	glUniform2fv(glGetUniformLocation(SP, name.c_str()), 1, glm::value_ptr(value));
}

void GLShader::SetVec3(const std::string& name, const glm::vec3& value) {
	glUniform3fv(glGetUniformLocation(SP, name.c_str()), 1, glm::value_ptr(value));
}

void GLShader::SetVec4(const std::string& name, const glm::vec4& value) {
	glUniform4fv(glGetUniformLocation(SP, name.c_str()), 1, glm::value_ptr(value));
}

void GLShader::SetMat4(const std::string& name, const glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(SP, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

