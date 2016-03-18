#ifndef GAME_ELEMENTS_HPP_INCLUDED
#define GAME_ELEMENTS_HPP_INCLUDED
#include "Object.hpp"
#include "Static.hpp"
#include "Entity.hpp"
#include "Texture.hpp"
#include "CollisionSeries.hpp"
#include "Animation.hpp"
#include <vector>
#include "vec_2d.hpp"
struct Layer {
            float parallax_modifier; // affects how much parallax the layer has
            vector <Object*> statics; // a vector which contains all statics in the layer;
            vector <Object*> entities;
};

struct Level {
        float background_r, background_g, background_b;
        int width; //these two variables
        int height; //may or may not be necessary
        Layer layers[8]; //layers to contain all statics and entities in the level
        vector <Texture> textures; //a vector to contain all textures used by statics in the level
        vector <CollisionSeries> collision_series;
};

struct Camera{
    float x, y, center_x, center_y;
    int width, height;
    float zoom;
    float rot;
};

struct Mouse{
    int x, y;
    Texture cursor;
};

#define PLAYER_RIGHT_FACING true
#define PLAYER_LEFT_FACING false

typedef enum{
    PLAYER_FALLING,
    PLAYER_STARTING_RUNNING,
    PLAYER_RUNNING,
    PLAYER_STOPPING_RUNNING,
    PLAYER_STOPPED,
    PLAYER_TURNING_RUNNING,
    PLAYER_TURNING_JUMPING,
    PLAYER_JUMPING_RUNNING,
    PLAYER_JUMPING_STOPPED
} player_state;

struct Player{
    player_state state;
    int rotation, closest_collision_series_index, pre_jump_rotation, jump_timer;
    short jump_num;
    float x, y, xv, yv, surface_velocity, xuv, yuv;
    float w,h;
    float penetration_amount;
    SDL_Rect respawn_zone;
    signed char jump_value, jump_interval_timer;
    //signed char current_frame;
    bool in_air, is_riding, pushing_other_player, was_pushing_other_player, upside_down, on_ground, against_wall, against_cieling;
    vec_2d vector_below, closest_vector;
    Animation run_animation, standing_animation, jump_animation;
    Texture standing_frame;
    Texture starting_running_frame;
};

#endif // GAME_ELEMENTS_HPP_INCLUDED
