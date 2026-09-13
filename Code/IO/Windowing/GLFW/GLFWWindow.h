#pragma once

#include "IO/Windowing/Window.h"

#include <cstdint>
#include <string>

struct GLFWwindow;

struct GLFWWindow : public Window{
    void Init() override;
    void Shutdown() override;
    void SwapBuffers() override;
    void Resizable(bool state) override;
    void VSync(bool state) override;
    void* GetHandle() override;
    bool CloseEvent() override;
    std::string Title = "Untitled";
    uint16_t Width = 1280, Height = 720;
};