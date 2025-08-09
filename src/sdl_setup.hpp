#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <format>

using namespace std;

static SDL_Window* window;
static SDL_Renderer* renderer;

inline void sdl_error(const char* message) {

    cerr << &message << SDL_GetError() << endl;
}

inline short sdl_setup() {

    cout << "Starting SDL init..." << endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {

        sdl_error("SDL initialization error: ");
        return 1;
    }

    cout << "Creating window..." << endl;
    window = SDL_CreateWindow(
        "Funky Pong 2D",
        0,
        0,
        640,
        420,
        SDL_WINDOW_SHOWN
    );

    if (!window) {

        sdl_error("SDL Window creation error: ");
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {

        sdl_error("SDL Renderer creation error: ");
        SDL_Quit();
        return 1;
    }

    return 0;
}