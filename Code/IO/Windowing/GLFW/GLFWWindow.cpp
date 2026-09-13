#include "IO/Windowing/GLFW/GLFWWindow.h"

#include <GLFW/glfw3.h>
#include <print>

static GLFWwindow* Handle = nullptr;

void GLFWWindow::Init() {
    Handle = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
    if (!Handle) {
        std::println("Error creating window, null handle");
    } else {
        std::println("Window successfully created");
    }
}

void GLFWWindow::Shutdown() {
    std::println("Window shutdown");
    glfwDestroyWindow(Handle);
}

void GLFWWindow::SwapBuffers() {
    glfwSwapBuffers(Handle);
    glfwPollEvents();
}

void GLFWWindow::Resizable(bool state) {
    if (Handle) {
        if (state) glfwSetWindowAttrib(Handle, GLFW_RESIZABLE, GLFW_TRUE);
        else glfwSetWindowAttrib(Handle, GLFW_RESIZABLE, GLFW_FALSE);
    }
}

void GLFWWindow::VSync(bool state) {
    if (Handle) {
        if (state) glfwSwapInterval(1);
        else glfwSwapInterval(0);
    }
}

void* GLFWWindow::GetHandle() {
    return Handle;
}

bool GLFWWindow::CloseEvent() {
    if (glfwWindowShouldClose(Handle)) return true;
    return false;
}
