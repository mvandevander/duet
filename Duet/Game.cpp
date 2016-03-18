#include "Game.hpp"

#include "version.h"
#include <SDL/SDL.h>
#include "SDL/SDL_mixer.h"
#include "Vsync.hpp"
#include "trig_functions_degrees.hpp"
#include "game_elements.hpp"
#include <sstream>
#include "GL_DrawingPrimitives.hpp"
#include "Entity_GrassType.hpp"
#include "Entity_SignType.hpp"
#include "Entity_CollisionType.hpp"
#include "Entity_CollisionType_White_Button.hpp"
#include "Entity_CollisionType_Black_Button.hpp"
#include "Entity_CollisionType_Grey_Button.hpp"
#include "Entity_CollisionType_Green.hpp"
#include "Entity_CollisionType_Red.hpp"
#include "Entity_CollisionType_Puzzle_Barrier.hpp"
#include "Entity_CollisionType_Death.hpp"
#include "Entity_CollisionType_Death_Green.hpp"
#include "Entity_CollisionType_Death_Red.hpp"
#include "Entity_ButtonType_White.hpp"
#include "Entity_ButtonType_Black.hpp"
#include "Entity_ButtonType_Grey.hpp"
#include "Entity_PlayerStartType_Red.hpp"
#include "Entity_PlayerStartType_Green.hpp"
#include "Entity_RespawnZoneType.hpp"
#include "Entity_DeathZoneType.hpp"
#include "Entity_ExitZoneType.hpp"
#include "Entity_DeathLaserType.hpp"
#include "Entity_MarkerType.hpp"
#include <string>

#include <GL/glew.h>

#if defined(_WIN32)
    #include <GL/wglew.h>
#endif

#include "GL_ShaderFuncs.hpp"
#include <SDL/SDL_image.h>

#include "vec_2d.hpp"

#define DELETE_ALL -1 //used as shorthand (longhand?) for the DeleteObjectInLevel function

void Game::DeleteObjectinLevel(vector<Object*> &level_objects, int index)
{
    if(index==DELETE_ALL){
        for(unsigned int i = 0; i<level_objects.size(); i++)
        {
            delete level_objects[i]; //the data stored at the pointer must be deleted
        }
        level_objects.clear();
    }
    else if(index!=DELETE_ALL){
        delete level_objects[index]; //the data stored at the pointer must be deleted
        level_objects.erase(level_objects.begin() + index);
    }
}

bool Game::initialize()
{
    SCREEN_WIDTH = 1280;
    SCREEN_HEIGHT = 720;
    SCREEN_BPP = 32;
    FONT_SIZE = 14;
    FRAMES_PER_SECOND_CAP = 60;
    fps_is_capped = true;
    screen = NULL;
    //font = NULL;
    white.r = 255;
    white.g = 255;
    white.b = 255;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    debug_mode = false;
    fps_is_shown = false;
    culled_drawing_is_used = true;

    //game_state = GAMESTATE_INTRO;
    game_state = GAMESTATE_IN_GAME;

    if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
        fprintf(stderr, "Unable to initialize SDL_mixer with 48000Hz 16-bit audio: %s\n", SDL_GetError());
        return false;
    }
    music = Mix_LoadMUS("sfx/Destiny.mp3");
    if(music == NULL) fprintf(stderr, "Failed loading music: sfx/Destiny.mp3\n");
    //if(music2 == NULL) fprintf(stderr, "The muzak2 didn't load!\n");
    if( Mix_FadeInMusic( music, -1, 2000)==-1) fprintf(stderr,"sfx/Destiny.mp3 is not playing\n");
    sounds.push_back(Mix_LoadWAV("sfx/wind1.ogg"));
    if(sounds[0] == NULL) fprintf(stderr,"Failed loading sound: sfx/wind1.ogg\n");
    Mix_FadeInChannel(-1, sounds[0], -1,1000);
    //sounds.push_back(Mix_LoadWAV("sfx/running_sound.ogg"));
    //sounds.push_back(Mix_LoadWAV("sfx/grass_rustle.ogg"));

    const SDL_VideoInfo* DesktopVideoInfo = SDL_GetVideoInfo();
    DESKTOP_RES_WIDTH = DesktopVideoInfo->current_w;
    DESKTOP_RES_HEIGHT = DesktopVideoInfo->current_h;

    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_WM_SetCaption("Duet","Duet");

    //bool fullscreen_mode = true;

    int xoffset = 0;
    int yoffset = 0;
    int new_width = 1280;
    int new_height = 720;
    if(fullscreen_mode == false) screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,SDL_OPENGL|SDL_DOUBLEBUF);
    else {
        screen = SDL_SetVideoMode(DESKTOP_RES_WIDTH,DESKTOP_RES_HEIGHT,0,SDL_OPENGL|SDL_FULLSCREEN|SDL_DOUBLEBUF);
        if(new_width>DESKTOP_RES_WIDTH)
            {
                double screen_ratio = (double)new_height/(double)new_width;
                new_width=DESKTOP_RES_WIDTH;
                new_height=nearbyint(screen_ratio*(double)DESKTOP_RES_WIDTH);
                xoffset = (DESKTOP_RES_WIDTH-new_width)/2;
                yoffset = (DESKTOP_RES_HEIGHT-new_height)/2;
            }
            else if(new_height>DESKTOP_RES_HEIGHT)
            {
                double screen_ratio = (double)new_width/(double)new_height;
                new_height=DESKTOP_RES_HEIGHT;
                new_width=nearbyint(screen_ratio*(double)DESKTOP_RES_HEIGHT);
                xoffset = (DESKTOP_RES_WIDTH-new_width)/2;
                yoffset = (DESKTOP_RES_HEIGHT-new_height)/2;
            }
            else if(new_width<=DESKTOP_RES_WIDTH)
            {
                double desktop_ratio = (double)DESKTOP_RES_HEIGHT/(double)DESKTOP_RES_WIDTH;
                int new_screen_height=nearbyint(desktop_ratio*(double)new_width);
                screen = SDL_SetVideoMode(new_width,new_screen_height,0,SDL_OPENGL|SDL_FULLSCREEN|SDL_DOUBLEBUF);
                xoffset = 0;
                yoffset = (new_screen_height-new_height)/2;
            }
            /*else if(new_height<DESKTOP_RES_HEIGHT)
            {
                double desktop_ratio = (double)DESKTOP_RES_WIDTH/(double)DESKTOP_RES_HEIGHT;
                int new_screen_width=nearbyint(desktop_ratio*(double)new_height);
                screen = SDL_SetVideoMode(new_screen_width,new_height,0,SDL_OPENGL|SDL_FULLSCREEN|SDL_DOUBLEBUF);
                xoffset = (new_screen_width-new_width)/2;
                yoffset = 0;
            }*/
    }
    glewExperimental=true;
    glewInit();

    SDL_ShowCursor(0); //turns off the SDL cursor, so that we can draw our own

    glClearColor(0,0,0,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,SCREEN_WIDTH,SCREEN_HEIGHT,0,-1,1);
    glViewport(xoffset,yoffset,new_width,new_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    InitVSync();
    SetVSyncState(true);

    LoadLevel("lvl/world1.lvl");

    AttachEntityReferences();

    loaded_level.layers[0].parallax_modifier = 1.5;
    loaded_level.layers[1].parallax_modifier = 1;
    loaded_level.layers[2].parallax_modifier = 1;
    loaded_level.layers[3].parallax_modifier = .63;
    loaded_level.layers[4].parallax_modifier = .40;
    loaded_level.layers[5].parallax_modifier = .25;
    loaded_level.layers[6].parallax_modifier = .1;
    loaded_level.layers[7].parallax_modifier = 0;

    BuildLists();


    //LOAD ALL TEXTURES AFTER THIS POINT!!!
    //
    //FAILURE TO COMPLY WILL RESULT IN A CTD ERROR
    has_quit = false;
    quit_at_end_of_frame = false;
    mouse.cursor.load("img/editor/editor_cursor.tga");
    mouse.x = 0;
    mouse.y = 0;

    intro_image.load("img/UI/intro_image.tga");
    intro_image_alpha = 0;
    intro_timer.Start();

    LoadHardCodedTextures(); //<--Shaders are also loaded here

    //camera.x = 706-SCREEN_WIDTH/2;
    //camera.y = 0;
    camera.x = 0;
    camera.y = 0;
    camera.width = SCREEN_WIDTH;
    camera.height = SCREEN_HEIGHT;
    camera.zoom = 1.0;

    player_1.x = 0; player_1.y = 200; player_1.w = 38; player_1.h = 32; player_1.xv = 0; player_1.yv = 0; player_1.surface_velocity = 0; player_1.state = PLAYER_STOPPED; player_1.in_air = true; player_1.rotation = 0; player_1.standing_frame.load("img/players/player_1.tga");
    player_2.x = 100; player_2.y = 200; player_2.w = 32; player_2.h = 50; player_2.xv = 0; player_2.yv = 0; player_2.surface_velocity = 0; player_2.state = PLAYER_STOPPED; player_2.jump_num = 0; player_2.in_air = true; player_2.rotation = 0; player_2.standing_frame.load("img/players/player_2.tga");

    //So, we want to find out the player start positions eh? Well, we will just look for them in the loaded_level structure

    for(unsigned int i = 0; i<loaded_level.layers[2].entities.size();i++){
            if(loaded_level.layers[2].entities[i]->type=="PlayerStartType_Red"){
                player_1.x = loaded_level.layers[2].entities[i]->bounds.x;
                player_1.y = loaded_level.layers[2].entities[i]->bounds.y;
            }
            if(loaded_level.layers[2].entities[i]->type=="PlayerStartType_Green"){
                player_2.x = loaded_level.layers[2].entities[i]->bounds.x;
                player_2.y = loaded_level.layers[2].entities[i]->bounds.y;
            }
    }


    player_1.closest_collision_series_index = 0;
    first_frame = true;

    //world_gravity = 2;
    world_state = 0; //this specifies what world we are in, which changes different behaviours of the players
    FOREGROUND_LAYER = 2;

    fps_clock_sample = SDL_GetTicks();
    current_frames_per_second = 0;
    frames_shown = 0;


    return true;
}

//AttachEntityReferences()
//------------------------
//Some of the entities in the game reference other entities by an index number,
//however this index number is not actually the index inside of the entities vector
//to avoid doing a linear search through all entities everytime we need to reference
//one entity from another, we update the indexes to reflect the actual vector index.

void Game::AttachEntityReferences()
{
    for(int layer=7;layer>-1;layer--)
    {
        for(int i=0;(unsigned int)(i)<loaded_level.layers[layer].entities.size();i++)
        {
            if(loaded_level.layers[layer].entities[i]->type=="ExitZoneType")
            {
                Entity_ExitZoneType* active_entity = reinterpret_cast<Entity_ExitZoneType*>(loaded_level.layers[layer].entities[i]);
                if(active_entity->puzzle_barrier_index!=-1){
                    int working_reference_index = -1;
                    for(unsigned int a = 0; a<loaded_level.layers[layer].entities.size(); a++)
                    {

                        if(loaded_level.layers[layer].entities[a]->type=="CollisionType_Puzzle_Barrier")
                        {
                            Entity_CollisionType_Puzzle_Barrier* barrier_to_test = reinterpret_cast<Entity_CollisionType_Puzzle_Barrier*>(loaded_level.layers[layer].entities[a]);
                            if(barrier_to_test->index == active_entity->puzzle_barrier_index) working_reference_index = a;
                        }

                    }
                    if(working_reference_index!=-1) active_entity->puzzle_barrier_index = working_reference_index;
                }
            }
            else if(loaded_level.layers[layer].entities[i]->type=="CollisionType_Puzzle_Barrier")
            {
                Entity_CollisionType_Puzzle_Barrier* active_entity = reinterpret_cast<Entity_CollisionType_Puzzle_Barrier*>(loaded_level.layers[layer].entities[i]);
                int working_reference_index = -1;
                for(unsigned int a = 0; a<loaded_level.layers[layer].entities.size(); a++)
                {
                    if(loaded_level.layers[layer].entities[a]->type=="RespawnZoneType")
                    {
                        Entity_RespawnZoneType* respawn_zone_to_test = reinterpret_cast<Entity_RespawnZoneType*>(loaded_level.layers[layer].entities[a]);
                        if(respawn_zone_to_test->index == active_entity->tied_respawn_zone_index) working_reference_index = a;
                    }

                }
                if(working_reference_index!=-1) active_entity->tied_respawn_zone_index = working_reference_index;
            }
        }
    }

}

