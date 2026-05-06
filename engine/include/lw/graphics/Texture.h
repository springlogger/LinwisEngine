#pragma once

#include <lw/helpers/stb_image.h>

#include <string>
#include <stdint.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include <stdexcept>


namespace lw
{

struct Color {
  unsigned char r, g, b;
};

//TODO: реализовать кэш текстур, чтобы материалы ссылающиеся на 1 текстуру её не дублировали
class Texture {
public:
    Texture() {
        width    = 1;
        height   = 1;
        channels = 4;
        pixels   = { 0xFFFFFFFF };
    }

    explicit Texture(const std::string& filepath) {
        // png считает v - сверху, у меня v - снизу
        stbi_set_flip_vertically_on_load(true);

        uint8_t* pixels_raw = stbi_load(filepath.c_str(), &width, &height, &channels, 4);
        if (!pixels_raw) {
            throw std::runtime_error("Failed to load: " + filepath + ", reason: " + stbi_failure_reason());
        }

        pixels.resize(width * height);
        std::memcpy(pixels.data(), pixels_raw, width * height * 4);
        channels = 4;

        stbi_image_free(pixels_raw);
    }

    int getWidth()  const { return width; }
    int getHeight() const { return height; }
    const std::vector<uint32_t>& getPixels() const { return pixels; }

private:
    std::vector<uint32_t> pixels;
    int width    = 0;
    int height   = 0;
    int channels = 0;
};

} // namespace lw
