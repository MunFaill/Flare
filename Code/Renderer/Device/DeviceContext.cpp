#include <Renderer/Device/Device.h>

#include "Renderer/Device/DeviceTexture.h"
#include "Renderer/Device/GL/GLBuffers.h"
#include "Renderer/Device/GL/GLContext.h"
#include "Renderer/Device/GL/GLMesh.h"
#include "Renderer/Device/GL/GLShader.h"
#include "Renderer/Device/GL/GLTexture.h"

static Devices CurrentDevice = OpenGL;

// Context
std::unique_ptr<DeviceContext> DeviceContext::Create() {
    switch (CurrentDevice) {
        case None: return nullptr; break;
        case OpenGL: return std::make_unique<GLContext>(); break;
        case Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Vertex Buffer
std::unique_ptr<VertexBuffer> VertexBuffer::Create() {
    switch (CurrentDevice) {
        case None: return nullptr; break;
        case OpenGL: return std::make_unique<GLVertexBuffer>(); break;
        case Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Index Buffer
std::unique_ptr<IndexBuffer> IndexBuffer::Create() {
    switch (CurrentDevice) {
        case None: return nullptr; break;
        case OpenGL: return std::make_unique<GLIndexBuffer>(); break;
        case Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Shader
std::unique_ptr<Shader> Shader::Create() {
    switch (CurrentDevice) {
        case None: return nullptr; break;
        case OpenGL: return std::make_unique<GLShader>(); break;
        case Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Mesh
std::unique_ptr<Mesh> Mesh::Create() {
    switch (CurrentDevice) {
        case None: return nullptr; break;
        case OpenGL: return std::make_unique<GLMesh>(); break;
        case Vulkan: return nullptr; break;
    }
    return nullptr;
}

// Texture
std::unique_ptr<Texture> Texture::Create() {
    switch (CurrentDevice) {
        case None: return nullptr; break;
        case OpenGL: return std::make_unique<GLTexture>(); break;
        case Vulkan: return nullptr; break;
    }
    return nullptr;
}

bool DeviceContext::IsNone() {
    if (CurrentDevice == None) return true;
    return false;
}

bool DeviceContext::IsOpenGL() {
    if (CurrentDevice == OpenGL) return true;
    return false;
}

bool DeviceContext::IsVulkan() {
    if (CurrentDevice == Vulkan) return true;
    return false;
}