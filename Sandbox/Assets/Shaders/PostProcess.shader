#[VERTEX]
#version 460 core

#include "Commom/ScreenTriangle.glsl"

out vec2 v_UV;

void main() {
    vec4 pos = ScreenTriangle();
    gl_Position = pos;
    v_UV = ScreenTriangleUV();
}

#[FRAGMENT]
#version 460 core

in vec2 v_UV;

out vec4 fragColor;

uniform sampler2D u_ScreenTexture;

void main() {

    float strength = 0.002;

    float r = texture(u_ScreenTexture, v_UV + vec2(strength, 0.0)).r;
    float g = texture(u_ScreenTexture, v_UV).g;
    float b = texture(u_ScreenTexture, v_UV - vec2(strength, 0.0)).b;

    fragColor = vec4(r, g, b, 1.0);
}