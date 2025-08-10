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
    // float x { 320.0f };
    // float y { 210.0f };
    Vector ball { 320.0f, 210.0f };

    // float player_x { 40 };
    // float player_y { (court_height / 2) - 37.5 };

    Vector player {
        40,
        (court_height / 2) - 37.5
    };

    // float ai_x { court_width - 40 };
    // float ai_y { (court_height / 2) - 37.5 };

    Vector ai {
        court_width - 40,
        (court_height / 2) - 37.5
    };

    const float angle = 
        (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 
        (2.0f * M_PI);

    Vector ball_velocity {
        (std::cos(angle) * SPEED), //* (3.14f / 180.0f),
        (std::sin(angle) * SPEED) //* (3.14f / 180.0f)
    };

    while (true) {

        event = check_sdl_events();

        if (event == EXIT) {
            break;
        }

        // Need to fix the ball getting stuck at the top and bottom boundaries.
        ball_velocity.x = check_and_invert(ball.x, court_x, court_width, ball_velocity.x);
        ball_velocity.y = check_and_invert(ball.y, court_y, court_height, ball_velocity.y);

        // Determine that the ball has hit either paddle
        const VectorPair player_new_velocity_and_position = player_check_and_invert(ball, player, ball_velocity, true);
        ball_velocity = player_new_velocity_and_position.one;
        ball = player_new_velocity_and_position.two;
        const VectorPair ai_new_velocity_and_position = player_check_and_invert(ball, ai, ball_velocity, false);
        ball_velocity = ai_new_velocity_and_position.one;
        ball = player_new_velocity_and_position.two;

        ball.x += ball_velocity.x;
        ball.y += ball_velocity.y;

        // If ball isn't within it's Y axis zone.
        if (!(ball.y > player.y && ball.y < (player.y + 75))) {
            // If the ball is below or above the player,
            // Move the player towards the ball.
            if (player.y > ball.y) {
                player.y = player.y - (SPEED / 2);
            } else if (player.y < (ball.y + 75)) {
                player.y = player.y + (SPEED / 2);
            }
        }

        // If ball isn't within it's Y axis zone.
        if (!(ball.y > ai.y && ball.y < (ai.y + 75))) {
            // If the ball is below or above the player,
            // Move the player towards the ball.
            if (ai.y > ball.y) {
                ai.y = ai.y - (SPEED / 2);
            } else if (ai.y < (ball.y + 75)) {
                ai.y = ai.y + (SPEED / 2);
            }
        }


        // Clear the screen before each draw.
        sdl_clear(renderer);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // Drawing logic
        draw_court(renderer);
        draw_ball(renderer, ball.x, ball.y);
        draw_player(renderer, player.x, player.y);
        draw_player(renderer, ai.x, ai.y);

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