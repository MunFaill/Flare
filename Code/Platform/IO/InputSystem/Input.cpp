#include "Platform/IO/InputSystem/Input.h"
#include "Platform/IO/InputSystem/Keys.h"
#include "Platform/Windowing/Window.h"

#include <GLFW/glfw3.h>

void Input::Initialize(Window& window) {
    m_WindowHandle = window.Handle;
}

void Input::Update() {
    m_PreviousState = m_CurrentState;

    for (auto& [key, state] : m_CurrentState) {
        if (key >= MOUSE_BUTTON_LEFT && key <= MOUSE_BUTTON_LAST) {
            state = (glfwGetMouseButton(m_WindowHandle, key) == GLFW_PRESS);
        } else {
            state = (glfwGetKey(m_WindowHandle, key) == GLFW_PRESS);
        }
    }
}

void Input::HideCursor(bool Status) {
    if (m_WindowHandle) {
        if (Status) glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    };
}

void Input::SetCursorPosition(float x, float y) {
    if (m_WindowHandle) glfwSetCursorPos(m_WindowHandle, x, y);
}

void Input::GetCursorPosition(double* X, double* Y) {
    if (m_WindowHandle) glfwGetCursorPos(m_WindowHandle, X, Y);
}


bool Input::KeyPressed(Key key) {
    if (!m_WindowHandle) return false;

    bool isPressed = false;
    
    if (key >= MOUSE_BUTTON_LEFT && key <= MOUSE_BUTTON_LAST) {
        isPressed = (glfwGetMouseButton(m_WindowHandle, key) == GLFW_PRESS);
    } else {
        isPressed = (glfwGetKey(m_WindowHandle, key) == GLFW_PRESS);
    }

    m_CurrentState[key] = isPressed;
    return isPressed;
}

bool Input::KeyJustPressed(Key key) {
    bool current = KeyPressed(key);
    
    bool previous = false;
    auto itPrev = m_PreviousState.find(key);
    if (itPrev != m_PreviousState.end()) {
        previous = itPrev->second;
    }

    return current && !previous;
}

bool Input::KeyJustReleased(Key key) {
    bool current = KeyPressed(key);
    
    bool previous = false;
    auto itPrev = m_PreviousState.find(key);
    if (itPrev != m_PreviousState.end()) {
        previous = itPrev->second;
    }

    return current && !previous;
}