#pragma once

#include "Core/Engine.h"

class Application {
    public:
        virtual ~Application() = default;

        inline virtual void OnStart() {}
        inline virtual void OnUpdate() {}
        inline virtual void OnStop() {}

        void Run();

    protected:
        EngineContext Context;
        bool Running = true;
};
