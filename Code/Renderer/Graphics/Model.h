#pragma once

#include <string>
#include <vector>

struct MeshPart {
    std::string MeshID;
    std::string MaterialID;
};

struct Model {
    std::vector<MeshPart> MeshParts;
};