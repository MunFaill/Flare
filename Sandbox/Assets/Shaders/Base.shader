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

struct Envoironment {
    vec4 AmbientColor;
};

struct Material {
    sampler2D Diffuse;
    vec4 Albedo;
    float Specular;
};

struct DirectionalLight {
    vec3 LightDirection;
    vec3 LightColor;
    vec3 Diffuse;
    vec3 Specular;
};

struct PointLight {
    vec3 LightPosition;
    vec3 LightColor;
    vec3 Diffuse;
    vec3 Specular;
    float Constant;
    float Linear;
    float Quadratic;
};

#define MAX_LIGHTS 4

uniform Envoironment environment;
uniform Material material;
uniform DirectionalLight dirlight;
uniform PointLight pointlight;
uniform vec3 viewPos;

vec3 DirLightFunc(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.LightDirection);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Specular);

    vec3 diffuse = light.Diffuse * diff * vec3(texture(material.Diffuse, vTexCoord));
    vec3 specular = light.Specular * spec * vec3(texture(material.Diffuse, vTexCoord));

    return (diffuse + specular) * light.LightColor;
}

vec3 PointLightFunc(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.LightPosition - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Specular);

    float distance = length(light.LightPosition - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    vec3 diffuse = light.Diffuse * diff * vec3(texture(material.Diffuse, vTexCoord));
    vec3 specular = light.Specular * spec * vec3(texture(material.Diffuse, vTexCoord));

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular) * light.LightColor;
}

void main() {
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(viewPos - vPosition);

    vec3 result = DirLightFunc(dirlight, normal, viewDir) + PointLightFunc(pointlight, normal, vPosition, viewDir);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        result += PointLightFunc(pointlight, normal, vPosition, viewDir);
    }

    FragmentColor = vec4(result, 1.0);
}