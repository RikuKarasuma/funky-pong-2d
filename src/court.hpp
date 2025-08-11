#pragma once

#include <SDL2/SDL.h>
#include "sdl/sdl_draw.hpp"

const static int16_t COURT_X = 25;
const static int16_t COURT_Y = 25;
const static int16_t COURT_WIDTH = 615;
const static int16_t COURT_HEIGHT = 395;

inline void draw_court(SDL_Renderer* renderer) {

    // Draw outer court
    const Color outer_color {150, 0, 0, 100};
    const SDL_Rect outer_court {0, 0, 640, 420};
    draw_rect(renderer, outer_color, outer_court);

    // Draw inner court
    const Color inner_color {0, 0, 0, 225};
    const SDL_Rect inner_court {10, 10, 620, 400}; 
    draw_rect(renderer, inner_color, inner_court);
}

inline bool at_boundary(float current, float start, float end) {

    return current < start || current > end;
}

inline float check_and_invert(float current,
                              float start,
                              float end,
                              float velocity) {

    if (at_boundary(current, start, end)) {

        return -velocity;
    }

    return velocity;
}
