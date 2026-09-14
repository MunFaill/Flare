#[VERTEX]
#version 460 core

#include "Object/ObjectVertex.glsl"

void main() {
    Vertex();
}

#[FRAGMENT]
#version 460 core

#include "Object/ObjectFragment.glsl"

void main() {
    Fragment();
}