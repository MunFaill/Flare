#include "IO/Windowing/WindowBackend.h"
#include "IO/Windowing/GLFW/GLFWBackend.h"

void WindowBackend::Init() {
    switch (CurrentAPI) {
        case WindowAPI::None: return; break;
        case WindowAPI::GLFW: return GLFWWindowBackend::Init();
    }
}

void WindowBackend::Shutdown() {
    switch (CurrentAPI) {
        case WindowAPI::None: return; break;
        case WindowAPI::GLFW: return GLFWWindowBackend::Shutdown();
    }
}