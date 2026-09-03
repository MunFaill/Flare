#pragma once

#include <vector>
#include <string>

struct AssetProcessor {
    void Process(const std::vector<std::string>& Files);
    void ProcessImages(const std::string& File);
    void ProcessShaders(const std::string& File);
    void ProcessMeshes(const std::string& File);
};