void Game::GoToNextLevel()
{
    if(world_state==0) //world 1
    {
        LoadLevel("lvl/world2.lvl");
        world_state=1;
    }
    else if(world_state==1)
    {
        LoadLevel("lvl/world3.lvl");
        world_state=2;
    }
    AttachEntityReferences();

    for(unsigned int i = 0; i<loaded_level.layers[2].entities.size();i++){
            if(loaded_level.layers[2].entities[i]->type=="PlayerStartType_Red"){
                player_1.x = loaded_level.layers[2].entities[i]->bounds.x;
                player_1.y = loaded_level.layers[2].entities[i]->bounds.y;
            }
            if(loaded_level.layers[2].entities[i]->type=="PlayerStartType_Green"){
                player_2.x = loaded_level.layers[2].entities[i]->bounds.x;
                player_2.y = loaded_level.layers[2].entities[i]->bounds.y;
            }
    }
    player_1.xv = 0; player_1.yv = 0; player_1.surface_velocity = 0; player_1.state = PLAYER_STOPPED; player_1.in_air = true;
    player_2.xv = 0; player_2.yv = 0; player_2.surface_velocity = 0; player_2.state = PLAYER_STOPPED; player_2.in_air = true;

    first_frame = true;

}

void Game::LoadHardCodedTextures()
{
    //REMEMBER TO PUT ANY OTHER TEXTURES THAT WILL BE NEEDED HERE:
    player_1.standing_frame.load("img/players/player_1.tga");
    player_2.standing_frame.load("img/players/player_2.tga");

    button_white_up.load("img/button_white_up.tga");
    button_black_up.load("img/button_black_up.tga");
    button_grey_up.load("img/button_grey_up.tga");
    button_white_down.load("img/button_white_down.tga");
    button_black_down.load("img/button_black_down.tga");
    button_grey_down.load("img/button_grey_down.tga");
    collision_box.load("img/editor/collision_box_white.tga");
    death_laser_emitter.load("img/death_laser_emitter.tga");
    death_laser_beam.load("img/death_laser_beam.tga");
    exit_zone.load("img/exit_zone.tga");
    death_zone.load("img/editor/death_zone.tga");

    simple_vertex_shader = LoadAndBuildShader(GL_VERTEX_SHADER,"shaders/simple.vert");
    simple_fragment_shader = LoadAndBuildShader(GL_FRAGMENT_SHADER,"shaders/simple.frag");
    simple_glsl_program = glCreateProgram();

    glAttachShader(simple_glsl_program, simple_vertex_shader);
    glAttachShader(simple_glsl_program, simple_fragment_shader);
    glLinkProgram(simple_glsl_program);

    mult_color = glGetUniformLocation(simple_glsl_program,"mult_color");
    add_color = glGetUniformLocation(simple_glsl_program,"add_color");
    blend_color = glGetUniformLocation(simple_glsl_program,"blend_color");
    blend_density = glGetUniformLocation(simple_glsl_program,"blend_density");
    hsl = glGetUniformLocation(simple_glsl_program,"hsl");
    brightness = glGetUniformLocation(simple_glsl_program,"brightness");
    contrast = glGetUniformLocation(simple_glsl_program,"contrast");
    colorize = glGetUniformLocation(simple_glsl_program,"colorize");

    GLint linked;
    glGetProgramiv(simple_glsl_program, GL_LINK_STATUS, &linked);
    if(linked){//yay
    }
    else{
        GLint blen = 0;
        GLsizei slen = 0;

        glGetProgramiv(simple_glsl_program, GL_INFO_LOG_LENGTH, &blen);
        if(blen>0){
            GLchar* compiler_log = (GLchar*)malloc(blen);
            glGetInfoLogARB(simple_glsl_program, blen, &slen, compiler_log);
            fprintf(stderr, "Shader Linker Error: %s \n", compiler_log);
            free(compiler_log);
        }
    }

    //font = LoadFont("GEORGIA.ttf", FONT_SIZE);
    //if(font==NULL){
      //  fprintf(stderr, "Unable to load font for use.\n" );
    //}
    font = UnserializeFont("font/Georgia.font");


}

void Game::ReloadTextures()
{
    for(unsigned int i = 0; i<loaded_level.textures.size(); i++) //go through all the textures in the file
    {
        loaded_level.textures[i] = (LoadTexture(loaded_level.textures[i].filename)); //load the filename specified and push the texture ont the end of loaded_level's textures
    }

    LoadHardCodedTextures();

}

void Game::ResizeVideo(int new_width, int new_height, bool fullscreen)
{
    int xoffset = 0, yoffset = 0;
    if(!fullscreen) screen = SDL_SetVideoMode(new_width,new_height,0,SDL_OPENGL|SDL_DOUBLEBUF);
    else {
            screen = SDL_SetVideoMode(DESKTOP_RES_WIDTH,DESKTOP_RES_HEIGHT,0,SDL_OPENGL|SDL_FULLSCREEN|SDL_DOUBLEBUF);
            if(new_width>DESKTOP_RES_WIDTH)
            {
                double screen_ratio = (double)new_height/(double)new_width;
                new_width=DESKTOP_RES_WIDTH;
                new_height=nearbyint(screen_ratio*(double)DESKTOP_RES_WIDTH);
                xoffset = (DESKTOP_RES_WIDTH-new_width)/2;
                yoffset = (DESKTOP_RES_HEIGHT-new_height)/2;
            }
            else if(new_height>DESKTOP_RES_HEIGHT)
            {
                double screen_ratio = (double)new_width/(double)new_height;
                new_height=DESKTOP_RES_HEIGHT;
                new_width=nearbyint(screen_ratio*(double)DESKTOP_RES_HEIGHT);
                xoffset = (DESKTOP_RES_WIDTH-new_width)/2;
                yoffset = (DESKTOP_RES_HEIGHT-new_height)/2;
            }
            else if(new_width<=DESKTOP_RES_WIDTH)
            {
                double desktop_ratio = (double)DESKTOP_RES_HEIGHT/(double)DESKTOP_RES_WIDTH;
                int new_screen_height=nearbyint(desktop_ratio*(double)new_width);
                screen = SDL_SetVideoMode(new_width,new_screen_height,0,SDL_OPENGL|SDL_FULLSCREEN|SDL_DOUBLEBUF);
                xoffset = 0;
                yoffset = (new_screen_height-new_height)/2;
            }
            /*else if(new_height<DESKTOP_RES_HEIGHT)
            {
                double desktop_ratio = (double)DESKTOP_RES_WIDTH/(double)DESKTOP_RES_HEIGHT;
                int new_screen_width=nearbyint(desktop_ratio*(double)new_height);
                screen = SDL_SetVideoMode(new_screen_width,new_height,0,SDL_OPENGL|SDL_FULLSCREEN|SDL_DOUBLEBUF);
                xoffset = (new_screen_width-new_width)/2;
                yoffset = 0;
            }*/
    }
    glClearColor(0,0,0,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,SCREEN_WIDTH,SCREEN_HEIGHT,0,-1,1);
    glViewport(xoffset,yoffset,new_width,new_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    InitVSync();
    //SetVSyncState(true);
    ReloadTextures(); // this reloads all textures needed by the level
    BuildLists();

}

void Game::quit()
{
    Mix_CloseAudio();
    //SDL_Quit(); //well why the fuck does leaving this here cause the program to throw a wierd garbled frame at the end?
}

void Game::change_fps_limit(short fps_max)
{
    FRAMES_PER_SECOND_CAP = fps_max;
}

void Game::update()
{
        if(fps_clock_sample + 1000 < SDL_GetTicks())
        {
            current_frames_per_second = frames_shown;
            frames_shown = 0;
            fps_clock_sample = SDL_GetTicks();
        }
        else frames_shown++;
        if(game_state==GAMESTATE_INTRO){
            GetInput();
            if(intro_timer.GetTicks()>500&&intro_timer.GetTicks()<6000)
            {
                intro_image_alpha+=.005;
            }
            else intro_image_alpha-=.005;
            if(intro_timer.GetTicks()>14000)
            {
                intro_image_alpha=1.0;
                intro_timer.Stop();
                game_state=GAMESTATE_IN_GAME;
            }

        }
        else if(game_state==GAMESTATE_IN_GAME){
            frame_time.Start();
            GetInput();
            UpdateLevelEntities();

            camera.center_x =(camera.x+(SCREEN_WIDTH/2));
            camera.center_y =(camera.y+(SCREEN_HEIGHT/2)+100);
            float avg_target_x = (player_1.x+player_2.x)/2;
            float avg_target_y = (player_1.y+player_2.y)/2;
            if(first_frame == true){
                first_frame = false;
                camera.x+=(avg_target_x-camera.center_x);
                camera.y+=(avg_target_y-camera.center_y);
            }
            else{

                //if(abs(int(avg_target_x-camera_center_x))>5) camera.x+=(avg_target_x-camera_center_x)/10;
                //if(abs(int(avg_target_y-camera_center_y))>5) camera.y+=(avg_target_y-camera_center_y)/10;
                camera.x+=(avg_target_x-camera.center_x)/10;
                camera.y+=(avg_target_y-camera.center_y)/10;
            }
            //if(camera.x<0) camera.x = 0;
            //if(camera.y+SCREEN_HEIGHT>128) camera.y = 128-SCREEN_HEIGHT;


            /*if(view_xview[0]<0) view_xview[0] = 0
            if(view_yview[0]<0) view_yview[0] = 0
            if(view_xview[0]>room_width-view_wview[0]) view_xview[0]=room_width-view_wview[0]
            if(view_yview[0]>room_height-view_hview[0]) view_yview[0]=room_height-view_hview[0]*/
        }
}

void Game::toggle_debug_mode()
{
    debug_mode = (debug_mode) ? false : true;
}


void Game::toggle_show_fps()
{
    fps_is_shown = (fps_is_shown) ? false : true;
}

void Game::toggle_limit_fps()
{
    fps_is_capped = (fps_is_capped) ? false : true;
}

void Game::toggle_vsync()
{
    SetVSyncState( (VSyncEnabled()) ? false : true);
}

void Game::toggle_culled_drawing()
{
    culled_drawing_is_used = (culled_drawing_is_used) ? false : true;
}


void Game::GetInput()
{
    pressedkey = SDL_GetKeyState(NULL);
        while(SDL_PollEvent(&event)){

                if(event.type == SDL_QUIT||pressedkey[SDLK_ESCAPE]) quit_at_end_of_frame = true;
                if(game_state==GAMESTATE_INTRO&&event.type == SDL_KEYDOWN){
                    game_state=GAMESTATE_IN_GAME;
                    intro_image_alpha=1.0;
                }
                if(event.type == SDL_MOUSEMOTION){
                    mouse.x = event.motion.x;
                    mouse.y = event.motion.y;
                }
                while(!(SDL_GetAppState() & SDL_APPINPUTFOCUS)){
                    SDL_Delay(100);
                    SDL_PollEvent(&event);
                    just_returned_focus = true;
                    if(event.type == SDL_VIDEOEXPOSE)
                    {
                        SDL_GL_SwapBuffers();
                    }
                }

                if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_PAGEDOWN and debug_mode)
                {
                    GoToNextLevel();
                }
                /*if(!player_1.on_ground)
                {
                    if(player_1.jump_timer<=4) player_1.jump_timer+=1;

                }
                else player_1.jump_timer = 0;*/

                if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_UP&&player_1.jump_timer<=5)
                {
                    //This may require some explanation, as the velocity here is not perhaps what should be expected
                    //we get this velocity by taking the velocity of the jump in the prototype (-11) and halving that,
                    //because of the doubled fps. But also, in the prototype, the jump input was taken after gravity
                    //was applied, so we need to compensate for the fact that now it isn't. So therefore we add the
                    //gravity back to our initial velocity. But we must add the gravity times two, because of the
                    //doubled fps thing again. So one frame in this version is two frames in the prototype. So, this
                    //extra .5 velocity is immediately removed when the gravity is applied. It mainly just saves me
                    //from having to add a boolean or some weird timer that sees if two frames have passed.
                      player_1.yv=-6;
                      player_1.on_ground = false;
                }


                if(player_2.jump_num == 0 and player_2.jump_timer>=5) player_2.jump_num = 1;
                if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_w)
                {
                    if(player_2.jump_timer<=5 or player_2.jump_num == 1)
                    {
                          player_2.yv=-6.5;
                          player_2.on_ground = false;
                          player_2.jump_num+=1;
                    }
                }

                if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_0)
                {
                    ResizeVideo(SCREEN_WIDTH,SCREEN_HEIGHT,true);
                }
                if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_9)
                {
                    ResizeVideo(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,true);
                }
                if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1)
                {
                    ResizeVideo(SCREEN_WIDTH,SCREEN_HEIGHT,false);
                }
                if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2)
                {
                    ResizeVideo(SCREEN_WIDTH*.8,SCREEN_HEIGHT*.8,false);
                }
                if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_3)
                {
                    ResizeVideo(SCREEN_WIDTH*.6,SCREEN_HEIGHT*.6,false);
                }
            }
            if(game_state==GAMESTATE_IN_GAME){


                //Update_Player_1()
                if(pressedkey[SDLK_RIGHT]&&player_1.xv>=0){if(player_1.xv<(7+player_1.surface_velocity)) player_1.xv+=.25;}
                else if(pressedkey[SDLK_RIGHT]&&player_1.xv<=0) player_1.xv+=.5;
                else if(pressedkey[SDLK_LEFT]&&player_1.xv<=0) {if(player_1.xv>(-7+player_1.surface_velocity)) player_1.xv-=.25;}
                else if(pressedkey[SDLK_LEFT]&&player_1.xv>=0) player_1.xv-=.5;
                else if(player_1.on_ground){
                    if(player_1.xv>player_1.surface_velocity) {
                        player_1.xv-=.75;
                        if(player_1.xv<player_1.surface_velocity) player_1.xv=player_1.surface_velocity;
                    }
                    if(player_1.xv<player_1.surface_velocity) {
                        player_1.xv+=.75;
                        if(player_1.xv>player_1.surface_velocity) player_1.xv=player_1.surface_velocity;
                    }
                }
                else player_1.xv-=(player_1.xv/90);

                //Update_Player_2()
                if(pressedkey[SDLK_d]&&player_2.xv>=0){if(player_2.xv<(3.5+player_2.surface_velocity)) player_2.xv+=.25;}
                else if(pressedkey[SDLK_d]&&player_2.xv<0) player_2.xv+=.5;
                else if(pressedkey[SDLK_a]&&player_2.xv<=0){if(player_2.xv>(-3.5+player_2.surface_velocity)) player_2.xv-=.25;}
                else if(pressedkey[SDLK_a]&&player_2.xv>0) player_2.xv-=.5;
                if((!pressedkey[SDLK_d] and !pressedkey[SDLK_a] and player_2.on_ground) or(fabs(player_2.xv)>(3.5+fabs(player_2.surface_velocity)) and player_2.on_ground)){
                    if(player_2.xv>player_2.surface_velocity) {
                        player_2.xv-=.75;
                        if(player_2.xv<player_2.surface_velocity) player_2.xv=player_2.surface_velocity;
                    }
                    if(player_2.xv<player_2.surface_velocity) {
                        player_2.xv+=.75;
                        if(player_2.xv>player_2.surface_velocity) player_2.xv=player_2.surface_velocity;
                    }
                }
                else if(!pressedkey[SDLK_d]&&!pressedkey[SDLK_a]) player_2.xv-=(player_2.xv/90);
            }


}


