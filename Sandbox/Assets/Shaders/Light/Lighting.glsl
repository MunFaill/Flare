#ifndef LIGHTING_SHADER
#define LIGHTING_SHADER

#define MAX_LIGHTS 8

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

vec3 DirLightFunc(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 baseColor, vec3 specColor, float specPower) {
    if (length(light.LightColor) <= 0.001 || length(light.LightDirection) <= 0.001) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(-light.LightDirection);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.Diffuse * diff * baseColor;
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), max(specPower, 1.0));
    vec3 specular = light.Specular * spec * specColor;

    return (diffuse + specular) * light.LightColor;
}

vec3 PointLightFunc(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor, vec3 specColor, float specPower) {
    if (length(light.LightColor) <= 0.001) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(light.LightPosition - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), max(specPower, 1.0));
    float distance = length(light.LightPosition - fragPos);
    
    float denom = light.Constant + light.Linear * distance + light.Quadratic * (distance * distance);
    float attenuation = denom > 0.0 ? (1.0 / denom) : 0.0;

    vec3 diffuse = light.Diffuse * diff * baseColor;
    vec3 specular = light.Specular * spec * specColor;

    return ((diffuse + specular) * attenuation) * light.LightColor;
}

#endif