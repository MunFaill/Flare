#pragma once

#include "Core/Engine.h"

class Application {
    public:
        virtual ~Application() = default;

        virtual void OnSetUp() = 0;
        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnStop() = 0;

        void Run();
    protected:
        bool Running = true;
        EngineContext Context;
};