void Game::UpdateLevelEntities()
{

    white_button_is_pressed = false;
    black_button_is_pressed = false;
    grey_button_is_pressed = false;

    //First, we want to check to see if the buttons are pressed, so that any other entites which might depend on this information will have an up
    //to date picture of the world's collision geometry.
    int layer = FOREGROUND_LAYER;
    for(unsigned int i = 0; i<loaded_level.layers[layer].entities.size();i++){
            if(loaded_level.layers[layer].entities[i]->type=="ButtonType_White") //WHITE BUTTON TYPE
            {
                Entity_ButtonType_White* active_entity = reinterpret_cast<Entity_ButtonType_White*>(loaded_level.layers[layer].entities[i]);
                if((DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x,active_entity->bounds.y+20,active_entity->w,active_entity->h-20) or
                    DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x,active_entity->bounds.y+20,active_entity->w,active_entity->h-20)) and
                   !white_button_is_pressed) white_button_is_pressed = true;
            }
            else if(loaded_level.layers[layer].entities[i]->type=="ButtonType_Black") //BLACK BUTTON TYPE
            {
                Entity_ButtonType_Black* active_entity = reinterpret_cast<Entity_ButtonType_Black*>(loaded_level.layers[layer].entities[i]);
                if((DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x,active_entity->bounds.y+20,active_entity->w,active_entity->h-20) or
                   DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->w,active_entity->h-20)) and
                    !black_button_is_pressed) black_button_is_pressed = true;
            }
            else if(loaded_level.layers[layer].entities[i]->type=="ButtonType_Grey") //GREY BUTTON TYPE
            {
                Entity_ButtonType_Black* active_entity = reinterpret_cast<Entity_ButtonType_Black*>(loaded_level.layers[layer].entities[i]);
                if((DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x,active_entity->bounds.y+20,active_entity->w,active_entity->h-20) or
                   DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x,active_entity->bounds.y+20,active_entity->w,active_entity->h-20)) and
                    !grey_button_is_pressed) grey_button_is_pressed = true;
            }
    }
    //update all the other entities in the level
    for(unsigned int layer = 0; layer<4; layer++)
    {
        for(unsigned int i = 0; i<loaded_level.layers[layer].entities.size();i++){
            if(loaded_level.layers[layer].entities[i]->type=="GrassType"){ //GRASS TYPE
                Entity_GrassType* active_entity = reinterpret_cast<Entity_GrassType*>(loaded_level.layers[layer].entities[i]);
                active_entity->update(player_1,(double(camera.x)/20000)+0.4);
            }
            else if(loaded_level.layers[layer].entities[i]->type=="SignType"){ //SIGN TYPE
                Entity_SignType* active_entity = reinterpret_cast<Entity_SignType*>(loaded_level.layers[layer].entities[i]);
                active_entity->update(camera, loaded_level.layers[layer].parallax_modifier,player_1);
            }
            else if(loaded_level.layers[layer].entities[i]->type=="RespawnZoneType")
            {
                Entity_RespawnZoneType* active_entity = reinterpret_cast<Entity_RespawnZoneType*>(loaded_level.layers[layer].entities[i]);
                if((DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->w,active_entity->h) and !active_entity->red_player_activated)){
                   active_entity->red_player_activated = true;
                   player_1.respawn_zone = active_entity->bounds;
                   }

                   if((DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->w,active_entity->h)) and !active_entity->green_player_activated){
                       active_entity->green_player_activated = true;
                       player_2.respawn_zone = active_entity->bounds;
                   }
            }
            else if(loaded_level.layers[layer].entities[i]->type=="DeathZoneType" or loaded_level.layers[layer].entities[i]->type=="CollisionType_Death")
            {
                Entity_DeathZoneType* active_entity = reinterpret_cast<Entity_DeathZoneType*>(loaded_level.layers[layer].entities[i]);
                if((DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->w,active_entity->h))){

                    KillPlayer(1);
                   }

                   if((DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->w,active_entity->h))){
                       KillPlayer(2);
                   }
            }
            else if(loaded_level.layers[layer].entities[i]->type=="CollisionType_Death_Red")
            {
                Entity_CollisionType_Death_Red* active_entity = reinterpret_cast<Entity_CollisionType_Death_Red*>(loaded_level.layers[layer].entities[i]);
                if((DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->w,active_entity->h)))
                {
                    KillPlayer(1);
                }

            }
            else if(loaded_level.layers[layer].entities[i]->type=="CollisionType_Death_Green")
            {
                Entity_CollisionType_Death_Green* active_entity = reinterpret_cast<Entity_CollisionType_Death_Green*>(loaded_level.layers[layer].entities[i]);
                   if((DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->w,active_entity->h)))
                   {
                       KillPlayer(2);
                   }
            }
            else if(loaded_level.layers[layer].entities[i]->type=="DeathLaserType")
            {
                Entity_DeathLaserType* active_entity = reinterpret_cast<Entity_DeathLaserType*>(loaded_level.layers[layer].entities[i]);
                //In order to see if the player is below the laser, we just test against a collision box that starts from the emitter and extends an arbitrary amount downward.
                //This could be rewritten to just do a horizontal collision test, but this should be adequate.
                if(DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->bounds.w,10000)
                   or DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->bounds.w,10000))
                {
                    //One of the players has stepped beneath the laser, so we trigger it
                    active_entity->is_triggered = true;

                }
                //else active_entity->firing_delay_time = 0;
                //the delay time has been increased from 7 in the prototype to 16 in this version because of two reasons. The first reason, is that the framerate
                //in this version of the game has been doubled, and since the timer works in frame-space. The timer needs to be doubled as well. So we double from
                //7 to 14. However, in the original code, the if statement was tested before the trigger is activated, which meant that there was a single frame
                //delay between the activation and the timer reaching 1. In other words, on the end of the frame where a player triggered the laser, the timer
                //was zero. In this version though, the following if statement has been moved below the initial trigger test in the update loop.
                //This means that the timer will never end a frame at 0. Which in effect makes the timer go one frame
                //faster. So to avoid an off by one error and make the code more readable. I just moved the timer increment below the intersection test.
                //However, I can't simply use 15, because a one frame difference in the prototype is doubled, again. So it has to be 16: (7+1)*2
                if(active_entity->is_triggered)
                {
                    if(active_entity->firing_delay_time<=16) active_entity->firing_delay_time++;
                }

                if(active_entity->firing_delay_time==16) //if you are wondering why it is 8 instead of 7, please just read the previous comment
                {
                    active_entity->is_triggered = false;
                    active_entity->firing_delay_time = 0;
                    active_entity->laser_lifetime = 0;
                    //The delay on the firing activation has elapsed, so the laser fires.
                    active_entity->is_firing = true;
                }
                if(active_entity->is_firing)
                {
                    active_entity->laser_lifetime++;
                    if(active_entity->laser_lifetime>=30) active_entity->is_firing = false;
                    //we need to figure out how far to fire to. To do this, we can just naively test an arbitrarily tall bounding box extending downward from the
                    //laser emitter against all collision entities in the foreground layer. Then we just take the closest collision entity and stop the laser there.
                    //The laser itself is just stored as a integer length, and is reconstructed as a bounding box afterwards.

                    //we set the beam length to an arbitrary non-zero size, this way when we test to see if the beam would be shorter if collided, it will not always fail
                    active_entity->laser_beam_length = 10000;

                    for(unsigned int i = 0; i < loaded_level.layers[FOREGROUND_LAYER].entities.size();i++){ //loop through all the entities in the world
                        if(loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType" or
                           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_White_Button" and white_button_is_pressed) or
                           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Black_Button" and black_button_is_pressed) or
                           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Grey_Button" and !grey_button_is_pressed)){ //Is it a collision entity?
                               //if so, we need to reintepret cast just in case we need a collision specific info, maybe not though.
                                Entity_CollisionType* active_collision_box = reinterpret_cast<Entity_CollisionType*>(loaded_level.layers[FOREGROUND_LAYER].entities[i]);
                                if(DoBoxesIntersect(active_entity->bounds.x,active_entity->bounds.y,32,10000,
                                                     active_collision_box->bounds.x,active_collision_box->bounds.y,active_collision_box->bounds.w,active_collision_box->bounds.h)) //we test the arbitrarily tall bounding box against the collision entity
                                   {
                                       //they do, in fact intersect. So all we need to do is check if the distance from the top of the bounding box to the
                                       //emitter is shorter than any previously found
                                       int distance_to_collision_box = abs((active_collision_box->bounds.y)-(active_entity->bounds.y+active_entity->bounds.w));
                                       if(distance_to_collision_box < active_entity->laser_beam_length) active_entity->laser_beam_length = distance_to_collision_box;

                                   }
                        }
                    }
                    if((DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x+8,active_entity->bounds.y+32,16,active_entity->laser_beam_length)))
                    {
                        //the player collided with the firing laser, so we kill him
                        KillPlayer(1);
                    }
                    if((DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x+8,active_entity->bounds.y+32,16,active_entity->laser_beam_length)))
                    {
                        //the player collided with the firing laser, so we kill him
                        KillPlayer(2);
                    }
                }

            }
            else if(loaded_level.layers[layer].entities[i]->type=="ExitZoneType")
            {
                Entity_ExitZoneType* active_entity = reinterpret_cast<Entity_ExitZoneType*>(loaded_level.layers[layer].entities[i]);

                //Oh, I'm so fucking clever, right? So the way this works is that activation level is stored as a short integer
                //It works sorta like this. 1 == Player 1 is here, 2 == Player 2 is here, 3 == Both are here!
                //But we can just use some basic addition to do this. We add 1 if player 1 is here, and 2 if player 2 is here
                //this way it will always return properly for each frame what the status is of the exit zone;
                if(active_entity->activation_level<3)
                {
                    active_entity->activation_level = 0;
                    if((DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->bounds.w,active_entity->bounds.h)))
                    {
                        active_entity->activation_level+=1;
                    }
                    if((DoBoxesIntersectf(player_2.x,player_2.y,player_2.w,player_2.h,active_entity->bounds.x,active_entity->bounds.y,active_entity->bounds.w,active_entity->bounds.h)))
                    {
                        active_entity->activation_level+=2;
                    }

                    if(active_entity->activation_level==3) //If both players are on the exit_zone at the same time
                    {
                        if(active_entity->puzzle_barrier_index==-1)
                        {
                            GoToNextLevel();
                            break;
                        }
                        if(loaded_level.layers[layer].entities[active_entity->puzzle_barrier_index]->type!="CollisionType_Puzzle_Barrier")
                        {
                            //For some reason the level is not configured properly, and the index is not pointing to an acceptable entity

                            stringstream index;
                            index.str("");
                            index<<i;
                            string index_t = index.str();
                            fprintf(stderr, "The Entity_ExitZoneType at index %s is improperly configured and is not pointing to -1 or a Puzzle_Barrier entity \n", index_t.c_str() );
                            throw 1;
                        }
                        Entity_CollisionType_Puzzle_Barrier* barrier_to_open = reinterpret_cast<Entity_CollisionType_Puzzle_Barrier*>(loaded_level.layers[layer].entities[active_entity->puzzle_barrier_index]);
                        barrier_to_open->is_open = true;
                    }
                }



            }
            else if(loaded_level.layers[layer].entities[i]->type=="CollisionType_Puzzle_Barrier")
            {
                Entity_CollisionType_Puzzle_Barrier* active_entity = reinterpret_cast<Entity_CollisionType_Puzzle_Barrier*>(loaded_level.layers[layer].entities[i]);

                if(active_entity->is_open)
                {
                    if(player_1.x>active_entity->x+64 and player_2.x>active_entity->x+64)
                    {
                        active_entity->is_open = false;
                    }
                    else if(player_1.x<active_entity->x and player_2.x>active_entity->x and abs(player_1.x-player_2.x)>512)
                    {
                        if(loaded_level.layers[layer].entities[active_entity->tied_respawn_zone_index]->type!="RespawnZoneType"){
                                stringstream index;
                                index.str("");
                                index<<i;
                                string index_t = index.str();
                                fprintf(stderr, "The Puzzle_Barrier Entity at index %s is improperly configured and is not pointing to a Respawn Zone entity \n", index_t.c_str() );
                                throw 1;
                        }
                        player_1.respawn_zone = loaded_level.layers[layer].entities[active_entity->tied_respawn_zone_index]->bounds;
                        KillPlayer(1);
                    }
                    else if(player_2.x<active_entity->x and player_1.x>active_entity->x and abs(player_1.x-player_2.x)>512)
                    {
                        if(loaded_level.layers[layer].entities[active_entity->tied_respawn_zone_index]->type!="RespawnZoneType"){
                                stringstream index;
                                index.str("");
                                index<<i;
                                string index_t = index.str();
                                fprintf(stderr, "The Puzzle_Barrier Entity at index %s is improperly configured and is not pointing to a Respawn Zone entity \n", index_t.c_str() );
                                throw 1;
                        }
                        player_2.respawn_zone = loaded_level.layers[layer].entities[active_entity->tied_respawn_zone_index]->bounds;
                        KillPlayer(2);
                    }
                }
            }
        }
    }

    player_1.x+=player_1.xv;
    player_2.x+=player_2.xv;

    player_1.against_wall = false;
    player_2.against_wall = false;
    player_1.against_cieling = false;
    player_2.against_cieling = false;
    player_1.pushing_other_player = false;
    player_2.pushing_other_player = false;
    CheckXCollisionsAndResolve();

    //if(player_1.pushing_other_player and player_2.against_wall) player_1.xv = 0;
    //if(player_2.pushing_other_player and player_1.against_wall) player_2.xv = 0;

    if(player_1.on_ground == false) {
        if(player_1.jump_timer<10) player_1.jump_timer+=1;
    }
    else player_1.jump_timer = 0;

    if(player_2.on_ground == false) {
        if(player_2.jump_timer<10) player_2.jump_timer+=1;
    }
    else player_2.jump_timer = 0;

    if(player_2.on_ground) player_2.jump_num = 0;

    player_2.yv+=.25;
    if(player_2.yv>6) player_2.yv = 6;
    player_2.y+=player_2.yv;

    player_1.yv+=.25;
    if(player_1.yv>6) player_1.yv = 6;
    player_1.y+=player_1.yv;

    CheckYCollisionsAndResolve();
    white_button_was_pressed = white_button_is_pressed;
    grey_button_was_pressed = grey_button_is_pressed;
    black_button_was_pressed = black_button_is_pressed;
    AddWatch(player_2.on_ground);


}

