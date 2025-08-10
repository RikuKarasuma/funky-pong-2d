#include "sdl/sdl_draw.hpp"
#include "models.hpp"
#include "ball.hpp"
#include "cstdlib"
#include "cmath"

static const int16_t PLAYER_WIDTH = 25;
static const int16_t PLAYER_HEIGHT = 75;

inline void draw_player(SDL_Renderer* renderer, float x, float y) {

    Color player_color { 255, 255, 255, 255 };
    SDL_Rect player {
        static_cast<int16_t>(x),
        static_cast<int16_t>(y),
        PLAYER_WIDTH,
        PLAYER_HEIGHT 
    };
    draw_rect(renderer, player_color, player);
}

inline bool at_player(Vector ball, Vector player) {
    
    return 
        ball.x < (player.x + PLAYER_WIDTH + BALL_RADIUS + 10) &&
        ball.y < (player.y + PLAYER_HEIGHT + BALL_RADIUS + 10) &&
        ball.x > (player.x - BALL_RADIUS - 10) &&
        ball.y > (player.y - BALL_RADIUS - 10);
}

inline float product(Vector one, Vector two) {

    return one.x * two.x * one.y * two.y;
}

inline Vector multiply(Vector vector, float by) {

    return {
        vector.x * by,
        vector.y * by
    };
}

inline Vector subtract(Vector vector, Vector two) {

    return {
        vector.x - two.x,
        vector.y - two.y
    };
}

inline Vector reflect(Vector vector, Vector n_unit) {

    return subtract(
        vector,
        multiply(n_unit, 2.0f * product(vector, n_unit))
    );
}

inline Vector rotate(Vector vector, float radians) {

    const float cos = std::cos(radians);
    const float sin = std::sin(radians);

    return {
        cos * vector.x - sin * vector.y,
        sin * vector.x + cos * vector.y
    };
}

inline VectorPair player_check_and_invert(Vector ball,
                                          Vector player,
                                          Vector velocity,
                                          bool left_paddle) {
    
    if (at_player(ball, player)) {

        // Calculate offset from the center of the paddle.
        const float half_paddle_height = 37.5f;
        const float player_center = player.y + half_paddle_height;
        float offset = (ball.y - player_center) / half_paddle_height;

        // Normalize the offset.
        if (offset < -1.0f) {
            offset = -1.0f;
        } else if (offset > 1.0f) {
            offset = 1.0f;
        }

        // Calculate the new direction.
        const float max_degrees = 25.0f;
        const float phi_radians = offset * (max_degrees * M_PI / 180.0f);
        const Vector n = (left_paddle ? Vector { 1.0f, 0.0f } : Vector { -1.0f, 0.0f });
        Vector new_velocity = reflect(velocity, n);
        new_velocity = rotate(new_velocity, +phi_radians);

        // Calculate the new speed.
        const float speed = std::sqrt(
            (new_velocity.x * new_velocity.x) + 
            (new_velocity.y * new_velocity.y)
        ) * 0.2f;

        // Ensure the ball is heading away from the respective paddle.
        if (left_paddle && new_velocity.x <= 0.0f) { 
            new_velocity.x = std::max(std::abs(new_velocity.x), speed);
        }
        
        if (!left_paddle && new_velocity.x >= 0.0f) {
             new_velocity.x = -std::max(std::abs(new_velocity.x), speed);
        }

        // Ensure the ball doesn't get stuck within the paddle.
        ball.x += left_paddle ? +0.5f : -0.5f;

        return {
            new_velocity,
            ball
        };
    }

    return {
        velocity,
        ball
    };
}


/**
 *         const VectorPair player_new_velocity_and_position = player_check_and_invert(ball, player, ball_velocity, true);
        ball_velocity = player_new_velocity_and_position.one;
        //ball = player_new_velocity_and_position.two;
        const VectorPair ai_new_velocity_and_position = player_check_and_invert(ball, ai, ball_velocity, true);
        ball_velocity = ai_new_velocity_and_position.one;
        //ball = player_new_velocity_and_position.two;
 */