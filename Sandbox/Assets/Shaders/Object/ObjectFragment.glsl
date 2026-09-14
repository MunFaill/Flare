#ifndef OBJECT_FRAGMENT_SHADER
#define OBJECT_FRAGMENT_SHADER

#include "../Light/Lighting.glsl"
#include "../Material/Material.glsl"

out vec4 FragmentColor;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec4 vColor;

uniform Material material;
uniform vec3 viewPos;

void Fragment() {
    vec4 baseColor = GetBaseColor(material, vTexCoord, vColor);
    vec4 specColor = GetSpecularColor(material, vTexCoord);

    vec3 litColor = EvaluateLighting(vPosition, vNormal, viewPos, baseColor.rgb, specColor.rgb, material.SpecularPower);

    FragmentColor = vec4(litColor, baseColor.a);
}

#endif