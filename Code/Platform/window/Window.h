#pragma once

#include <string>
#include <cstdint>

class GLFWwindow;

struct Window {
    std::string Title = "";
    uint32_t Width = 1360, Height = 768;
    void Init();
    void Update();
    void Shutdown();
    bool CloseEvent();
    GLFWwindow* Handle = nullptr;
};