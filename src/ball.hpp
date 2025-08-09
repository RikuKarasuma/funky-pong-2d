#pragma once

#include "sdl/sdl_draw.hpp"
#include "models.hpp"



inline void draw_ball(SDL_Renderer* renderer,
                      float x,
                      float y) {

    const Color ball_color {255, 255, 255, 255};
    const Circle ball {
        static_cast<int16_t>(x),
        static_cast<int16_t>(y),
        10,
        ball_color
    };
    draw_circle(renderer, ball);
}