void Game::CheckXCollisionsAndResolve(int recursions)
{
    if(recursions>=10) return;
    //first we check and resolve collisions between the players
    if(player_1.xv!=0)player_1.xuv = player_1.xv/fabs(player_1.xv);
    else player_1.xuv = 0;
    if(player_2.xv!=0)player_2.xuv = player_2.xv/fabs(player_2.xv);
    else player_2.xuv = 0;
    if(world_state>0){
    if(DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,player_2.x,player_2.y,player_2.w,player_2.h))
    {
        float penetration_amount = 0;
        if(player_1.x<=player_2.x) penetration_amount = fabs((player_1.x+player_1.w)-player_2.x);
        else penetration_amount = fabs((player_2.x+player_2.w)-(player_1.x));
        //penetration_amount+=.5;
        //if(player_2.against_wall)
        if(fabs(player_1.xv)>fabs(player_2.xv)){
                player_1.x+=(-1 * player_1.xuv)*penetration_amount;
                player_2.xv = player_1.xv;
                if(player_2.against_wall) player_1.xv = 0;
                player_1.pushing_other_player = true;

        }
        else if (fabs(player_1.xv)<fabs(player_2.xv)){
                player_2.x+= (-1*player_2.xuv)*penetration_amount;
                player_1.xv = player_2.xv;
                if(player_1.against_wall) player_2.xv = 0;
                player_2.pushing_other_player = true;
        }
        else {
            if(player_1.x<=player_2.x)
            {
                player_1.x-=(penetration_amount/2);
                player_2.x+=(penetration_amount/2);
            }
            else
            {
                player_1.x+=(penetration_amount/2);
                player_2.x-=(penetration_amount/2);
            }
            if(fabs(player_1.xv)==fabs(player_2.xv) and player_1.xv!=player_2.xv){
                player_1.xv = 0;
                player_2.xv = 0;
            }
        }
        CheckXCollisionsAndResolve(recursions+1);
    }
    }
    //then we resolve collisions for environment

        for(unsigned int i = 0; i < loaded_level.layers[FOREGROUND_LAYER].entities.size();i++){ //check if there is ground below us
        if(loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType" or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_White_Button" and white_button_is_pressed) or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Black_Button" and black_button_is_pressed) or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Grey_Button" and !grey_button_is_pressed) or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Puzzle_Barrier") or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Green") or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Red" ) or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Death_Red" ) or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Death_Green" ))
         { //COLLISION_BOX TYPE
               if(loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Puzzle_Barrier")
               {
                   Entity_CollisionType_Puzzle_Barrier* active_entity = reinterpret_cast<Entity_CollisionType_Puzzle_Barrier*>(loaded_level.layers[FOREGROUND_LAYER].entities[i]);
                   if(active_entity->is_open) continue; //If the collision type is the puzzle barrier and it is open, we ignore this entity and skip to the next one.
               }
            Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(loaded_level.layers[FOREGROUND_LAYER].entities[i]);
            //Check for collision with player_1
            if(DoBoxesIntersectf(active_entity->bounds.x,active_entity->bounds.y,active_entity->bounds.w,active_entity->bounds.h,player_1.x,player_1.y,player_1.w,player_1.h) and
               active_entity->type!="CollisionType_Green" and active_entity->type!="CollisionType_Death_Red")
            {
                if((loaded_level.layers[FOREGROUND_LAYER].entities[i]->type == "CollisionType_White_Button" and !white_button_was_pressed)
                   or (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type == "CollisionType_Black_Button" and !black_button_was_pressed)
                   or (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type == "CollisionType_Grey_Button" and grey_button_was_pressed)) KillPlayer(1);
                   else{
                        //float x_penetration;
                        if(fabs(active_entity->bounds.x-(player_1.x+player_1.w))<fabs((active_entity->bounds.x+active_entity->bounds.w)-player_1.x))
                        {
                            //if(fabs(active_entity->bounds.x-(player_1.x+player_1.w))<fabs(active_entity->bounds.y-(player_1.y+player_1.h))
                            //and fabs(active_entity->bounds.x-(player_1.x+player_1.w))<fabs((active_entity->bounds.y+active_entity->bounds.h)-player_1.y)){

                                   player_1.x+=(active_entity->bounds.x-(player_1.x+player_1.w));

                                    player_1.xv=0;
                                    player_1.against_wall = true;
                                    CheckXCollisionsAndResolve(recursions+1);
                               //}
                        }
                        else //if(fabs((active_entity->bounds.x+active_entity->bounds.w)-player_1.x)<fabs(active_entity->bounds.y-(player_1.y+player_1.h))
                            //and fabs((active_entity->bounds.x+active_entity->bounds.w)-player_1.x)<fabs((active_entity->bounds.y+active_entity->bounds.h)-player_1.y))
                            {
                                player_1.x+=((active_entity->bounds.x+active_entity->bounds.w)-player_1.x);
                                player_1.xv=0;
                                player_1.against_wall = true;
                                CheckXCollisionsAndResolve(recursions+1);
                            }
                   }
            }
            //Check for collision with player_2
            if(DoBoxesIntersectf(active_entity->bounds.x,active_entity->bounds.y,active_entity->bounds.w,active_entity->bounds.h,player_2.x,player_2.y,player_2.w,player_2.h) and
               active_entity->type!="CollisionType_Red" and active_entity->type!="CollisionType_Death_Green")
            {
                    if((loaded_level.layers[FOREGROUND_LAYER].entities[i]->type == "CollisionType_White_Button" and !white_button_was_pressed)
                   or (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type == "CollisionType_Black_Button" and !black_button_was_pressed)
                   or (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type == "CollisionType_Grey_Button" and grey_button_was_pressed)) KillPlayer(2);
                   else{
                    //float x_penetration;
                    if(fabs(active_entity->bounds.x-(player_2.x+player_2.w))<fabs((active_entity->bounds.x+active_entity->bounds.w)-player_2.x))
                    {
                        player_2.x+=(active_entity->bounds.x-(player_2.x+player_2.w));
                    }
                    else player_2.x+=((active_entity->bounds.x+active_entity->bounds.w)-player_2.x);

                    player_2.xv=0;
                    player_2.against_wall = true;
                    CheckXCollisionsAndResolve(recursions+1);
                }
            }
        }
    }
}


