#include <Flare.h>

class Sandbox : public Application {
    public:
        Sandbox() = default;
        ~Sandbox() override = default;

        void OnSetUp() override;
        void OnStart() override;
        void OnUpdate() override;
        void OnStop() override;
};