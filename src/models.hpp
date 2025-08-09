#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

struct Circle {
    int16_t x;
    int16_t y;
    int16_t radius;
    Color color;
};