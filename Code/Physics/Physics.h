#pragma once

#include <flecs.h>

struct Physics {
    void Initialize(const flecs::world& World);
    void Update(const flecs::world& World, float DeltaTime);
    void Shutdown();
};