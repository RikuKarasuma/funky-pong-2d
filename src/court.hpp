#pragma once

#include <SDL2/SDL.h>
#include "sdl_draw.hpp"



inline void draw_court(SDL_Renderer* renderer) {

    // Draw outer court
    const Color outer_color {255, 0, 0, 255};
    const SDL_Rect outer_court {0, 0, 640, 420}; 
    draw_rect(renderer, outer_color, outer_court);

    // Draw inner court
    const Color inner_color {0, 0, 0, 200};
    const SDL_Rect inner_court {10, 10, 620, 400}; 
    draw_rect(renderer, inner_color, inner_court);
}