void Game::CheckYCollisionsAndResolve(int recursions)
{
    bool bump_hack; //This is just a hack to fix a bug where a player blocked by red/green collision blocks is
                    //carrying another player and jumps, causing the other player to jump,
                    //but shouldn't because the carrier is boxed in.
    bump_hack = false;
    if(recursions>=10) return;
    //first we check and resolve collisions between the players
    if(player_1.yv!=0)player_1.yuv = player_1.yv/fabs(player_1.yv);
    else player_1.yuv = 0;
    if(player_2.yv!=0)player_2.yuv = player_2.yv/fabs(player_2.yv);
    else player_2.yuv = 0;
    if(world_state>0){
    if(DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,player_2.x,player_2.y,player_2.w,player_2.h))
    {
        float penetration_amount;
        if(player_1.y<player_2.y) {
            penetration_amount = fabs((player_1.y+player_1.h)-player_2.y);
            if(player_1.against_cieling){
                player_2.y+=penetration_amount;
                player_2.yv = 0;
            }
            else player_1.y-=penetration_amount;
            player_1.yv = player_2.yv;
            bump_hack = true;
            player_1.on_ground = true;
            //if(player_2.on_ground) player_1.yv = 0;
        }
        else if(player_1.y>player_2.y)
        {
            penetration_amount = fabs((player_2.y+player_2.h)-(player_1.y));
            if(player_2.against_cieling){
                player_1.y+=penetration_amount;
                player_1.yv = 0;
            }
            else player_2.y-=penetration_amount;
            player_2.on_ground = true;
            player_2.yv = player_1.yv;
            bump_hack = true;
            //if(player_1.on_ground) player_2.yv = 0;
        }
    }
    else if(DoBoxesIntersectf(player_1.x,player_1.y+2,player_1.w,player_1.h,player_2.x,player_2.y,player_2.w,player_2.h) or
            DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,player_2.x,player_2.y+2,player_2.w,player_2.h))
    {
        if(world_state>=2){
            if(player_1.y<player_2.y) player_1.surface_velocity = player_2.xv;
            else player_2.surface_velocity = player_1.xv;
        }

    }
    }

    if(DoBoxesIntersectf(player_1.x,player_1.y+2,player_1.w,player_1.h,player_2.x,player_2.y,player_2.w,player_2.h) and world_state >= 1){
        player_2.surface_velocity = 0;
        if(player_2.yv) player_2.on_ground = false;
    }
    else if(DoBoxesIntersectf(player_1.x,player_1.y,player_1.w,player_1.h,player_2.x,player_2.y+2,player_2.w,player_2.h) and world_state >= 1){
        player_1.surface_velocity = 0;
        if(player_1.yv) player_1.on_ground = false;
    }
    else{
        player_1.surface_velocity = 0;
        player_2.surface_velocity = 0;
        if(player_1.yv) player_1.on_ground = false;
        if(player_2.yv) player_2.on_ground = false;
        }


    //then we resolve collisions for environment

        for(unsigned int i = 0; i < loaded_level.layers[FOREGROUND_LAYER].entities.size();i++){ //check if there is ground below us
        if((loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType") or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_White_Button" and white_button_is_pressed) or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Black_Button" and black_button_is_pressed) or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Grey_Button" and !grey_button_is_pressed) or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Red") or
            (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Green")or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Death_Red") or
           (loaded_level.layers[FOREGROUND_LAYER].entities[i]->type=="CollisionType_Death_Green" ))
           {
            Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(loaded_level.layers[FOREGROUND_LAYER].entities[i]);
            //Check for collision with player_1
            if(DoBoxesIntersectf(active_entity->bounds.x,active_entity->bounds.y,active_entity->bounds.w,active_entity->bounds.h,player_1.x,player_1.y,player_1.w,player_1.h) and
               active_entity->type!="CollisionType_Green" and active_entity->type!="CollisionType_Death_Red")
            {
                        //float x_penetration;
                        if(fabs(active_entity->bounds.y-(player_1.y+player_1.h))<fabs((active_entity->bounds.y+active_entity->bounds.h)-player_1.y))
                        {
                            player_1.y+=(active_entity->bounds.y-(player_1.y+player_1.h));
                            player_1.on_ground = true;
                        }
                        else {
                            player_1.y+=((active_entity->bounds.y+active_entity->bounds.h)-player_1.y);
                            player_1.against_cieling = true;
                            if(bump_hack)
                            {
                                player_1.on_ground = true;
                                player_2.on_ground = true;
                                player_2.y-=player_2.yv;
                                player_2.yv = 0;
                            }
                        }
                        player_1.yv=0;
                        CheckYCollisionsAndResolve(recursions+1);

            }
            //Check for collision with player_2
            if(DoBoxesIntersectf(active_entity->bounds.x,active_entity->bounds.y,active_entity->bounds.w,active_entity->bounds.h,player_2.x,player_2.y,player_2.w,player_2.h)
               and active_entity->type!="CollisionType_Red" and active_entity->type!="CollisionType_Death_Green")
            {
                        //float x_penetration;
                        if(fabs(active_entity->bounds.y-(player_2.y+player_2.h))<fabs((active_entity->bounds.y+active_entity->bounds.h)-player_2.y))
                        {
                            player_2.y+=(active_entity->bounds.y-(player_2.y+player_2.h));
                            player_2.on_ground = true;
                        }
                        else {
                            player_2.y+=((active_entity->bounds.y+active_entity->bounds.h)-player_2.y);
                            player_2.against_cieling = true;
                            if(bump_hack)
                            {
                                player_1.y-=player_1.yv;
                                player_1.yv = 0;
                                player_2.on_ground = true;
                                player_1.on_ground = true;
                            }
                        }
                        player_2.yv=0;
                        CheckYCollisionsAndResolve(recursions+1);
                }

        }
    }
}

void Game::KillPlayer(int player_number)
{
    if(player_number==1)
    {
        player_1.x = player_1.respawn_zone.x+16;
        player_1.y = player_1.respawn_zone.y+player_1.respawn_zone.h-player_1.h;
    }
    else
    {
        player_2.x = (player_2.respawn_zone.x+player_2.respawn_zone.w)-player_2.w-16;
        player_2.y = player_2.respawn_zone.y+player_2.respawn_zone.h-player_2.h;
    }
}


point_2d Game::CorrectForSphereCollision(int x, int y, int radius, int num_times_checked)
{
    point_2d corrected_position(x,y);
    if(num_times_checked<6){
    for(unsigned int i = 0; i <loaded_level.collision_series.size();i++)
        {
            // we want to find the closest collision series to shorten collision detection
            vec_2d closest_segment_in_series = loaded_level.collision_series[i].return_closest_segment(point_2d(x,y));
            vec_2d vector_from_point_to_closest_segment = find_shortest_vector_from_point_to_line(point_2d(x,y),closest_segment_in_series);
            if(vector_from_point_to_closest_segment.len<radius)
            {
                corrected_position.x= (int)(x+((radius-vector_from_point_to_closest_segment.len)*vector_from_point_to_closest_segment.dx));
                corrected_position.y= (int)(y+((radius-vector_from_point_to_closest_segment.len)*vector_from_point_to_closest_segment.dy));
                CorrectForSphereCollision(corrected_position.x,corrected_position.y,radius,num_times_checked+1);
            }
        }
        return corrected_position;
    }
    else return corrected_position;

}

void Game::FindVectorBelowPlayer()
{
    /*player_1.are_vectors_below = false;
    if(loaded_level.collision_series.size()!=0){
        //so we can compare values between different collision series
        int closest_below_vector_y = -1;
        for(unsigned int i = 0; i <loaded_level.collision_series.size();i++)
        {
            //we also need to find the segment below the player_1 to know how close he is to the ground
            vec_2d segment_below_in_series;
            segment_below_in_series = loaded_level.collision_series[i].return_segment_directly_below(point_2d(player_1.x,player_1.y));
            if(segment_below_in_series.len>0) //if the length of the segment below in this series is zero, there isn't one below
            {
                player_1.are_vectors_below = true;
                int this_series_below_vector_y = segment_below_in_series.find_y_at(player_1.x);
                if(this_series_below_vector_y>player_1.y&&this_series_below_vector_y<closest_below_vector_y||closest_below_vector_y==-1)
                {
                    closest_below_vector_y = this_series_below_vector_y;
                    player_1.vector_below = segment_below_in_series;
                }
            }
        }
    }*/
}

void Game::FindClosestVectorToPlayer()
{
   /* player_1.closest_collision_series_index = -1;
    if(loaded_level.collision_series.size()!=0){
        //so we can compare values between different collision series
        int closest_series_distance = -1;
        for(unsigned int i = 0; i <loaded_level.collision_series.size();i++)
        {
            // we want to find the closest collision series to shorten collision detection
            vec_2d closest_segment_in_series = loaded_level.collision_series[i].return_closest_segment(point_2d(player_1.x,player_1.y));
            vec_2d vector_from_player_to_closest_segment = find_shortest_vector_from_point_to_line(point_2d(player_1.x,player_1.y),closest_segment_in_series);
            if(vector_from_player_to_closest_segment.len<closest_series_distance||closest_series_distance==-1)
            {
                player_1.closest_vector = closest_segment_in_series;
                closest_series_distance = (int)vector_from_player_to_closest_segment.len;
                player_1.closest_collision_series_index = i;
            }
        }
    }*/
}


