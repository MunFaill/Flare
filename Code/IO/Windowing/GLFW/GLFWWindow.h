#pragma once

#include "IO/Windowing/Window.h"

#include <cstdint>
#include <string>

struct GLFWwindow;

struct GLFWWindow : public Window{
    ~GLFWWindow() override = default;
    void Init() override;
    void Shutdown() override;
    void SwapBuffers() override;
    void Resizable(bool state) override;
    void VSync(bool state) override;
    void* GetHandle() override;
    bool CloseEvent() override;
};