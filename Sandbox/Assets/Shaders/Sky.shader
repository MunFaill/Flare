#[VERTEX]
#version 460 core

#include "Commom/ScreenTriangle.glsl"

uniform mat4 u_InverseProjection;
uniform mat4 u_InverseView;

out vec3 vRayDir;

void main() {
    vec4 viewPos = u_InverseProjection * ScreenTriangle();
    viewPos /= viewPos.w;

    mat3 invViewRot = mat3(u_InverseView);
    vRayDir = invViewRot * viewPos.xyz;

    gl_Position = ScreenTriangle();
}

#[FRAGMENT]
#version 460 core

in vec3 vRayDir;
out vec4 fragColor;

uniform sampler2D u_SkyTexture;
uniform float u_Exposure;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec3 rayDir = normalize(vRayDir);
    vec2 uv = SampleSphericalMap(rayDir);
    vec3 color = texture(u_SkyTexture, uv).rgb;
    color *= u_Exposure;
    fragColor = vec4(color, 1.0);
}