void Game::draw()
{
     glClear(GL_COLOR_BUFFER_BIT);
     if(game_state==GAMESTATE_INTRO)
     {
         glColor4f(1.0,1.0,1.0,intro_image_alpha);
         intro_image.draw(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0,1.0);
         glColor4f(1.0,1.0,1.0,1.0);
     }
     if(game_state==GAMESTATE_IN_GAME){
         DrawGLRectSolid(square2,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,loaded_level.background_r,loaded_level.background_g,loaded_level.background_b,1.0);
            //draw the level

            //camera.rot+=0;
            //camera.zoom=0;

            float zoom_offset_x = (SCREEN_WIDTH/2-((SCREEN_WIDTH*camera.zoom)/2))/camera.zoom;
            float zoom_offset_y = (SCREEN_HEIGHT/2-((SCREEN_HEIGHT*camera.zoom)/2))/camera.zoom;

            for(int layer=7;layer>-1;layer--)
            {
                if(layer==1) //draw the players inside of the layers;
                {
                if(debug_mode){
                    glLoadIdentity();


                    glScalef(camera.zoom,camera.zoom,0);
                    glTranslatef((-camera.x*loaded_level.layers[layer].parallax_modifier),(-camera.y*loaded_level.layers[layer].parallax_modifier),0);
                    glTranslatef(zoom_offset_x,zoom_offset_y,0);
                    glTranslatef(-SCREEN_WIDTH/2,-SCREEN_HEIGHT/2,0);
                    glRotatef(camera.rot,0.0,0.0,1.0);
                    glTranslatef(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0);

                    DrawGLRectSolidFromCurrentMatrix(square2,player_1.x,player_1.y,player_1.w,player_1.h,1.0,0.0,0.0,1.0);
                    DrawGLRectSolidFromCurrentMatrix(square2,player_2.x,player_2.y,player_2.w,player_2.h,0.0,1.0,0.0,1.0);
                }
                else{
                    player_1.standing_frame.draw(player_1.x+19-camera.x,player_1.y+16-camera.y,player_1.rotation,1.0,true);
                    player_2.standing_frame.draw(player_2.x+16-camera.x,player_2.y+25-camera.y,player_2.rotation,1.0,true);
                }
                }
                if(culled_drawing_is_used)
                {

                    //Check if a static could possibly be visible, if not, we cull it (make sure that this is faster than not doing it before leaving implemented. May only be useful if the static count is very high.
                    vector <Static*> visible_statics_for_this_layer;
                    for(int i=0;(unsigned int)(i)<loaded_level.layers[layer].statics.size();i++)
                    {
                        Static* active_static = reinterpret_cast<Static*>(loaded_level.layers[layer].statics[i]);
                        if(layer==7){visible_statics_for_this_layer.push_back(active_static);}
                        else{
                            int max_width = (int)sqrt(active_static->w*active_static->w + active_static->h*active_static->h);
                            //int max_width = sqrt(pow(active_static->w,2) + pow(active_static->h,2));
                            float temp_parallax_mod = loaded_level.layers[layer].parallax_modifier;
                            //if(layer==0) temp_parallax_mod = 1;
                            float parallax_cam_x = camera.x*(temp_parallax_mod)*(active_static->parallax_offset);
                            float parallax_cam_y = camera.y*(temp_parallax_mod)*(active_static->parallax_offset);
                            float parallax_cam_width =  (camera.width/temp_parallax_mod)*active_static->parallax_offset;
                            float parallax_cam_height = (camera.height/temp_parallax_mod)*active_static->parallax_offset;
                            if(DoBoxesIntersectf(active_static->x-max_width/2,active_static->y-max_width/2,max_width,max_width,
                                                 parallax_cam_x, parallax_cam_y, parallax_cam_width, parallax_cam_height)) visible_statics_for_this_layer.push_back(active_static);

                        }
                    }

                    for(int i=0;(unsigned int)(i)<visible_statics_for_this_layer.size();i++)
                    {
                        glLoadIdentity();
                        //adjust for the camera
                        Static* active_static = visible_statics_for_this_layer[i];

                        glScalef(camera.zoom,camera.zoom,0);
                        //glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                        glTranslatef(-camera.x*(loaded_level.layers[layer].parallax_modifier)*(active_static->parallax_offset),-camera.y*(loaded_level.layers[layer].parallax_modifier)*(active_static->parallax_offset),0);
                        glTranslatef(zoom_offset_x,zoom_offset_y,0);
                        glRotatef(camera.rot,0.0,0.0,1.0);
                        glPushMatrix();
                        glTranslatef(active_static->x,active_static->y,0);
                        glRotatef( active_static->rotation, 0.0, 0.0, 1.0 );
                        if(active_static->is_flipped_horizontally) glScalef(-(active_static->w),active_static->h,0);
                        else glScalef(active_static->w,active_static->h,0);

                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, loaded_level.textures[active_static->texture_index].GLtexture);
                            glUseProgram(simple_glsl_program);

                            glUniform4f(mult_color,active_static->r_mult, active_static->g_mult, active_static->b_mult, active_static->a_mult);
                            glUniform4f(add_color,active_static->r_add, active_static->g_add, active_static->b_add, active_static->a_add);
                            glUniform3f(blend_color,active_static->r_blend, active_static->g_blend, active_static->b_blend);
                            glUniform1f(blend_density,active_static->a_blend);

                            glUniform3f(hsl,active_static->hue, active_static->saturation, active_static->lightness);
                            glUniform1f(brightness, active_static->brightness);
                            glUniform1f(contrast, active_static->contrast);

                            glUniform1f(colorize, active_static->is_using_colorize_hue);

                            if(active_static->is_using_additive_tinting)glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
                            else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                            glBegin(GL_QUADS);
                                glTexCoord2f(active_static->u,active_static->v); glVertex2f(-.5,-.5);
                                glTexCoord2f(active_static->u2,active_static->v); glVertex2f(.5,-.5);
                                glTexCoord2f(active_static->u2,active_static->v2); glVertex2f(.5,.5);
                                glTexCoord2f(active_static->u,active_static->v2); glVertex2f(-.5,.5);
                            glEnd();

                            glUseProgram(0);


                        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        glPopMatrix();

                    }
                    glColor4f(1.0,1.0,1.0,1.0);

                    //do some more culling, this time for the entities
                    vector <Entity*> visible_entities_for_this_layer;
                    for(int i=0;(unsigned int)(i)<loaded_level.layers[layer].entities.size();i++)
                    {
                            Entity* active_entity = reinterpret_cast<Entity*>(loaded_level.layers[layer].entities[i]);
                            if(layer==7){visible_entities_for_this_layer.push_back(active_entity);}
                            else{
                            int max_width = sqrt(active_entity->w*active_entity->w + active_entity->h*active_entity->h);
                            //int max_width = sqrt(pow(active_entity->w,2) + pow(active_entity->h,2));
                            float temp_parallax_mod = loaded_level.layers[layer].parallax_modifier;
                            //if(layer==0) temp_parallax_mod = 1;
                            if(DoBoxesIntersectf(active_entity->x-max_width/2,active_entity->y-max_width/2,max_width,max_width,camera.x*temp_parallax_mod,
                                                      camera.y*temp_parallax_mod,camera.width/temp_parallax_mod,
                                                      camera.height/temp_parallax_mod)) visible_entities_for_this_layer.push_back(active_entity);
                        }
                    }


                    for(int i=0;(unsigned int)(i)<visible_entities_for_this_layer.size();i++)
                    {
                        glLoadIdentity();
                        glColor4f(1.0,1.0,1.0,1.0);
                        if((visible_entities_for_this_layer[i]->type=="CollisionType") or (visible_entities_for_this_layer[i]->type=="CollisionType_White_Button")
                           or (visible_entities_for_this_layer[i]->type=="CollisionType_Black_Button") or (visible_entities_for_this_layer[i]->type=="CollisionType_Grey_Button")
                           or (visible_entities_for_this_layer[i]->type=="CollisionType_Red") or (visible_entities_for_this_layer[i]->type=="CollisionType_Green")
                           or (visible_entities_for_this_layer[i]->type=="CollisionType_Puzzle_Barrier"))
                           {
                            Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(visible_entities_for_this_layer[i]);

                            glScalef(camera.zoom,camera.zoom,0);
                            //glTranslatef((SCREEN_WIDTH/2),(SCREEN_HEIGHT/2),0);
                            glRotatef(camera.rot,0.0,0.0,1.0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);


                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            //glDisable(GL_TEXTURE_2D);

                            if(debug_mode){
                                if(visible_entities_for_this_layer[i]->type == "CollisionType") glColor4f(0.0,0.0,1.0,1.0);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_White_Button" and white_button_is_pressed) glColor4f(1.0,1.0,1.0,1.0);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_White_Button" and !white_button_is_pressed) glColor4f(1.0,1.0,1.0,0.2);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_Black_Button" and black_button_is_pressed) glColor4f(0.2,0.2,0.2,1.0);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_Black_Button" and !black_button_is_pressed) glColor4f(0.2,0.2,0.2,0.2);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_Grey_Button" and !grey_button_is_pressed) glColor4f(0.5,0.5,0.5,1.0);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_Grey_Button" and grey_button_is_pressed) glColor4f(0.5,0.5,0.5,0.2);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_Red") glColor4f(1.0,0.0,0.0,1.0);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_Green") glColor4f(0.0,1.0,0.0,1.0);
                                else if(visible_entities_for_this_layer[i]->type == "CollisionType_Puzzle_Barrier") {
                                    Entity_CollisionType_Puzzle_Barrier* active_entity = (reinterpret_cast<Entity_CollisionType_Puzzle_Barrier*>(visible_entities_for_this_layer[i]));
                                    if(active_entity->is_open) glColor4f(0.0,1.0,1.0,0.2);
                                    else glColor4f(0.0,1.0,1.0,1.0);
                                }

                                glBindTexture(GL_TEXTURE_2D, collision_box.GLtexture);
                                glCallList(square);
                            }
                            glColor4f(1.0,1.0,1.0,1.0);
                            glPopMatrix();
                        }
                        else if(visible_entities_for_this_layer[i]->type=="CollisionType_Death"){
                            Entity_CollisionType_Death* active_entity = reinterpret_cast<Entity_CollisionType_Death*>(visible_entities_for_this_layer[i]);
                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);
                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            glColor4f(1.0,1.0,1.0,1.0);
                            glBindTexture(GL_TEXTURE_2D, death_zone.GLtexture);
                            glCallList(square);
                            glPopMatrix();
                        }
                        else if(visible_entities_for_this_layer[i]->type=="CollisionType_Death_Red"){
                            Entity_CollisionType_Death_Green* active_entity = reinterpret_cast<Entity_CollisionType_Death_Green*>(visible_entities_for_this_layer[i]);
                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);
                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, death_zone.GLtexture);
                            glColor4f(1.0,0.0,0.0,1.0);
                            glCallList(square);
                            glPopMatrix();
                        }
                        else if(visible_entities_for_this_layer[i]->type=="CollisionType_Death_Green"){
                            Entity_CollisionType_Death_Green* active_entity = reinterpret_cast<Entity_CollisionType_Death_Green*>(visible_entities_for_this_layer[i]);
                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);
                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, death_zone.GLtexture);
                            glColor4f(0.0,1.0,0.0,1.0);
                            glCallList(square);
                            glPopMatrix();
                        }
                        else if(visible_entities_for_this_layer[i]->type=="ButtonType_White"){ //WHITE BUTTON
                            //glColor4f(0.0,0.0,0.0,1.0);
                            Entity_ButtonType_White* active_entity = reinterpret_cast<Entity_ButtonType_White*>(visible_entities_for_this_layer[i]);

                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);

                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            if(white_button_is_pressed) glBindTexture(GL_TEXTURE_2D, button_white_down.GLtexture);
                            else glBindTexture(GL_TEXTURE_2D, button_white_up.GLtexture);
                            glCallList(square);
                            glPopMatrix();
                        }
                        else if(visible_entities_for_this_layer[i]->type=="ButtonType_Black"){ //BLACK BUTTON
                            //glColor4f(0.0,0.0,0.0,1.0);
                            Entity_ButtonType_Black* active_entity = reinterpret_cast<Entity_ButtonType_Black*>(visible_entities_for_this_layer[i]);

                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);

                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            if(black_button_is_pressed) glBindTexture(GL_TEXTURE_2D, button_black_down.GLtexture);
                            else glBindTexture(GL_TEXTURE_2D, button_black_up.GLtexture);
                            glCallList(square);
                            glPopMatrix();
                        }
                        else if(visible_entities_for_this_layer[i]->type=="ButtonType_Grey"){ //GREY BUTTON
                            //glColor4f(0.0,0.0,0.0,1.0);
                            Entity_ButtonType_Grey* active_entity = reinterpret_cast<Entity_ButtonType_Grey*>(visible_entities_for_this_layer[i]);
                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);

                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            if(grey_button_is_pressed) glBindTexture(GL_TEXTURE_2D, button_grey_down.GLtexture);
                            else glBindTexture(GL_TEXTURE_2D, button_grey_up.GLtexture);
                            glCallList(square);
                            glPopMatrix();
                        }
                        else if(visible_entities_for_this_layer[i]->type=="ExitZoneType"){
                            //glColor4f(0.0,0.0,0.0,1.0);
                            Entity_ExitZoneType* active_entity = reinterpret_cast<Entity_ExitZoneType*>(visible_entities_for_this_layer[i]);
                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);
                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, exit_zone.GLtexture);
                            if(active_entity->activation_level==0) glColor4f(1.0,1.0,1.0,1.0);
                            else if(active_entity->activation_level==1) glColor4f(1.0,0.0,0.0,1.0);
                            else if(active_entity->activation_level==2) glColor4f(0.0,1.0,0.0,1.0);
                            else if(active_entity->activation_level==3) glColor4f(1.0,1.0,0.0,1.0);
                            glCallList(square);
                            glPopMatrix();

                        }
                        else if(visible_entities_for_this_layer[i]->type=="DeathLaserType"){ //GREY BUTTON
                            //glColor4f(0.0,0.0,0.0,1.0);
                            Entity_DeathLaserType* active_entity = reinterpret_cast<Entity_DeathLaserType*>(visible_entities_for_this_layer[i]);
                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);
                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, death_laser_emitter.GLtexture);
                            if(active_entity->is_firing)
                            {
                                glColor4f(1.0,0.0,0.0,1.0);
                            }
                            glCallList(square);
                            glPopMatrix();
                            if(active_entity->is_firing)
                            {
                                glTranslatef(active_entity->bounds.x,active_entity->bounds.y+32,0);
                                glScalef(32,active_entity->laser_beam_length,0);
                                glBindTexture(GL_TEXTURE_2D, death_laser_beam.GLtexture);
                                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
                                glBegin(GL_QUADS);
                                    glTexCoord2i(0,active_entity->laser_lifetime); glVertex2f(0,0);
                                    glTexCoord2i(1,active_entity->laser_lifetime);  glVertex2f(1, 0);
                                    glTexCoord2i(1,active_entity->laser_beam_length/32);  glVertex2f(1, 1);
                                    glTexCoord2i(0,active_entity->laser_beam_length/32);  glVertex2f(0, 1);
                                glEnd();
                                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
                                 //DrawGLRectSolidFromCurrentMatrix(active_entity->bounds.x,active_entity->bounds.y+32,32,active_entity->laser_beam_length, 1.0, 0.0, 0.0, 1.0);
                            }
                            glColor4f(1.0,1.0,1.0,1.0);
                        }
                        else if(visible_entities_for_this_layer[i]->type=="GrassType"){ //GRASS TYPE
                            //glColor4f(0.0,0.0,0.0,1.0);
                            Entity_GrassType* active_entity = reinterpret_cast<Entity_GrassType*>(visible_entities_for_this_layer[i]);
                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);
                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, loaded_level.textures[active_entity->texture_index].GLtexture);
                            glCallList(square);
                            glPopMatrix();
                        }
                        else if(visible_entities_for_this_layer[i]->type=="SignType"){ //SIGN TYPE
                            Entity_SignType* active_entity = reinterpret_cast<Entity_SignType*>(visible_entities_for_this_layer[i]);
                            glColor4f(1.0,1.0,1.0,active_entity->fade_alpha);
                            glScalef(camera.zoom,camera.zoom,0);
                            glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                            glTranslatef(zoom_offset_x,zoom_offset_y,0);
                            glPushMatrix();
                            glTranslatef(active_entity->x,active_entity->y,0);
                            glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                            glScalef(active_entity->w,active_entity->h,0);
                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, loaded_level.textures[active_entity->texture_index].GLtexture);
                            glCallList(square);
                            glPopMatrix();
                        }

                    }
                    glColor4f(1.0,1.0,1.0,1.0);
                }
                else{
                 //OLD AND OBSOLETE? METHOD FOR DRAWING THE LEVEL (DOESN'T DO ANY CULLING WHATSOEVER)

                for(int i=0;(unsigned int)(i)<loaded_level.layers[layer].statics.size();i++)
                {
                    glLoadIdentity();
                    //adjust for the camera
                    Static* active_static = reinterpret_cast<Static*>(loaded_level.layers[layer].statics[i]);
                    glScalef(camera.zoom,camera.zoom,0);
                    glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                    glPushMatrix();
                    glTranslatef(active_static->x,active_static->y,0);
                    glRotatef( active_static->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_static->w,active_static->h,0);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, loaded_level.textures[active_static->texture_index].GLtexture);
                    glCallList(square);
                    glPopMatrix();
                    //draws the boxes used to determine whether an object is inside the screen or not.
                    //int max_width = sqrt(pow(active_static->w, 2) + pow(active_static->h, 2));
                    //if(DoBoxesIntersectf(active_static->x-max_width/2,active_static->y-max_width/2,max_width,max_width,camera.x,camera.y,camera.width,camera.height)) DrawGLRect(1,(active_static->x-max_width/2)-camera.x,(active_static->y-max_width/2)-camera.y,max_width,max_width,0.0,1.0,0.0,1.0);
                    //else DrawGLRect(1,(active_static->x-max_width/2)-camera.x,(active_static->y-max_width/2)-camera.y,max_width,max_width,1.0,0.0,0.0,1.0);
                }

                glColor4f(1.0,1.0,1.0,1.0);
                //Entities need to be drawn differently based on type
                for(int i=0;(unsigned int)(i)<loaded_level.layers[layer].entities.size();i++)
                {
                    glLoadIdentity();
                    if(loaded_level.layers[layer].entities[i]->type=="CollisionType" and debug_mode){ //COLLISION_TYPE
                        Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(loaded_level.layers[layer].entities[i]);
                        //glScalef(camera.zoom,camera.zoom,0);
                        glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                        glPushMatrix();
                        glTranslatef(active_entity->x,active_entity->y,0);
                        glScalef(active_entity->w,active_entity->h,0);
                        glDisable(GL_TEXTURE_2D);
                        glColor4f(0.0,0.0,0.0,1.0);
                        //glColor4f(1.0,0.0,1.0,1.0);
                        //glBindTexture(GL_TEXTURE_2D, collision_box.GLtexture);
                        glCallList(square);
                        glPopMatrix();
                }
                    if(loaded_level.layers[layer].entities[i]->type=="GrassType"){
                        //glColor4f(0.0,0.0,0.0,1.0);
                        Entity_GrassType* active_entity = reinterpret_cast<Entity_GrassType*>(loaded_level.layers[layer].entities[i]);
                        glScalef(camera.zoom,camera.zoom,0);
                        glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                        glPushMatrix();
                        glTranslatef(active_entity->x,active_entity->y,0);
                        glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                        glScalef(active_entity->w,active_entity->h,0);
                        glEnable(GL_TEXTURE_2D);
                        glBindTexture(GL_TEXTURE_2D, loaded_level.textures[active_entity->texture_index].GLtexture);
                        glCallList(square);
                        glPopMatrix();
                    }
                    if(loaded_level.layers[layer].entities[i]->type=="SignType"){
                        Entity_SignType* active_entity = reinterpret_cast<Entity_SignType*>(loaded_level.layers[layer].entities[i]);
                        glColor4f(1.0,1.0,1.0,active_entity->fade_alpha);
                        glScalef(camera.zoom,camera.zoom,0);
                        glTranslatef(-camera.x*loaded_level.layers[layer].parallax_modifier,-camera.y*loaded_level.layers[layer].parallax_modifier,0);
                        glPushMatrix();
                        glTranslatef(active_entity->x,active_entity->y,0);
                        glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                        glScalef(active_entity->w,active_entity->h,0);
                        glEnable(GL_TEXTURE_2D);
                        glBindTexture(GL_TEXTURE_2D, loaded_level.textures[active_entity->texture_index].GLtexture);
                        glCallList(square);
                        glPopMatrix();
                    }
                }
                glColor4f(1.0,1.0,1.0,1.0);

            }
            }
                //glColor4f(0.0,0.0,0.0,1.0);



                //glColor4f(1.0,1.0,1.0,1.0);
                glLoadIdentity();
                if(intro_image_alpha>0)
                 {
                     DrawGLRectSolid(square2,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,1.0,1.0,1.0,intro_image_alpha);
                 }

            //Draw more debug info
            if(debug_mode){

                Text_Outlined(font,0,0,"DEBUG MODE",white,black);
                stringstream debug_text;
                debug_text.str("");
                debug_text<<"player_1: x:"<<player_1.x<<" y:"<<player_1.y<<" xv:"<<player_1.xv<<" yv:"<<player_1.yv;
                Text_Outlined(font,0,FONT_SIZE,debug_text.str(),white,black);
                debug_text.str("");
                debug_text<<"          pushing_other_player:"<<player_1.pushing_other_player<<" was_pushing_other_player:"<<player_1.was_pushing_other_player;
                Text_Outlined(font,0,FONT_SIZE*2,debug_text.str(),white,black);
                debug_text.str("");
                debug_text<<"player_2: x:"<<player_2.x<<" y:"<<player_2.y<<" xv:"<<player_2.xv<<" yv:"<<player_2.yv;
                Text_Outlined(font,0,FONT_SIZE*3,debug_text.str(),white,black);
                debug_text.str("");
                debug_text<<"          pushing_other_player:"<<player_2.pushing_other_player<<" was_pushing_other_player:"<<player_2.was_pushing_other_player;
                Text_Outlined(font,0,FONT_SIZE*4,debug_text.str(),white,black);
                debug_text.str("");
                debug_text<<"player_1.on_ground"<<player_1.on_ground<<"player_2.on_ground"<<player_2.on_ground<<"player_2.jump_timer"<<player_2.jump_timer;
                Text_Outlined(font,0,FONT_SIZE*5,debug_text.str(),white,black);
                debug_text.str("");
                debug_text<<"Desktop.w"<<DESKTOP_RES_WIDTH<<" h"<<DESKTOP_RES_HEIGHT;
                Text_Outlined(font,0,FONT_SIZE*6,debug_text.str(),white,black);
                debug_text.str("");
                debug_text<<"player_1.xuv = "<<player_1.xuv;
                Text_Outlined(font,0,FONT_SIZE*7,debug_text.str(),white,black);
                debug_text.str("");
                debug_text<<"player_2.xuv = "<<player_2.xuv;
                Text_Outlined(font,200,FONT_SIZE*7,debug_text.str(),white,black);
                debug_text.str("");
                debug_text<<"watched_var = "<<watched_var;
                Text_Outlined(font,0,FONT_SIZE*8,debug_text.str(),white,black);

            }
    }
            //Draw the FPS Display
            if(fps_is_shown){
                stringstream fps_text;
                fps_text.str("");
                fps_text<<"FPS: "<<current_frames_per_second;
                Text_Outlined(font,SCREEN_WIDTH-FONT_SIZE*5,4,fps_text.str(),white,black);
            }

        stringstream build_info_stream;
        build_info_stream.str("");
        build_info_stream<<"Version "<<AutoVersion::FULLVERSION_STRING;
        build_info_stream<<"  Built "<<AutoVersion::DATE<<"/"<<AutoVersion::MONTH<<"/"<<AutoVersion::YEAR<<"  #"<<AutoVersion::BUILDS_COUNT;
        string build_info = build_info_stream.str();
        //size_t ver_length = build_info.size();
        int build_info_text_width, throwaway;
        SizeText(font, build_info, &build_info_text_width, &throwaway);
        Text_Outlined(font,SCREEN_WIDTH-build_info_text_width-10,SCREEN_HEIGHT-FONT_SIZE-8,build_info,white,black);

        //DRAW THE MOUSE CURSOR
        //mouse.cursor.draw(mouse.x+5,mouse.y+9,0,1.0);

        SDL_GL_SwapBuffers();
        if(quit_at_end_of_frame) has_quit = true;
        if( ( fps_is_capped == true ) && ( frame_time.GetTicks() < 1000 / FRAMES_PER_SECOND_CAP ) ) { //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND_CAP ) - frame_time.GetTicks() );
        }
}


