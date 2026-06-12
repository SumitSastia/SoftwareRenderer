#include <texture.h>
#include <string>
#include <filesystem>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::filesystem::path base = std::filesystem::current_path();

void Texture::load(const char* path) {

    if (pixelData) {
        stbi_image_free(pixelData);
        delete pixelData;
    }

    std::string path_str(path);
    std::string base_str(base);

    base_str.erase(base_str.size() - 5);

    std::string finalPath = base_str + path_str;

    pixelData = stbi_load(finalPath.c_str(), &width, &height, nullptr, 4);

    if (!pixelData) {
        std::cerr << "Failed to Load Image!\n" << finalPath << std::endl;
        return;
    }

    // RGBA -> 4
    uint32_t totalBytes = width * height * 4;
    color = new uint32_t[width * height];

    for (uint32_t i = 0; i < totalBytes; i += 4) {

        uint8_t R = pixelData[i + 0];
        uint8_t G = pixelData[i + 1];
        uint8_t B = pixelData[i + 2];
        uint8_t A = pixelData[i + 3];

        color[i / 4] = (A << 24) | (B << 16) | (G << 8) | R;
    }
}

uint32_t Texture::sample(const float u, const float v) const {

    uint32_t tx = static_cast<uint32_t>(u * (width - 1));
    uint32_t ty = static_cast<uint32_t>(v * (height - 1));

    const uint32_t index = (ty * width + tx);

    return color[index];
}