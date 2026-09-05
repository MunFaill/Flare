#pragma once

#include "Platform/IO/InputSystem/Keys.h"

#include <unordered_map>

class GLFWwindow;
class Window;

class Input {
    public:
        void Initialize(Window& window);
        void Update();
        void HideCursor(bool Status);
        void SetCursorPosition(float x, float y);
        void GetCursorPosition(double* X, double* Y);

        bool KeyPressed(Key key);
        bool KeyJustPressed(Key key);
        bool KeyJustReleased(Key key);

    private:
        GLFWwindow* m_WindowHandle = nullptr;
        std::unordered_map<Key, bool> m_CurrentState;
        std::unordered_map<Key, bool> m_PreviousState;
};