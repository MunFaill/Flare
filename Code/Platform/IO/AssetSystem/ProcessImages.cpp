#include "Platform/IO/AssetSystem/AssetProcessor.h"
#include "Platform/IO/AssetSystem/Assets.h"
#include "Platform/IO/FileSystem/File.h"

#include <cstdint>
#include <memory>
#include <print>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void AssetProcessor::ProcessImages(const std::string& File) {
    stbi_set_flip_vertically_on_load(true);
    int W, H, Nrc;
    uint8_t* Data = stbi_load(File.c_str(), &W, &H, &Nrc, 4);

    if (!Data) {
        std::println("Failed to load image: {}", File);
        return;
    }

    std::unique_ptr<Texture> texture = Texture::Create();
    texture->SendData(Data, W, H);
    stbi_image_free(Data);
    Assets::Textures.Add(File::GetFileName(File), std::move(texture));
}