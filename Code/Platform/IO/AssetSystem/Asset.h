#pragma once

#include "Renderer/Device/DeviceMesh.h"
#include "Renderer/Device/DeviceTexture.h"
#include "Renderer/Device/DeviceShader.h"

#include <memory>
#include <print>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template<typename T>
class AssetManager {
    public:
        void Add(const std::string& ID, std::unique_ptr<T> asset) {
            m_Map[ID] = std::move(asset);
            std::println("New asset added as id: {}", ID);
        }

        T* Get(const std::string& ID) {
            auto it = m_Map.find(ID);
            if (it != m_Map.end()) {
                return it->second.get();
            }
            std::println("Error: Asset not found, id: {}", ID);
            return nullptr;
        }

        bool Has(const std::string& ID) {
            return m_Map.contains(ID);
        }

        void Clear() {
            m_Map.clear();
        }
    private:
        std::unordered_map<std::string, std::unique_ptr<T>> m_Map;
};

struct Assets {
    inline static AssetManager<Mesh> Meshes;
    inline static AssetManager<Texture> Textures;
    inline static AssetManager<Shader> Shaders;
};

// AssetProcessor

void AssetProcessor(const std::vector<std::string>& Files);