void Game::BuildLists()
{
    square=glGenLists(2);
    glNewList(square,GL_COMPILE);
        glBegin(GL_QUADS);
            glTexCoord2i(0,0);  glVertex2f(-.5, -.5);
            glTexCoord2i(1,0);  glVertex2f(.5, -.5);
            glTexCoord2i(1,1);  glVertex2f(.5, .5);
            glTexCoord2i(0,1);  glVertex2f(-.5, .5);
        glEnd();
    glEndList();
    glNewList(square+1,GL_COMPILE);
        glBegin(GL_QUADS);
            glTexCoord2i(0,0); glVertex2f(0,0);
            glTexCoord2i(1,0);  glVertex2f(1, 0);
            glTexCoord2i(1,1);  glVertex2f(1, 1);
            glTexCoord2i(0,1);  glVertex2f(0, 1);
        glEnd();
    glEndList();

        square2=glGenLists(1);
    glNewList(square2,GL_COMPILE);
        glBegin(GL_QUADS);
            glTexCoord2i(0,0); glVertex2f(0,0);
            glTexCoord2i(1,0);  glVertex2f(1, 0);
            glTexCoord2i(1,1);  glVertex2f(1, 1);
            glTexCoord2i(0,1);  glVertex2f(0, 1);
        glEnd();
    glEndList();

}


