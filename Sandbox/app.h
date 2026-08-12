#pragma once

#include <Flare.h>

class App : public Application {
    public:
        App() = default;
        ~App() override = default;

        void OnStart() override;
        void OnUpdate() override;
        void OnStop() override;
};
