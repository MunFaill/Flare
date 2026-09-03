#pragma once

#include "Platform/IO/AssetSystem/AssetManager.h"

#include <Renderer/Device/Device.h>

struct Assets {
    inline static AssetManager<Shader> Shaders;
    inline static AssetManager<Texture> Textures;
    inline static AssetManager<Mesh> Meshes;
};