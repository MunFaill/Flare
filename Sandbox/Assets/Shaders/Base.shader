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

vec3 light() {
    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    vec3 lightColor = vec3(1.0);

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vPosition);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * lightColor;

    vec3 result = (ambient + diffuse) * vColor.xyz;
    return result;
}

void main() {
    FragmentColor = vec4(light(), vColor.a);
}