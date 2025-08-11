#pragma once

#include "sdl/sdl_draw.hpp"
#include "models.hpp"
#include "court.hpp"

static const uint16_t BALL_RADIUS = 10;
static const float SPEED = 7.5f;


inline void draw_ball(SDL_Renderer* renderer, const Vector& position) {

    const Color ball_color {255, 255, 255, 255};
    const Circle ball {
        static_cast<int16_t>(position.x),
        static_cast<int16_t>(position.y),
        BALL_RADIUS,
        ball_color
    };
    draw_circle(renderer, ball);
}

inline void check_boundaries(Vector& ball_velocity, const Vector& ball) {

    // todo Need to fix the ball getting stuck at the top and bottom boundaries.
    ball_velocity.x = check_and_invert(ball.x, COURT_X, COURT_WIDTH, ball_velocity.x);
    ball_velocity.y = check_and_invert(ball.y, COURT_Y, COURT_HEIGHT, ball_velocity.y);
}