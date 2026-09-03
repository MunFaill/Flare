#pragma once

#include <memory>

#include "Core/Engine.h"

class Application {
    public:
        virtual ~Application() = default;
        void Run();

        virtual void OnSetup() = 0;
        virtual void OnStart() = 0;
        virtual void OnUpdate(float DeltaTime) = 0;
        virtual void OnShutdown() = 0;
    protected:
        bool Running = true;
        std::unique_ptr<EngineModules> Modules;
};