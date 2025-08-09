#include "sdl/sdl_setup.hpp"
#include "sdl/sdl_exit.hpp"
#include "sdl/sdl_draw.hpp"
#include "court.hpp"
#include "ball.hpp"
#include "cstdlib"
#include "ctime"
#include "cmath"
#include <iostream>


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

    // random seed
    std::srand(static_cast<unsigned>(time(nullptr)));

    const int16_t court_x = 25;
    const int16_t court_width = 615;
    const int16_t court_y = 25;
    const int16_t court_height = 395;

    // Starting point for ball  
    float x {320.0f};
    float y {210.0f};

    const float angle = 
        static_cast<float>(rand()) /
        static_cast<float>(RAND_MAX) * 
        10.0f *
        M_PI;

    const float speed = 5.0f;
    float velocity_x = std::cos(angle) * speed;
    float velocity_y = std::sin(angle) * speed;

    while (true) {

        event = check_sdl_events();

        if (event == EXIT) {
            break;
        }

        velocity_x = check_and_invert(x, court_x, court_width, velocity_x);
        velocity_y = check_and_invert(y, court_y, court_height, velocity_y);

        x += velocity_x;
        y += velocity_y;


        // Clear the screen before each draw.
        sdl_clear(renderer);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        draw_court(renderer);
        draw_ball(renderer, x, y);


        SDL_RenderPresent(renderer);
        // 16 ms per frame roughly 60 fps.
        SDL_Delay(16);
    }
}


int main() {

    status = sdl_setup();

    main_loop();
    
    sdl_exit(window, renderer);
    cout << "Exiting..." << endl;
    return status;
}