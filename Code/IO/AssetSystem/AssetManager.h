#pragma once

#include <memory>
#include <print>
#include <string>
#include <unordered_map>
#include <utility>

template<typename T>
struct AssetManager {
    inline void Add(const std::string& ID, std::unique_ptr<T> asset) {
        m_Map[ID] = std::move(asset);
        std::println("New asset added as id: {}", ID);
    }

    inline T* Get(const std::string& ID) {
        auto it = m_Map.find(ID);
        if (it != m_Map.end()) {
            return it->second.get();
        }
        std::println("Error: Asset not found, id: {}", ID);
        return nullptr;
    }

    inline bool Has(const std::string& ID) {
        return m_Map.contains(ID);
    }

    inline void Clear() {
        m_Map.clear();
    }

    private:
        std::unordered_map<std::string, std::unique_ptr<T>> m_Map;
};