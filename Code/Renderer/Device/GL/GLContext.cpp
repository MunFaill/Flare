#include "Renderer/Device/GL/GLContext.h"
#include "Platform/Windowing/Window.h"

#include <print>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  

void GLContext::Initialize(Window& window) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #if defined (__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #endif
    std::println("GL context configured");

    glfwMakeContextCurrent(window.Handle);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::println("Error to load GLAD");
        window.Shutdown();
    }

    glfwSetFramebufferSizeCallback(window.Handle, framebuffer_size_callback);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::println("GL context initialized");
}

void GLContext::DrawCall(uint32_t IndexCount) {
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
}

void GLContext::DepthTest(bool State) {
    if (State) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}

void GLContext::CullFaces(bool State) {
    if (State) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);
}

void GLContext::Blend(bool State) {
    if (State) glEnable(GL_BLEND);
    else glDisable(GL_BLEND);
}

void GLContext::Clear(glm::vec4 Color) {
    glClearColor(Color.x, Color.y, Color.z, Color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}