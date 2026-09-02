#[VERTEX]
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = u_ViewProjection * u_Model * vec4(aPosition, 1.0);

    vPosition = vec3(u_Model * vec4(aPosition, 1.0));
    vNormal = mat3(transpose(inverse(u_Model))) * aNormal;
    vTexCoord = aTexCoord;
    vColor = aColor;
}

#[FRAGMENT]
#version 460 core

#include "Light/Lighting.glsl"
#include "Material/Material.glsl"

out vec4 FragmentColor;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec4 vColor;

struct Environment {
    vec4 AmbientColor;
};

uniform PointLight pointlights[MAX_LIGHTS];
uniform int u_NumPointLights;

uniform Environment environment;
uniform Material material;
uniform DirectionalLight dirlight;
uniform vec3 viewPos;

void main() {
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(viewPos - vPosition);
    
    vec4 baseColor = GetBaseColor(material, vTexCoord, vColor);
    vec4 specColor = GetSpecularColor(material, vTexCoord);

    vec3 ambientContrib = length(environment.AmbientColor.rgb) <= 0.001 ? vec3(0.05) : environment.AmbientColor.rgb * 0.1;

    vec3 result = ambientContrib * baseColor.rgb;

    result += DirLightFunc(dirlight, normal, viewDir, baseColor.rgb, specColor.rgb, material.SpecularPower);

    for (int i = 0; i < u_NumPointLights; i++) {
        result += PointLightFunc(pointlights[i], normal, vPosition, viewDir, baseColor.rgb, specColor.rgb, material.SpecularPower);
    }

    FragmentColor = vec4(result, baseColor.a);
}