#include "IO/Windowing/GLFW/GLFWBackend.h"

#include <GLFW/glfw3.h>
#include <print>

void GLFWWindowBackend::Init() {
    std::println("Initializing window backend");
    if (!glfwInit()) {
        std::println("Error: Unable to initialize window backend");
    } else {
        std::println("Window backend initialized with success");
    }
}

void GLFWWindowBackend::Shutdown() {
    std::println("Warning: Window backend shutdown");
    glfwTerminate();
}