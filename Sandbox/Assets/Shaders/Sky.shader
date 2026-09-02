#[VERTEX]
#version 460 core

uniform mat4 u_InverseProjection;
uniform mat4 u_InverseView;

out vec3 vRayDir;

void main() {
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);
    vec4 ndcPos = vec4(x, y, 1.0, 1.0);
    vec4 viewPos = u_InverseProjection * ndcPos;
    viewPos /= viewPos.w;

    mat3 invViewRot = mat3(u_InverseView);
    vRayDir = invViewRot * viewPos.xyz;

    gl_Position = ndcPos;
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