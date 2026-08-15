#pragma once

#include <cstdint>
#include <string>

struct GLFWwindow;

struct Window {
    void Init();
    void Shutdown();
    void SwapBuffers();
    bool CloseEvent();
    std::string Title = "Untitled";
    uint16_t Width = 1280, Height = 720;
    GLFWwindow* Handle = nullptr;
};