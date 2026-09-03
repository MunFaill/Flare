#include <Flare.h>

class App : public Application {
    public:
        App() = default;
        ~App() override = default;

        void OnSetup() override;
        void OnStart() override;
        void OnUpdate(float delta) override;
        void OnShutdown() override;
};