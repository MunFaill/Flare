#include "Platform/window/WindowBackend.h"

#include <GLFW/glfw3.h>
#include <print>

void WindowBackend::Init() {
    if(!glfwInit()) {
        std::println("Error: GLFW could not be initialized");
    } else {
        std::println("GLFW initialized");
    }
}

void WindowBackend::Shutdown() {
    glfwTerminate();
    std::println("Warning: GLFW shutdown");
}