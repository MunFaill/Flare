#pragma once

enum class WindowAPI {None = -1, GLFW = 0};

struct WindowBackend {
    static void Init();
    static void Shutdown();

    inline static WindowAPI CurrentAPI = WindowAPI::GLFW;
};