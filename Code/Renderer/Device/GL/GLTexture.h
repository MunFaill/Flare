#pragma once

#include <cstdint>

#include "Renderer/Device/DeviceTexture.h"

class GLTexture : public Texture {
    public:
        GLTexture();
        ~GLTexture() override;

        void Bind(uint32_t Slot = 0) override;
        void Unbind() override;
        void SendData(unsigned char* data, uint32_t Width, uint32_t Height) override;
        void Mipmaps(bool state) override;
    private:
        uint32_t TextureObject = 0;
        uint32_t LocalSlot = 0;
};