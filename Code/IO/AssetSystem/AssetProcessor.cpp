#include "IO/AssetSystem/AssetProcessor.h"

void AssetProcessor::Process(const std::vector<std::string>& Files) {
    for (const std::string& FilePath : Files) {
        if (FilePath.ends_with(".shader")) {
            ProcessShaders(FilePath);
        }

        else if (FilePath.ends_with(".png") || FilePath.ends_with(".jpeg") || FilePath.ends_with(".jpg")) {
            ProcessImages(FilePath);
        }

        else if (FilePath.ends_with(".glb") || FilePath.ends_with(".gltf")) {
            ProcessMeshes(FilePath);
        }
    }
}