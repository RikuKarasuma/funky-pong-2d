#pragma once

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;


inline void sdl_exit(SDL_Window* window, SDL_Renderer* renderer) {

    cout << "Destroying SDL..." << endl;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    cout << "Destuction complete." << endl;
}