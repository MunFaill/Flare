#pragma once

#include <cstdint>
#include <memory>

class Texture {
    public:
    virtual ~Texture() = default;

    virtual void Bind(uint32_t Slot) = 0;
    virtual void Unbind() = 0;
    virtual void SendData(unsigned char* data, uint32_t Width, uint32_t Height) = 0;
    virtual void Mipmaps(bool state) = 0;

    static std::unique_ptr<Texture> Create();
};
