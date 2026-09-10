#pragma once

#include <string>
#include <glm/glm.hpp>

struct Material {
    std::string ShaderID = "Base";
    std::string DiffuseTextureID = "DefaultDiffuse";
    std::string SpecularTextureID = "DefaultSpecular";
    glm::vec4 Albedo{1.0f};
    float SpecularPower = 32.0f;
};