#pragma once

#include "Platform/IO/InputSystem/Keys.h"

#include <unordered_map>
#include <GLFW/glfw3.h>

class Input {
public:
    void Update(GLFWwindow* window);

    bool KeyPressed(Key key);
    bool KeyJustPressed(Key key);
    bool KeyJustReleased(Key key);

private:
    GLFWwindow* m_WindowHandle = nullptr;
    std::unordered_map<Key, bool> m_CurrentState;
    std::unordered_map<Key, bool> m_PreviousState;
};