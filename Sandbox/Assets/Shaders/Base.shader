#[VERTEX]
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = vec4(aPosition, 1.0);

    vPosition = aPosition;
    vNormal = aNormal;
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

void main() {
    FragmentColor = vColor;
}