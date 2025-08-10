#pragma once

#include "sdl/sdl_draw.hpp"
#include "models.hpp"

static const uint16_t BALL_RADIUS = 10;
static const float SPEED = 7.5f;


inline void draw_ball(SDL_Renderer* renderer,
                      float x,
                      float y) {

    const Color ball_color {255, 255, 255, 255};
    const Circle ball {
        static_cast<int16_t>(x),
        static_cast<int16_t>(y),
        BALL_RADIUS,
        ball_color
    };
    draw_circle(renderer, ball);
}