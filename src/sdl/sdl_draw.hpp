#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../models.hpp"

inline void sdl_clear(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}


inline void draw_rect(SDL_Renderer* renderer,
                      const Color& color,
                      const SDL_Rect& rect) {

    SDL_SetRenderDrawColor(
        renderer,
        color.red,
        color.green,
        color.blue,
        color.alpha
    );

    SDL_RenderFillRect(renderer, &rect);
}

inline void draw_circle(SDL_Renderer* renderer,
                        const Circle& circle) {

    circleRGBA(
        renderer,
        circle.x,
        circle.y,
        circle.radius,
        circle.color.red,
        circle.color.green,
        circle.color.blue,
        circle.color.alpha
    );

    filledCircleRGBA(
        renderer,
        circle.x,
        circle.y,
        circle.radius,
        circle.color.red,
        circle.color.green,
        circle.color.blue,
        circle.color.alpha
    );
}
