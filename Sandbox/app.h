#include <Flare.h>

class App : public Application {
    public:
        App() = default;
        ~App() override = default;

        void OnSetup() override;
        void OnStart() override;
        void OnUpdate(float DeltaTime, float FramesPerSecond) override;
        void OnShutdown() override;
};