#include "sdl_setup.hpp"
#include "sdl_exit.hpp"
#include "sdl_draw.hpp"
#include "court.hpp"

static short status {0};

enum Event {
    EXIT,
    NOTHING
};

Event check_sdl_events() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {

            return EXIT;
        }
    }

    return NOTHING;
}

void main_loop() {

    Event event;

    while (true) {

        event = check_sdl_events();

        if (event == EXIT) {
            break;
        }

        sdl_clear(renderer);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        draw_court(renderer);

        SDL_RenderPresent(renderer);
    }
}


int main() {

    status = sdl_setup();

    main_loop();
    
    sdl_exit(window, renderer);
    cout << "Exiting..." << endl;
    return status;
}