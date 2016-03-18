#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED
#include <SDL/SDL.h>
#include "Timer.hpp"
#include "game_elements.hpp"
#include "SDLTypingHandler.hpp"

#include <string>
#include <GL/glew.h>

#include "GL_DrawingPrimitives.hpp"
#include "Font.hpp"
//#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include "SDL/SDL_mixer.h"
#include <vector>
#include "Object.hpp"
#include "Static.hpp"
#include "Entity.hpp"
#include "CollisionSeries.hpp"
#include "CollisionPoint.hpp"

#include "vec_2d.hpp"
#include "Texture.hpp"

using namespace std;

typedef enum{
    GAMESTATE_INTRO,
    GAMESTATE_IN_GAME
} game_state_type;

class Game{
    public:
    bool initialize();
    void update();
    void quit();
    void draw();
    void toggle_debug_mode();
    void toggle_show_fps();
    void toggle_limit_fps();
    void toggle_vsync();
    void toggle_culled_drawing();
    void change_fps_limit(short fps_max);

    bool quit_at_end_of_frame, has_quit;

    bool fullscreen_mode;

    private:

    bool is_taking_typing_input;
        SDLTypingHandler typing_handler;
        int FRAMES_PER_SECOND_CAP, SCREEN_WIDTH, SCREEN_HEIGHT, DESKTOP_RES_WIDTH, DESKTOP_RES_HEIGHT,
        SCREEN_BPP, FONT_SIZE;
        int world_gravity;

            Texture collision_box, button_white_up, button_white_down, button_black_up, button_black_down,
            button_grey_up, button_grey_down, death_laser_emitter, death_laser_beam, exit_zone, death_zone;

            GLuint simple_vertex_shader;
            GLuint simple_fragment_shader;
            GLuint simple_glsl_program;

            int mult_color, add_color, blend_color, blend_density, hsl, brightness, contrast, colorize;

            bool white_button_is_pressed, black_button_is_pressed, grey_button_is_pressed;
            bool white_button_was_pressed, black_button_was_pressed, grey_button_was_pressed;

        short FOREGROUND_LAYER;
        bool fps_is_capped;
        Timer frame_time;

        Timer intro_timer;
        Timer player_falling_timer;
        Texture intro_image;
        float intro_image_alpha;

        bool debug_mode;
        bool culled_drawing_is_used;
        bool fps_is_shown;
        bool first_frame;
        bool optimization_state;
        bool super_optimization_state;
        bool just_returned_focus;

        short world_state;
        unsigned int fps_clock_sample;
        int current_frames_per_second;
        int frames_shown;

        SDL_Surface *screen;
        SDL_Event event;

        GLuint square; //these are display lists
        GLuint square2; //this one too

        struct Font* font;
        SDL_Color white;
        SDL_Color black;
        Mix_Music *music;
        vector<Mix_Chunk*> sounds;

        Level loaded_level;

        Camera camera;
        Mouse mouse;
        Player player_1;
        Player player_2;
        Uint8 *pressedkey;

        game_state_type game_state;

        void BuildLists();
        void ReloadTextures();
        void LoadHardCodedTextures();
        void GoToNextLevel();
        void ResizeVideo(int new_width, int new_height, bool fullscreen_mode = false);
        bool LoadLevel(std::string filename);
        void DeleteObjectinLevel(vector<Object*> &level_objects, int index);
        void GetInput();
        void CheckXCollisionsAndResolve(int recursions = 0);
        void CheckYCollisionsAndResolve(int recursions = 0);
        void KillPlayer(int player_number);
        point_2d CorrectForSphereCollision(int x, int y, int radius, int num_times_checked = 0);
        void UpdateLevelEntities();
        void FindVectorBelowPlayer();
        void FindClosestVectorToPlayer();
        void AttachEntityReferences();
        double watched_var;
        void AddWatch(double var_to_watch){watched_var = var_to_watch;};

};

#endif // GAME_HPP_INCLUDED
