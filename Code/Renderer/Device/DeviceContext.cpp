#include <Renderer/Device/Device.h>
#include <memory>

#include "Renderer/Device/GL/GLBuffers.h"
#include "Renderer/Device/GL/GLContext.h"
#include "Renderer/Device/GL/GLMesh.h"
#include "Renderer/Device/GL/GLShader.h"
#include "Renderer/Device/GL/GLTexture.h"

static Devices CurrentDevice = Devices::OpenGL;

// Context
std::unique_ptr<DeviceContext> DeviceContext::Create() {
    switch (CurrentDevice) {
        case Devices::None: return nullptr; break;
        case Devices::OpenGL: return std::make_unique<GLContext>(); break;
        case Devices::Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Vertex Buffer
std::unique_ptr<VertexBuffer> VertexBuffer::Create() {
    switch (CurrentDevice) {
        case Devices::None: return nullptr; break;
        case Devices::OpenGL: return std::make_unique<GLVertexBuffer>(); break;
        case Devices::Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Index Buffer
std::unique_ptr<IndexBuffer> IndexBuffer::Create() {
    switch (CurrentDevice) {
        case Devices::None: return nullptr; break;
        case Devices::OpenGL: return std::make_unique<GLIndexBuffer>(); break;
        case Devices::Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Frame Buffer
std::unique_ptr<FrameBuffer> FrameBuffer::Create() {
    switch (CurrentDevice) {
        case Devices::None: return nullptr; break;
        case Devices::OpenGL: return std::make_unique<GLFrameBuffer>(); break;
        case Devices::Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Render Buffer
std::unique_ptr<RenderBuffer> RenderBuffer::Create() {
    switch (CurrentDevice) {
        case Devices::None: return nullptr; break;
        case Devices::OpenGL: return std::make_unique<GLRenderBuffer>(); break;
        case Devices::Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Shader
std::unique_ptr<Shader> Shader::Create() {
    switch (CurrentDevice) {
        case Devices::None: return nullptr; break;
        case Devices::OpenGL: return std::make_unique<GLShader>(); break;
        case Devices::Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Mesh
std::unique_ptr<Mesh> Mesh::Create() {
    switch (CurrentDevice) {
        case Devices::None: return nullptr; break;
        case Devices::OpenGL: return std::make_unique<GLMesh>(); break;
        case Devices::Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Texture
std::unique_ptr<Texture> Texture::Create() {
    switch (CurrentDevice) {
        case Devices::None: return nullptr; break;
        case Devices::OpenGL: return std::make_unique<GLTexture>(); break;
        case Devices::Vulkan: return nullptr; break;
    }
    return nullptr;
}

bool DeviceContext::IsNone() {
    if (CurrentDevice == Devices::None) return true;
    return false;
}

bool DeviceContext::IsOpenGL() {
    if (CurrentDevice == Devices::OpenGL) return true;
    return false;
}

bool DeviceContext::IsVulkan() {
    if (CurrentDevice == Devices::Vulkan) return true;
    return false;
}