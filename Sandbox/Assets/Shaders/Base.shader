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

out vec4 FragmentColor;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec4 vColor;

struct Material {
    sampler2D Texture;
    vec4 Albedo;
};

struct Light {
    vec3 LightPos;
    vec3 LightColor;
};

#define MAX_LIGHTS 8

uniform Material material;
uniform Light light;

vec3 lightFunc() {
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.LightPos - vPosition);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.LightColor;

    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * vec3(1.0); // Ambient color

    vec3 result = (ambient + diffuse) * material.Albedo.xyz;
    return result;
}

void main() {
    vec4 texColor = texture(material.Texture, vTexCoord);
    FragmentColor = vec4(lightFunc() * texColor.rgb, material.Albedo.a * texColor.a);
}