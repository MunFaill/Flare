#ifndef MATERIAL
#define MATERIAL

struct Material {
    sampler2D Diffuse;
    sampler2D Specular;
    vec4 Albedo;
    float SpecularPower;
};

vec4 GetBaseColor(Material mat, vec2 uv, vec4 vertexColor) {
    vec4 texColor = texture(mat.Diffuse, uv);
    if (texColor.a == 0.0) texColor = vec4(1.0);

    vec4 albedo = mat.Albedo.a > 0.0 ? mat.Albedo : vec4(1.0);
    vec4 vColor = vertexColor.a > 0.0 ? vertexColor : vec4(1.0);

    return texColor * albedo * vColor;
}

vec4 GetSpecularColor(Material mat, vec2 uv) {
    vec4 texColor = texture(mat.Specular, uv);
    if (texColor.a == 0.0) texColor = vec4(1.0);
    return texColor;
}

#endif