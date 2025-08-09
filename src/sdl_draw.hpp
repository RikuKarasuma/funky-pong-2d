#pragma once

#include <SDL2/SDL.h>
#include "models.hpp"

inline void sdl_clear(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}


inline void draw_rect(SDL_Renderer* renderer,
                      Color color,
                      SDL_Rect rect) {

    SDL_SetRenderDrawColor(
        renderer,
        color.red,
        color.green,
        color.blue,
        color.alpha
    );

    SDL_RenderFillRect(renderer, &rect);
}