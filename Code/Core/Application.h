#pragma once

#include "Core/Engine.h"
#include <string>

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
        std::string WindowTilte = "";
        uint32_t WindowWidth = 0, WIndowHeight = 0;
};
