#include "sdl/sdl_setup.hpp"
#include "sdl/sdl_exit.hpp"
#include "sdl/sdl_draw.hpp"
#include "court.hpp"
#include "ball.hpp"
#include "cstdlib"
#include "ctime"
#include "cmath"
#include <iostream>
#include "player.hpp"
#include <random>


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

bool random_boolean(double p = 0.5) {
    static std::mt19937 rnd(std::random_device{}());
    return std::bernoulli_distribution(p)(rnd);
}

void main_loop() {

    Event event;

    // random seed
    std::srand(static_cast<unsigned>(time(nullptr)));

    Vector ball { 320.0f, 210.0f };

    Vector player {
        40,
        (COURT_HEIGHT / 2) - (PLAYER_HEIGHT / 2)
    };

    Vector ai {
        COURT_WIDTH - 40,
        (COURT_HEIGHT / 2) - (PLAYER_HEIGHT / 2)
    };

    const float angle = 
        (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 
        (2.0f * (M_PI / 180.f));

    Vector ball_velocity {
        random_boolean() ? (std::cos(angle) * SPEED) : - (std::cos(angle) * SPEED),
        (std::sin(angle) * SPEED)
    };

    while (true) {

        event = check_sdl_events();

        if (event == EXIT) {
            break;
        }

        // Boundary and ball movement.
        check_boundaries(ball_velocity, ball);
        check_player_boundaries(player, ai, ball, ball_velocity);
        ai_movement(ball, player, ai);

        // Clear the screen before each draw.
        sdl_clear(renderer);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // Drawing logic
        draw_court(renderer);
        draw_ball(renderer, ball);
        draw_player(renderer, player);
        draw_player(renderer, ai);

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
