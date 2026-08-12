#pragma once

#include <Flare.h>

class App : public Application {
    public:
        App();
        ~App() override;

        void OnStart() override;
        void OnUpdate() override;
        void OnStop() override;
};
