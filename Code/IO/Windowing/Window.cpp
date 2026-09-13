#include "IO/Windowing/Window.h"
#include "IO/Windowing/WindowBackend.h"
#include "IO/Windowing/GLFW/GLFWWindow.h"

std::unique_ptr<Window> Window::Create() {
    switch (WindowBackend::CurrentAPI) {
        case WindowAPI::None: return nullptr; break;
        case WindowAPI::GLFW: return std::make_unique<GLFWWindow>(); break;
    }
    return nullptr;
}