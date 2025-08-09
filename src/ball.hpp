#pragma once

#include "sdl/sdl_draw.hpp"
#include "models.hpp"



inline void draw_ball(SDL_Renderer* renderer) {

    const Color ball_color {255, 255, 255, 255};
    const Circle ball { 320, 210, 10, ball_color};
    draw_circle(renderer, ball);
}