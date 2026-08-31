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
#define MAX_LIGHTS 4

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

uniform PointLight pointlights[MAX_LIGHTS];
uniform int u_NumPointLights;

uniform Envoironment environment;
uniform Material material;
uniform DirectionalLight dirlight;
uniform PointLight pointlight;
uniform vec3 viewPos;

vec4 GetBaseColor() {
    vec4 texColor = texture(material.Diffuse, vTexCoord);
    
    if (texColor.a == 0.0) {
        texColor = vec4(1.0);
    }

    vec4 albedo = material.Albedo.a > 0.0 ? material.Albedo : vec4(1.0);
    vec4 vertexCol = vColor.a > 0.0 ? vColor : vec4(1.0);

    return texColor * albedo * vertexCol;
}

vec3 DirLightFunc(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 baseColor) {
    if (length(light.LightColor) <= 0.001 || length(light.LightDirection) <= 0.001) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(-light.LightDirection);
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float specPower = max(material.Specular, 1.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPower);

    vec3 diffuse = light.Diffuse * diff * baseColor;
    vec3 specular = light.Specular * spec * baseColor;

    return (diffuse + specular) * light.LightColor;
}

vec3 PointLightFunc(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor) {
    if (length(light.LightColor) <= 0.001) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(light.LightPosition - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float specPower = max(material.Specular, 1.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPower);

    float distance = length(light.LightPosition - fragPos);
    
    float denom = light.Constant + light.Linear * distance + light.Quadratic * (distance * distance);
    float attenuation = denom > 0.0 ? (1.0 / denom) : 0.0;

    vec3 diffuse = light.Diffuse * diff * baseColor;
    vec3 specular = light.Specular * spec * baseColor;

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular) * light.LightColor;
}

void main() {
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(viewPos - vPosition);
    vec4 baseColor = GetBaseColor();

    vec3 ambientContrib = environment.AmbientColor.rgb;
    if (length(ambientContrib) <= 0.001) {
        ambientContrib = vec3(0.05);
    } else {
        ambientContrib = ambientContrib * 0.1;
    }
    vec3 result = ambientContrib * baseColor.rgb;

    result += DirLightFunc(dirlight, normal, viewDir, baseColor.rgb);

    for (int i = 0; i < u_NumPointLights; i++) {
        result += PointLightFunc(pointlights[i], normal, vPosition, viewDir, baseColor.rgb);
    }

    FragmentColor = vec4(result, baseColor.a);
}