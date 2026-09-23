#ifndef SCREEN_TRIANGLE
#define SCREEN_TRIANGLE

vec4 ScreenTriangle() {
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);
    vec4 ndcPos = vec4(x, y, 1.0, 1.0);
    return ndcPos;
}

vec2 ScreenTriangleUV() {
    return ScreenTriangle().xy * 0.5 + 0.5;
}

#endif