#include "Platform/window/Window.h"

#include <GLFW/glfw3.h>
#include <print>

void Window::Init() {
    Handle = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
    if (!Handle) {
        std::println("Error: Window is null");
    } else {
        std::println("Window created");
    }
}

void Window::Update() {
    glfwSwapBuffers(Handle);
    glfwPollEvents();
}

void Window::Shutdown() {
    glfwDestroyWindow(Handle);
}

bool Window::CloseEvent() {
    if (Handle && glfwWindowShouldClose(Handle)) {
        return true;
    }
    return false;
}