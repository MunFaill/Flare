#pragma once

#include <cstdint>
#include <memory>
#include <string>

struct Window {
    virtual ~Window() = default;
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void SwapBuffers() = 0;
    virtual void Resizable(bool state) = 0;
    virtual void VSync(bool state) = 0;
    virtual void* GetHandle() = 0;
    virtual bool CloseEvent() = 0;
    std::string Title = "Untitled";
    uint16_t Width = 1280, Height = 720;

    static std::unique_ptr<Window> Create();
};