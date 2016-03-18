#ifndef NAMESPACE_MAIN_EDITOR_HPP_INCLUDED
#define NAMESPACE_MAIN_EDITOR_HPP_INCLUDED

namespace main_editor
{
    //int lastFpsSample = 0;
    //int lastFps = 0;
    //int currentFps = 0;

    Timer frame_time; //timer is for frame limiting only at the moment
    SDLTypingHandler typing_handler;


        //create a level
    Level working_level;
    #define NO_FILE "n!o!f!i!l!e!"
    string loaded_filename = NO_FILE;


    int active_texture_index = 0;

    Texture editor_bg;
    Texture snap_grid_bg;
    Texture snap_grid_overlay;
    Texture editor_cursor;
    Texture editor_cursor_move;
    Texture editor_cursor_rotate;
    Texture editor_cursor_scale;
    Texture editor_cursor_clip;

    Texture collision_box;
    Texture collision_box_white;
    Texture collision_box_black;
    Texture collision_box_grey;
    Texture collision_box_red;
    Texture collision_box_green;
    Texture marker;
    Texture respawn_zone;
    Texture exit_zone;
    Texture death_zone;
    Texture death_laser;
    Texture player_red;
    Texture player_green;
    Texture button_white_up;
    Texture button_black_up;
    Texture button_grey_up;

    GLuint simple_vertex_shader;
    GLuint simple_fragment_shader;
    GLuint simple_glsl_program;

    int mult_color, add_color, blend_color, blend_density, hsl, brightness, contrast, colorize;

    int active_layer = 2;
    float active_layer_parallax_mod = 1.00;


    int mx = 0; //Create some variables to keep track of the mouse x and y easily
    int my = 0;
    int mx_inworld = 0;
    int my_inworld = 0;

    #define CONTROLS 1
    #define DEBUG_TEXT 2
    int text_info_state = CONTROLS;

    #define NO_OBJECT_HOVERED_OVER -1
    #define NO_OBJECT_GRABBED -1
    int ObjectHoveredOverIndex = NO_OBJECT_HOVERED_OVER;
    int ObjectGrabbedIndex = NO_OBJECT_GRABBED;

    #define NO_COLLISION_SERIES_ACTIVE -1

    int ActiveCollisionSeriesIndex = NO_COLLISION_SERIES_ACTIVE;

    enum editor_manipulation_type{
    MOVE,
    ROTATE,
    SCALE,
    CLIP
    };

    editor_manipulation_type manipulation_mode = MOVE;

    enum basic_object_type{
        STATIC,
        ENTITY,
        COLLISION_VECTOR
    };

    basic_object_type object_editing_type = STATIC;

    int initial_grab_offset_x = 0;
    int initial_grab_offset_y = 0;
    int initial_grab_mx = 0;
    int initial_grab_my = 0;
    int initial_grab_object_x = 0;
    int initial_grab_object_y = 0;
    int initial_grab_object_w = 0;
    int initial_grab_object_h = 0;
    int initial_grab_object_rotation = 0;
    int initial_grab_angle = 0;
    int initial_pallete_scroll = 0;
    int pallete_scroll = 0;
    int texture_pallete_image_size = 50;

    float initial_grab_object_u = 0;
    float initial_grab_object_u2 = 0;
    float initial_grab_object_v = 0;
    float initial_grab_object_v2 = 0;
    bool is_grabbing_secondary_uv = false;

    bool left_side_is_hovered = false;
    bool right_side_is_hovered = false;
    bool top_side_is_hovered = false;
    bool bottom_side_is_hovered = false;

    bool left_side_is_grabbed = false;
    bool right_side_is_grabbed = false;
    bool top_side_is_grabbed = false;
    bool bottom_side_is_grabbed = false;
    SDL_Rect initial_grab_object_bounds;

    float camera_x = 0.00;
    float camera_y = 0.00;
    float initial_grab_camera_x = 0;
    float initial_grab_camera_y = 0;
    float initial_cam_grab_mx = 0;
    float initial_cam_grab_my = 0;
    float initial_texture_bar_grab_mx = 0;
    float camera_zoom = 1.00;

    Object* Hovered_Object = NULL;
    Object* Grabbed_Object = NULL;

    CollisionPoint* Closest_Collision_Point = NULL;
    CollisionPoint* Hovered_Collision_Point = NULL;
    CollisionPoint* Grabbed_Collision_Point = NULL;
    int Closest_Collision_Point_Index;

    CollisionSeries* Hovered_Collision_Series = NULL;
    CollisionSeries* Grabbed_Collision_Series = NULL;

    #define NUMBER_OF_ENTITY_TYPES 23
    string entity_types[NUMBER_OF_ENTITY_TYPES] =
    {
        "CollisionType",
        "CollisionType_White_Button",
        "CollisionType_Black_Button",
        "CollisionType_Grey_Button",
        "CollisionType_Red",
        "CollisionType_Green",
        "CollisionType_Puzzle_Barrier",
        "CollisionType_Death",
        "CollisionType_Death_Green",
        "CollisionType_Death_Red",
        "ButtonType_White",
        "ButtonType_Black",
        "ButtonType_Grey",
        "ParticleSystemType",
        "PlayerStartType_Red",
        "PlayerStartType_Green",
        "MarkerType",
        "ExitZoneType",
        "RespawnZoneType",
        "DeathZoneType",
        "DeathLaserType",
        "SignType",
        "GrassType"
    };

    unsigned int active_entity_type_index = 0;

    bool is_adding_point = false;
    bool grid_snapping_is_on = false;

    bool is_object_grabbed = false;
    bool is_object_hovered = false;

    bool shift_is_pressed = false;
    bool alt_is_pressed = false;
    bool space_was_pressed_last_frame = false;
    bool mouse_is_in_pallete = false;
    bool initial_mouse_is_in_pallete = false;

    enum input_type{
        INPUT_NONE,
        INPUT_LOAD_TEXTURE,
        INPUT_LOAD_LEVEL,
        INPUT_SAVE_LEVEL,
    };

    input_type input_mode = INPUT_NONE;

    Uint8 *pressedkey = NULL;

    bool is_taking_typing_input = false;
    bool entity_editor_is_open = false;

    bool should_copy_static_settings = false;
    bool should_paste_static_settings = false;

    Object* Clipboard_Static;

    bool is_actual_screen_size_shown = true;

    vector<BaseUIElement*> entity_editor_UIElements;
    int entity_editor_active_index = -1;
    bool entity_editor_is_initialized = false;
    bool is_actually_editing_a_static = false;
    bool is_actually_editing_global_settings = false;

    bool is_game_preview_mode_on = false;
    bool are_collision_boxes_always_shown = true;
    bool info_text_is_hidden = false;

    int in_game_overlay_offset_x = 0;
    int in_game_overlay_offset_y = 0;
    int in_game_camera_x = 0;
    int in_game_camera_y = 0;

    unsigned int fps_clock_sample;
    int current_frames_per_second = 0;
    int frames_shown = 0;

    bool under_object_modifier_is_active = false;

    Status_List editor_status_list;
    const SDL_VideoInfo* DesktopVideoInfo;

            //we make a pointer to access the level's array of the currently selected object type (ENTITY or STATIC) in a polymorphic way
        //so that I don't have to duplicate a bunch of code
    vector<Object*> *working_level_statics_or_entities;

}

#endif // NAMESPACE_MAIN_EDITOR_HPP_INCLUDED
