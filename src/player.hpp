#include "sdl/sdl_draw.hpp"
#include "models.hpp"
#include "ball.hpp"
#include "cstdlib"
#include "cmath"

inline constexpr short PLAYER_WIDTH = 25;
inline constexpr short PLAYER_HEIGHT = 75;
inline constexpr short BOUNDARY_BUFFER = 10;

inline void draw_player(SDL_Renderer* renderer, const Vector& position) {

    Color player_color { 255, 255, 255, 255 };
    SDL_Rect player {
        static_cast<int16_t>(position.x),
        static_cast<int16_t>(position.y),
        PLAYER_WIDTH,
        PLAYER_HEIGHT 
    };
    draw_rect(renderer, player_color, player);
}

inline bool at_player(const Vector& ball, const Vector& player) {
    
    return 
        ball.x < (player.x + PLAYER_WIDTH + BALL_RADIUS + BOUNDARY_BUFFER) &&
        ball.y < (player.y + PLAYER_HEIGHT + BALL_RADIUS + BOUNDARY_BUFFER) &&
        ball.x > (player.x - BALL_RADIUS - BOUNDARY_BUFFER) &&
        ball.y > (player.y - BALL_RADIUS - BOUNDARY_BUFFER);
}

inline Vector contact_normal(const Vector& ball, const Vector& player) {

    // Calculate overlaps.
    const float overlap_x_left = ball.x - (player.x - BALL_RADIUS - BOUNDARY_BUFFER);
    const float overlap_x_right = (player.x + PLAYER_WIDTH + BALL_RADIUS + BOUNDARY_BUFFER) - ball.x;
    const float overlap_y_top = ball.y - (player.y - BALL_RADIUS - BOUNDARY_BUFFER);
    const float overlap_y_bottom = (player.y + PLAYER_HEIGHT + BALL_RADIUS + BOUNDARY_BUFFER) - ball.y;

    // smallest overlap is the contact side.
    const float minX = std::min(overlap_x_left, overlap_x_right);
    const float minY = std::min(overlap_y_top, overlap_y_bottom);

    if (minX < minY) {

        return 
            (overlap_x_left < overlap_x_right) ? 
            Vector{-1.0f, 0.0f} :
            Vector{+1.0f, 0.0f};
    }
    else {

        return 
            (overlap_y_top < overlap_y_bottom) ?
            Vector{0.0f, -1.0f} :
            Vector{0.0f, +1.0f};
    }
}

inline float penetration_amount(const Vector& ball, const Vector& player, const Vector& normal) {

    // Calculate the ball penetration in order to determine the amount to push away by.
    const float overlap_x_left = ball.x - (player.x - BALL_RADIUS - BOUNDARY_BUFFER);
    const float overlap_x_right = (player.x + PLAYER_WIDTH + BALL_RADIUS + BOUNDARY_BUFFER) - ball.x;
    const float overlap_y_top = ball.y - (player.y - BALL_RADIUS - BOUNDARY_BUFFER);
    const float overlap_y_bottom = (player.y + PLAYER_HEIGHT + BALL_RADIUS + BOUNDARY_BUFFER) - ball.y;

    const float push =
                    (normal.x < 0) ? overlap_x_left :
                    (normal.x > 0) ? overlap_x_right :
                    (normal.y < 0) ? overlap_y_top : overlap_y_bottom;

    return push + 0.01f;
}


inline float product(const Vector& one, const Vector& two) {

    return one.x * two.x + one.y * two.y;
}

inline Vector multiply(const Vector& vector, const float by) {

    return {
        vector.x * by,
        vector.y * by
    };
}

inline Vector subtract(const Vector& vector, const Vector& two) {

    return {
        vector.x - two.x,
        vector.y - two.y
    };
}

inline Vector reflect(const Vector& vector, const Vector& n_unit) {

    Vector reflect_multple = multiply(n_unit, 2.0f * product(vector, n_unit));

    return subtract(
        vector,
        reflect_multple
    );
}

inline Vector rotate(const Vector& vector, float radians) {

    const float cos = std::cos(radians);
    const float sin = std::sin(radians);

    return {
        cos * vector.x - sin * vector.y,
        sin * vector.x + cos * vector.y
    };
}

inline VectorPair player_check_and_invert(const Vector& player,
                                          const bool left_paddle,
                                          Vector& ball,
                                          Vector& velocity) {
    
    if (at_player(ball, player)) {

        const Vector normal = contact_normal(ball, player);

        if (velocity.x * normal.x + velocity.y * normal.y > 0.0f) {

            return {
                velocity,
                ball
            };
        }


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
        const float push_amount = penetration_amount(ball, player, normal);
        ball.x += normal.x * push_amount;
        ball.y += normal.y * push_amount;

        Vector new_velocity = reflect(velocity, normal);

        if (std::fabs(normal.x) > 0.5f) {
            new_velocity = rotate(new_velocity, +phi_radians);
        }

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

inline void check_player_boundaries(const Vector& player,
                                    const Vector& ai,
                                    Vector& ball,
                                    Vector& ball_velocity) {

    // Determine that the ball has hit the left paddle.
    VectorPair ball_new_velocity_and_position = player_check_and_invert(
        player,
        true,
        ball,
        ball_velocity
    );

    ball_velocity.x = ball_new_velocity_and_position.one.x;
    ball_velocity.y = ball_new_velocity_and_position.one.y;

    // Determine that the ball has hit the right paddle.
    ball_new_velocity_and_position = player_check_and_invert(
        ai,
        false,
        ball,
        ball_velocity
    );

    // Update ball velocity and new ball position.
    ball_velocity.x = ball_new_velocity_and_position.one.x;
    ball_velocity.y = ball_new_velocity_and_position.one.y;
    ball.x += ball_velocity.x;
    ball.y += ball_velocity.y;
}


inline void ai_movement(Vector& ball, Vector& player, Vector& ai) {

    // If ball isn't within it's Y axis zone.
    if (!(ball.y > player.y && ball.y < (player.y + PLAYER_HEIGHT))) {
        // If the ball is below or above the player,
        // Move the player towards the ball.
        if (player.y > ball.y) {
            player.y = player.y - (SPEED / 2);
        } else if (player.y < (ball.y + PLAYER_HEIGHT)) {
            player.y = player.y + (SPEED / 2);
        }
    }

    // If ball isn't within it's Y axis zone.
    if (!(ball.y > ai.y && ball.y < (ai.y + PLAYER_HEIGHT))) {
        // If the ball is below or above the player,
        // Move the player towards the ball.
        if (ai.y > ball.y) {
            ai.y = ai.y - (SPEED / 2);
        } else if (ai.y < (ball.y + PLAYER_HEIGHT)) {
            ai.y = ai.y + (SPEED / 2);
        }
    }
}
