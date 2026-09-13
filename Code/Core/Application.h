#pragma once

#include <memory>
#include <flecs.h>

#include "Core/Engine.h"

class Application {
    public:
        virtual ~Application() = default;
        void Run();

        virtual void OnSetup() = 0;
        virtual void OnStart() = 0;
        virtual void OnUpdate(float DeltaTime) = 0;
        virtual void OnShutdown() = 0;
        flecs::world World;
    protected:
        bool Running = true;
        std::unique_ptr<EngineModules> Modules;
};