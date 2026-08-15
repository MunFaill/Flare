#include "Platform/Windowing/WindowBackend.h"

#include <GLFW/glfw3.h>
#include <print>

void WindowBackend::Init() {
    std::println("Initializing window backend");
    if (!glfwInit()) {
        std::println("Error: Unable to initialize window backend");
    } else {
        std::println("Window backend initialized with success");
    }
}

void WindowBackend::Shutdown() {
    std::println("Warning: Window backend shutdown");
    glfwTerminate();
}

void WindowBackend::Reset() {
    std::println("Reseting window backend, it might cause crashes");
    Shutdown();
    Init();
}