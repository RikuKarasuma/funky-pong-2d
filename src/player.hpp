#include "sdl/sdl_draw.hpp"
#include "models.hpp"
#include "ball.hpp"

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

inline bool at_player(float x,
                      float y,
                      float player_x,
                      float player_y) {
    
    return 
        x < (player_x + PLAYER_WIDTH + BALL_RADIUS + 10) &&
        y < (player_y + PLAYER_HEIGHT + BALL_RADIUS + 10) &&
        x > (player_x - BALL_RADIUS - 10) &&
        y > (player_y - BALL_RADIUS - 10);
}

inline float player_check_and_invert(float x, 
                                     float y, 
                                     float player_x, 
                                     float player_y,
                                     float velocity) {
    
    if (at_player(x, y, player_x, player_y)) {

        return -velocity;
    }

    return velocity;
}