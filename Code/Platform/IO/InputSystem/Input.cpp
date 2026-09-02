#include "Platform/IO/InputSystem/Input.h"

void Input::Update(GLFWwindow* window) {
    m_WindowHandle = window;

    m_PreviousState = m_CurrentState;

    for (auto& [key, state] : m_CurrentState) {
        state = (glfwGetKey(m_WindowHandle, key) == GLFW_PRESS);
    }
}

bool Input::KeyPressed(Key key) {
    if (!m_WindowHandle) return false;

    if (m_CurrentState.find(key) == m_CurrentState.end()) {
        m_CurrentState[key] = (glfwGetKey(m_WindowHandle, key) == GLFW_PRESS);
    }

    return m_CurrentState[key];
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

    return !current && previous;
}