bool Game::LoadLevel(string load_filename)
{
    ifstream load_file(load_filename.c_str(),ios::in|ios::binary);
    if(!load_file.is_open()){ //something's wrong, and the file isn't real, so don't load it
        fprintf(stderr, "Unable to load level, does not exist: %s \n", load_filename.c_str());
        load_file.close();
        return false;
    }
    else { // we load the level file
        int level_file_version;
        load_file.read((char*) &level_file_version, sizeof(int));
        if(level_file_version==3){
            for(int i = 0; i<=4; i++){ // we go through all the layers in the level
                DeleteObjectinLevel(loaded_level.layers[i].statics,DELETE_ALL); //clear the loaded_level's statics
                DeleteObjectinLevel(loaded_level.layers[i].entities,DELETE_ALL); //clear the loaded_level's entities
                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    loaded_level.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        char type;
                        load_file.read((char*) &type, sizeof(char)); //find out what type of entity it is
                        if(type=='g'){
                            Entity* temp_entity = new Entity_GrassType(0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='s'){
                            Entity* temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    loaded_level.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)) return false;
                        loaded_level.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            loaded_level.textures.clear(); //clear the loaded_level's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    loaded_level.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of loaded_level's textures
                }
            }
            size_t num_collision_series;
            loaded_level.collision_series.clear();
            load_file.read((char*) &num_collision_series, sizeof(num_collision_series));
            if(num_collision_series>0){
                loaded_level.collision_series.reserve(num_collision_series);
                for(unsigned int i = 0; i<num_collision_series; i++)
                {
                    CollisionSeries temp_series(0,0);
                    if(!temp_series.unserialize(load_file)) return false;
                    loaded_level.collision_series.push_back(temp_series);
                }
            }
        }
        else if(level_file_version==4){
            for(int i = 0; i<=5; i++){ // we go through all the layers in the level
                DeleteObjectinLevel(loaded_level.layers[i].statics,DELETE_ALL); //clear the loaded_level's statics
                DeleteObjectinLevel(loaded_level.layers[i].entities,DELETE_ALL); //clear the loaded_level's entities
                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    loaded_level.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        char type;
                        load_file.read((char*) &type, sizeof(char)); //find out what type of entity it is
                        if(type=='c'){
                            Entity* temp_entity = new Entity_CollisionType(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='g'){
                            Entity* temp_entity = new Entity_GrassType(0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='s'){
                            Entity* temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    loaded_level.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)) return false;
                        loaded_level.layers[i].statics.push_back(temp_static);

                    }
                }
            }
            size_t num_textures;
            loaded_level.textures.clear(); //clear the loaded_level's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    loaded_level.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of loaded_level's textures
                }
            }
            size_t num_collision_series;
            loaded_level.collision_series.clear();
            load_file.read((char*) &num_collision_series, sizeof(num_collision_series));
            if(num_collision_series>0){
                loaded_level.collision_series.reserve(num_collision_series);
                for(unsigned int i = 0; i<num_collision_series; i++)
                {
                    CollisionSeries temp_series(0,0);
                    if(!temp_series.unserialize(load_file)) return false;
                    loaded_level.collision_series.push_back(temp_series);
                }
            }
        }
        else if(level_file_version==5){
            for(int i = 0; i<=7; i++){ // we go through all the layers in the level
                DeleteObjectinLevel(loaded_level.layers[i].statics,DELETE_ALL); //clear the loaded_level's statics
                DeleteObjectinLevel(loaded_level.layers[i].entities,DELETE_ALL); //clear the loaded_level's entities
                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    loaded_level.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        char type;
                        load_file.read((char*) &type, sizeof(char)); //find out what type of entity it is
                        if(type=='c'){
                            Entity* temp_entity = new Entity_CollisionType(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='1'){
                            Entity* temp_entity = new Entity_CollisionType_White_Button(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='2'){
                            Entity* temp_entity = new Entity_CollisionType_Black_Button(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='3'){
                            Entity* temp_entity = new Entity_CollisionType_Grey_Button(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='4'){
                            Entity* temp_entity = new Entity_CollisionType_Red(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='5'){
                            Entity* temp_entity = new Entity_CollisionType_Green(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='6'){
                            Entity* temp_entity = new Entity_ButtonType_White(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='7'){
                            Entity* temp_entity = new Entity_ButtonType_Black(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='8'){
                            Entity* temp_entity = new Entity_ButtonType_Grey(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='g'){
                            Entity* temp_entity = new Entity_GrassType(0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='s'){
                            Entity* temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            loaded_level.layers[i].entities.push_back(temp_entity);
                        }
                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    loaded_level.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)) return false;
                        loaded_level.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            loaded_level.textures.clear(); //clear the loaded_level's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    loaded_level.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of loaded_level's textures
                }
            }
            size_t num_collision_series;
            loaded_level.collision_series.clear();
            load_file.read((char*) &num_collision_series, sizeof(num_collision_series));
            if(num_collision_series>0){
                loaded_level.collision_series.reserve(num_collision_series);
                for(unsigned int i = 0; i<num_collision_series; i++)
                {
                    CollisionSeries temp_series(0,0);
                    if(!temp_series.unserialize(load_file)) return false;
                    loaded_level.collision_series.push_back(temp_series);
                }
            }
        }
        if(level_file_version==6){
            for(int i = 0; i<=7; i++){ // we go through all the layers in the level
                DeleteObjectinLevel(loaded_level.layers[i].statics,DELETE_ALL); //clear the loaded_level's statics
                DeleteObjectinLevel(loaded_level.layers[i].entities,DELETE_ALL); //clear the loaded_level's entities

                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    loaded_level.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        string::size_type typename_length;
                        load_file.read((char*) &typename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                        vector <char> typename_buffer(typename_length); //create a vector as long as the read length in which we'll store the filename
                        load_file.read(&typename_buffer[0],typename_length); //read in the filename from the file and store it in the vector
                        string type;
                        type.assign(typename_buffer.begin(),typename_buffer.end()); //assign the vector to a string

                        //we create a NULL pointer to store our new entity
                        Entity* temp_entity = NULL;

                        //Now we interpret the type string so that we know what type of Entity should be loaded
                        //if it matches up to a known type, we allocate the memory for that type and assign the
                        //pointer to the new memory space

                        if(type=="CollisionType"){
                            temp_entity = new Entity_CollisionType(0,0,0,0);
                        }
                        else if(type=="CollisionType_White_Button"){
                            temp_entity = new Entity_CollisionType_White_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Black_Button"){
                            temp_entity = new Entity_CollisionType_Black_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Grey_Button"){
                            temp_entity = new Entity_CollisionType_Grey_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Red"){
                            temp_entity = new Entity_CollisionType_Red(0,0,0,0);
                        }
                        else if(type=="CollisionType_Green"){
                            temp_entity = new Entity_CollisionType_Green(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death"){
                            temp_entity = new Entity_CollisionType_Death(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death_Red"){
                            temp_entity = new Entity_CollisionType_Death_Red(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death_Green"){
                            temp_entity = new Entity_CollisionType_Death_Green(0,0,0,0);
                        }
                        else if(type=="CollisionType_Puzzle_Barrier"){
                            temp_entity = new Entity_CollisionType_Puzzle_Barrier(0,0,0,0);
                        }
                        else if(type=="ButtonType_White"){
                            temp_entity = new Entity_ButtonType_White(0,0,0,0);
                        }
                        else if(type=="ButtonType_Black"){
                            temp_entity = new Entity_ButtonType_Black(0,0,0,0);
                        }
                        else if(type=="ButtonType_Grey"){
                            temp_entity = new Entity_ButtonType_Grey(0,0,0,0);
                        }
                        else if(type=="PlayerStartType_Red"){
                            temp_entity = new Entity_PlayerStartType_Red(0,0,0,0);
                        }
                        else if(type=="PlayerStartType_Green"){
                            temp_entity = new Entity_PlayerStartType_Green(0,0,0,0);
                        }
                        else if(type=="MarkerType"){
                            temp_entity = new Entity_MarkerType(0,0,0,0);
                        }
                        else if(type=="RespawnZoneType"){
                            temp_entity = new Entity_RespawnZoneType(0,0,0,0);
                        }
                        else if(type=="DeathZoneType"){
                            temp_entity = new Entity_DeathZoneType(0,0,0,0);
                        }
                        else if(type=="DeathLaserType"){
                            temp_entity = new Entity_DeathLaserType(0,0,0,0);
                        }
                        else if(type=="ExitZoneType"){
                            temp_entity = new Entity_ExitZoneType(0,0,0,0);
                        }
                        else if(type=="GrassType"){
                            temp_entity = new Entity_GrassType(0,0,0,0,0);
                        }
                        else if(type=="SignType"){
                            temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                        }
                        else{ //we don't know about this entity type, so we can't handle loading the file
                            fprintf(stderr, "Unexpected Entity Type: %s \n", type.c_str()); //print what we can to the error log
                            load_file.close(); //clean up
                            return false;

                        }

                        //Now that we know the type of the entity, we can just load it
                            if(!temp_entity->unserialize(load_file)){
                                //if the unserialize function returns false, loading has failed somehow
                                load_file.close(); //don't forget to clean up
                                return false;
                            }
                            loaded_level.layers[i].entities.push_back(temp_entity);

                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    loaded_level.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)){
                            //something really wierd must've happened
                            load_file.close();
                             return false;
                        }
                        loaded_level.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            loaded_level.textures.clear(); //clear the loaded_level's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    loaded_level.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of loaded_level's textures
                }
            }
        }
        if(level_file_version==7){
            load_file.read((char*) &loaded_level.background_r, sizeof(loaded_level.background_r));
            load_file.read((char*) &loaded_level.background_g, sizeof(loaded_level.background_r));
            load_file.read((char*) &loaded_level.background_b, sizeof(loaded_level.background_r));
            for(int i = 0; i<=7; i++){ // we go through all the layers in the level
                DeleteObjectinLevel(loaded_level.layers[i].statics,DELETE_ALL); //clear the loaded_level's statics
                DeleteObjectinLevel(loaded_level.layers[i].entities,DELETE_ALL); //clear the loaded_level's entities

                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    loaded_level.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        string::size_type typename_length;
                        load_file.read((char*) &typename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                        vector <char> typename_buffer(typename_length); //create a vector as long as the read length in which we'll store the filename
                        load_file.read(&typename_buffer[0],typename_length); //read in the filename from the file and store it in the vector
                        string type;
                        type.assign(typename_buffer.begin(),typename_buffer.end()); //assign the vector to a string

                        //we create a NULL pointer to store our new entity
                        Entity* temp_entity = NULL;

                        //Now we interpret the type string so that we know what type of Entity should be loaded
                        //if it matches up to a known type, we allocate the memory for that type and assign the
                        //pointer to the new memory space

                        if(type=="CollisionType"){
                            temp_entity = new Entity_CollisionType(0,0,0,0);
                        }
                        else if(type=="CollisionType_White_Button"){
                            temp_entity = new Entity_CollisionType_White_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Black_Button"){
                            temp_entity = new Entity_CollisionType_Black_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Grey_Button"){
                            temp_entity = new Entity_CollisionType_Grey_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Red"){
                            temp_entity = new Entity_CollisionType_Red(0,0,0,0);
                        }
                        else if(type=="CollisionType_Green"){
                            temp_entity = new Entity_CollisionType_Green(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death"){
                            temp_entity = new Entity_CollisionType_Death(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death_Red"){
                            temp_entity = new Entity_CollisionType_Death_Red(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death_Green"){
                            temp_entity = new Entity_CollisionType_Death_Green(0,0,0,0);
                        }
                        else if(type=="CollisionType_Puzzle_Barrier"){
                            temp_entity = new Entity_CollisionType_Puzzle_Barrier(0,0,0,0);
                        }
                        else if(type=="ButtonType_White"){
                            temp_entity = new Entity_ButtonType_White(0,0,0,0);
                        }
                        else if(type=="ButtonType_Black"){
                            temp_entity = new Entity_ButtonType_Black(0,0,0,0);
                        }
                        else if(type=="ButtonType_Grey"){
                            temp_entity = new Entity_ButtonType_Grey(0,0,0,0);
                        }
                        else if(type=="PlayerStartType_Red"){
                            temp_entity = new Entity_PlayerStartType_Red(0,0,0,0);
                        }
                        else if(type=="PlayerStartType_Green"){
                            temp_entity = new Entity_PlayerStartType_Green(0,0,0,0);
                        }
                        else if(type=="MarkerType"){
                            temp_entity = new Entity_MarkerType(0,0,0,0);
                        }
                        else if(type=="RespawnZoneType"){
                            temp_entity = new Entity_RespawnZoneType(0,0,0,0);
                        }
                        else if(type=="DeathZoneType"){
                            temp_entity = new Entity_DeathZoneType(0,0,0,0);
                        }
                        else if(type=="DeathLaserType"){
                            temp_entity = new Entity_DeathLaserType(0,0,0,0);
                        }
                        else if(type=="ExitZoneType"){
                            temp_entity = new Entity_ExitZoneType(0,0,0,0);
                        }
                        else if(type=="GrassType"){
                            temp_entity = new Entity_GrassType(0,0,0,0,0);
                        }
                        else if(type=="SignType"){
                            temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                        }
                        else{ //we don't know about this entity type, so we can't handle loading the file
                            fprintf(stderr, "Unexpected Entity Type: %s \n", type.c_str()); //print what we can to the error log
                            load_file.close(); //clean up
                            return false;

                        }

                        //Now that we know the type of the entity, we can just load it
                            if(!temp_entity->unserialize(load_file)){
                                //if the unserialize function returns false, loading has failed somehow
                                load_file.close(); //don't forget to clean up
                                return false;
                            }
                            loaded_level.layers[i].entities.push_back(temp_entity);

                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    loaded_level.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)){
                            //something really wierd must've happened
                            load_file.close();
                             return false;
                        }
                        loaded_level.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            loaded_level.textures.clear(); //clear the loaded_level's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    loaded_level.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of loaded_level's textures
                }
            }
        }
        else fprintf(stderr, "Unable to load level, unsupported .level version or is not a .lvl file: %s \n", load_filename.c_str());
        load_file.close();
        return true;
    }
}
