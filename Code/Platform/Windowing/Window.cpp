#include "Platform/Windowing/Window.h"

#include <GLFW/glfw3.h>
#include <print>

void Window::Init() {
    Handle = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
    if (!Handle) {
        std::println("Error creating window, null handle");
    } else {
        std::println("Window successfully created");
    }
}

void Window::Shutdown() {
    std::println("Window shutdown");
    glfwDestroyWindow(Handle);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(Handle);
    glfwPollEvents();
}

bool Window::CloseEvent() {
    if (glfwWindowShouldClose(Handle)) return true;
    return false;
}
