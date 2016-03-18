#ifndef MAIN_EDITOR_FUNCTIONS_HPP_INCLUDED
#define MAIN_EDITOR_FUNCTIONS_HPP_INCLUDED



int InitializeMainEditor()
{
    using namespace main_editor;
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    if( Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) == -1){
        fprintf(stderr, "Unable to initialize SDL_mixer with 48000Hz 16-bit audio: %s\n", SDL_GetError());
        return 1;
    }
    /*if( TTF_Init() == -1 ) {
        fprintf(stderr, "Unable to initialize SDL_TTF: %s\n", SDL_GetError());
        return 1;
    }

    debug_font = TTF_OpenFont( "GEORGIA.ttf", DEBUG_FONT_SIZE);
    if(debug_font==NULL){
        fprintf(stderr, "Unable to load GEORGIA.ttf font for use: %s\n", SDL_GetError());
        return 1;
    }*/




    //testing_font = temp;

    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);


    Uint32 videoMode = SDL_OPENGL|SDL_DOUBLEBUF;
    if(WINDOW_IS_RESIZABLE) videoMode = SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE;
    if(FULLSCREEN_IS_ENABLED) videoMode = SDL_OPENGL|SDL_FULLSCREEN|SDL_DOUBLEBUF;

    DesktopVideoInfo = SDL_GetVideoInfo();

    //This should only be used for the editor:
    if(FULLSCREEN_IS_ENABLED)
    {
        SCREEN_WIDTH = DesktopVideoInfo->current_w;
        SCREEN_HEIGHT = DesktopVideoInfo->current_h;
        LOGICAL_SCREEN_WIDTH = SCREEN_WIDTH;
        LOGICAL_SCREEN_HEIGHT = SCREEN_HEIGHT;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,videoMode);
    glewExperimental=true;
    glewInit();
    //GLeeInit();

    //debug_font = LoadFont("GEORGIA.ttf",14);
    //SerializeFont("Georgia.font",debug_font);
    //DeleteFont(debug_font);
    debug_font = UnserializeFont("font/Georgia.font");

    //This should be used for the game:
    //screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,videoMode);

    SDL_WM_SetCaption("Duet Level Editor","Duet Level Editor");
    SDL_ShowCursor(0); //turns off the SDL cursor, so that we can draw our own

    glClearColor(1,1,1,1);



    glEnable(GL_BLEND);
    //glEnable(GL_COLOR_MATERIAL);
    //glEnable(GL_CLAMP_TO_EDGE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,LOGICAL_SCREEN_WIDTH,LOGICAL_SCREEN_HEIGHT,0,-1,1);
    glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    InitVSync();
    SetVSyncState(VSYNC_IS_ENABLED);

      //set the level width and height
    working_level.width = 4000;//DEFAULT_LEVEL_WIDTH;
    working_level.height = 4000;//DEFAULT_LEVEL_HEIGHT;
    working_level.layers[0].parallax_modifier = 1.5;
    working_level.layers[1].parallax_modifier = 1;
    working_level.layers[2].parallax_modifier = 1;
    working_level.layers[3].parallax_modifier = .63;
    working_level.layers[4].parallax_modifier = .40;
    working_level.layers[5].parallax_modifier = .25;
    working_level.layers[6].parallax_modifier = .1;
    working_level.layers[7].parallax_modifier = 0;
    working_level.background_r = 1.0; working_level.background_g = 1.0; working_level.background_b = 1.0;


    active_texture_index = 0;

     editor_bg = LoadTexture("img/editor/editor_bg.tga", true);
     snap_grid_bg = LoadTexture("img/editor/snap_grid_bg.tga");
     snap_grid_overlay = LoadTexture("img/editor/snap_grid_overlay.tga");
     editor_cursor = LoadTexture("img/editor/editor_cursor.tga");
     editor_cursor_move = LoadTexture("img/editor/editor_cursor_move.tga");
     editor_cursor_rotate = LoadTexture("img/editor/editor_cursor_rotate.tga");
     editor_cursor_scale = LoadTexture("img/editor/editor_cursor_scale.tga");
     editor_cursor_clip = LoadTexture("img/editor/editor_cursor_clip.tga");

     collision_box = LoadTexture("img/editor/collision_box.tga");
     collision_box_white = LoadTexture("img/editor/collision_box_white.tga");
     collision_box_black = LoadTexture("img/editor/collision_box_black.tga");
     collision_box_grey = LoadTexture("img/editor/collision_box_grey.tga");
     collision_box_red = LoadTexture("img/editor/collision_box_red.tga");
     collision_box_green = LoadTexture("img/editor/collision_box_green.tga");
     marker = LoadTexture("img/editor/marker.tga");
     respawn_zone = LoadTexture("img/editor/respawn_zone.tga");
     exit_zone = LoadTexture("img/editor/exit_zone.tga");
     death_zone = LoadTexture("img/editor/death_zone.tga");
     death_laser = LoadTexture("img/editor/death_laser.tga");
     player_red = LoadTexture("img/editor/player1.tga");
     player_green = LoadTexture("img/editor/player2.tga");
     button_white_up = LoadTexture("img/button_white_up.tga");
     button_black_up = LoadTexture("img/button_black_up.tga");
     button_grey_up = LoadTexture("img/button_grey_up.tga");

     BuildLists();


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


     fps_clock_sample = SDL_GetTicks();

     return 0;
}

void GetFPSSampleForThisFrame()
{
    using namespace main_editor;
    if(fps_clock_sample + 1000 < SDL_GetTicks())
    {
        current_frames_per_second = frames_shown;
        frames_shown = 0;
        fps_clock_sample = SDL_GetTicks();
    }
    else frames_shown++;
    frame_time.Start();
}

void HandleEntityEditorModeInput()
{
    using namespace main_editor;
        if(event.type==SDL_MOUSEMOTION){
            mx = event.motion.x;
            my = event.motion.y;
        }
        if(is_actually_editing_a_static)
        {
            Static *active_static = reinterpret_cast<Static*>(working_level.layers[active_layer].statics[entity_editor_active_index]);
            if(entity_editor_is_initialized==false)
            {
                //entity_editor_UIElements.push_back( new UI_TextField("Fade in Delay(in milliseconds)",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE,400,DEBUG_FONT_SIZE*2,&(active_entity->fade_delay_ms)));
                entity_editor_UIElements.push_back( new UI_Sliderf("Red Multiply", debug_font, SCREEN_WIDTH/2-500, SCREEN_HEIGHT/4, 250, 20, &(active_static->r_mult), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Green Multiply", debug_font, SCREEN_WIDTH/2-500, SCREEN_HEIGHT/4+50, 250, 20, &(active_static->g_mult), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Blue Multiply", debug_font, SCREEN_WIDTH/2-500, SCREEN_HEIGHT/4+100, 250, 20, &(active_static->b_mult), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Alpha Multiply", debug_font, SCREEN_WIDTH/2-500, SCREEN_HEIGHT/4+150, 250, 20, &(active_static->a_mult), 0.0, 1.0));

                entity_editor_UIElements.push_back( new UI_Sliderf("Red Add", debug_font, SCREEN_WIDTH/2-500, SCREEN_HEIGHT/4+200, 250, 20, &(active_static->r_add), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Green Add", debug_font, SCREEN_WIDTH/2-500, SCREEN_HEIGHT/4+250, 250, 20, &(active_static->g_add), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Blue Add", debug_font, SCREEN_WIDTH/2-500, SCREEN_HEIGHT/4+300, 250, 20, &(active_static->b_add), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Alpha Add", debug_font, SCREEN_WIDTH/2-500, SCREEN_HEIGHT/4+350, 250, 20, &(active_static->a_add), 0.0, 1.0));

                entity_editor_UIElements.push_back( new UI_Sliderf("Red Blend", debug_font, SCREEN_WIDTH/2-200, SCREEN_HEIGHT/4, 250, 20, &(active_static->r_blend), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Green Blend", debug_font, SCREEN_WIDTH/2-200, SCREEN_HEIGHT/4+50, 250, 20, &(active_static->g_blend), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Blue Blend", debug_font, SCREEN_WIDTH/2-200, SCREEN_HEIGHT/4+100, 250, 20, &(active_static->b_blend), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Alpha Blend", debug_font, SCREEN_WIDTH/2-200, SCREEN_HEIGHT/4+150, 250, 20, &(active_static->a_blend), 0.0, 1.0));

                entity_editor_UIElements.push_back( new UI_Sliderf("Hue", debug_font, SCREEN_WIDTH/2+100, SCREEN_HEIGHT/4, 250, 20, &(active_static->hue), -0.5, 0.5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Saturation", debug_font, SCREEN_WIDTH/2+100, SCREEN_HEIGHT/4+50, 250, 20, &(active_static->saturation), 0.0, 5.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Lightness", debug_font, SCREEN_WIDTH/2+100, SCREEN_HEIGHT/4+100, 250, 20, &(active_static->lightness), -1.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Brightness", debug_font, SCREEN_WIDTH/2+100, SCREEN_HEIGHT/4+150, 250, 20, &(active_static->brightness), 0.0, 2.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("Contrast", debug_font, SCREEN_WIDTH/2+100, SCREEN_HEIGHT/4+200, 250, 20, &(active_static->contrast), 0.0, 2.0));

                entity_editor_UIElements.push_back( new UI_Button("Copy Settings",debug_font, SCREEN_WIDTH/2-500,SCREEN_HEIGHT/4+400,&should_copy_static_settings));
                entity_editor_UIElements.push_back( new UI_Button("Paste Settings",debug_font, SCREEN_WIDTH/2-500,SCREEN_HEIGHT/4+450,&should_paste_static_settings));

                entity_editor_UIElements.push_back( new UI_Button("Toggle Blend Mode",debug_font, SCREEN_WIDTH/2-120,SCREEN_HEIGHT/2+50,&(active_static->is_using_additive_tinting)));
                entity_editor_UIElements.push_back( new UI_Button("Toggle Colorize",debug_font, SCREEN_WIDTH/2+100,SCREEN_HEIGHT/2+50,&(active_static->is_using_colorize_hue)));
                entity_editor_UIElements.push_back( new UI_Button("Okay/Close",debug_font, SCREEN_WIDTH/2-40,SCREEN_HEIGHT/2+100,&entity_editor_is_open));
                entity_editor_is_initialized = true;

            }

        }
        else if(is_actually_editing_global_settings)
        {
            if(entity_editor_is_initialized==false)
            {
                //entity_editor_UIElements.push_back( new UI_TextField("Fade in Delay(in milliseconds)",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE,400,DEBUG_FONT_SIZE*2,&(active_entity->fade_delay_ms)));
                entity_editor_UIElements.push_back( new UI_Sliderf("BG Red", debug_font, SCREEN_WIDTH/2-125, SCREEN_HEIGHT/4, 250, 20, &(working_level.background_r), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("BG Green", debug_font, SCREEN_WIDTH/2-125, SCREEN_HEIGHT/4+50, 250, 20, &(working_level.background_g), 0.0, 1.0));
                entity_editor_UIElements.push_back( new UI_Sliderf("BG Blue", debug_font, SCREEN_WIDTH/2-125, SCREEN_HEIGHT/4+100, 250, 20, &(working_level.background_b), 0.0, 1.0));

                entity_editor_UIElements.push_back( new UI_Button("Okay/Close",debug_font, SCREEN_WIDTH/2-40,SCREEN_HEIGHT/2+100,&entity_editor_is_open));
                entity_editor_is_initialized = true;

            }
        }
        else if(entity_types[active_entity_type_index] == "ParticleSystemType") //SIGN_TYPE ENTITIES
        {
            Entity_ParticleSystemType *active_entity = reinterpret_cast<Entity_ParticleSystemType*>(working_level.layers[active_layer].entities[entity_editor_active_index]);
            if(entity_editor_is_initialized==false)
            {
                entity_editor_UIElements.push_back( new UI_Slideri("Frequency", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5, 250, 10, &(active_entity->particle_frequency), 0, 50));
                entity_editor_UIElements.push_back( new UI_Slideri("Min Life", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+35, 250, 10, &(active_entity->particle_min_life), 0, 100));
                entity_editor_UIElements.push_back( new UI_Slideri("Max Life", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*2), 250, 10, &(active_entity->particle_max_life), 0, 100));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min XV", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*3), 250, 10, &(active_entity->particle_min_xv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max XV", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*4), 250, 10, &(active_entity->particle_max_xv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min XA", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*5), 250, 10, &(active_entity->particle_min_xa), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max XA", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*6), 250, 10, &(active_entity->particle_max_xa), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min YV", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*7), 250, 10, &(active_entity->particle_min_yv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max YV", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*8), 250, 10, &(active_entity->particle_max_yv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min YA", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*9), 250, 10, &(active_entity->particle_min_ya), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max YA", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*10), 250, 10, &(active_entity->particle_max_ya), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min RotV", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*11), 200, 10, &(active_entity->particle_min_rotv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max RotV", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*12), 200, 10, &(active_entity->particle_max_rotv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min RotA", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*13), 200, 10, &(active_entity->particle_min_rota), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max RotA", debug_font, SCREEN_WIDTH/4, SCREEN_HEIGHT/5+(35*14), 200, 10, &(active_entity->particle_max_rota), -5, 5));

                entity_editor_UIElements.push_back( new UI_Sliderf("Min Width", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5, 250, 10, &(active_entity->particle_min_width), 0, 100));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max Width", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+35, 250, 10, &(active_entity->particle_max_width), 0, 100));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min WidthV", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+35*2, 250, 10, &(active_entity->particle_min_widthv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max WidthV", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+35*3, 250, 10, &(active_entity->particle_max_widthv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min WidthA", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+(35*4), 250, 10, &(active_entity->particle_min_widtha), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max WidthA", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+(35*5), 250, 10, &(active_entity->particle_max_widtha), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min Height", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+(35*6), 250, 10, &(active_entity->particle_min_height), 0, 100));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max Height", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+(35*7), 250, 10, &(active_entity->particle_max_height), 0, 100));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min HeightV", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+(35*8), 250, 10, &(active_entity->particle_min_heightv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max HeightV", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+(35*9), 250, 10, &(active_entity->particle_max_heightv), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Min HeightA", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+(35*10), 250, 10, &(active_entity->particle_min_heighta), -5, 5));
                entity_editor_UIElements.push_back( new UI_Sliderf("Max HeightA", debug_font, SCREEN_WIDTH/4+300, SCREEN_HEIGHT/5+(35*11), 250, 10, &(active_entity->particle_max_heighta), -5, 5));


                                                                      /* this->particle_frequency = particle_frequency;
                                           this->particle_min_life = particle_min_life;
                                           this->particle_max_life = particle_max_life;
                                           this->particle_min_xv = particle_min_xv;
                                           this->particle_max_xv = particle_max_xv;
                                           this->particle_min_xa = particle_min_xa;
                                           this->particle_max_xa = particle_max_xa;
                                           this->particle_min_yv = particle_min_yv;
                                           this->particle_max_yv = particle_max_xv;
                                           this->particle_min_ya = particle_min_ya;
                                           this->particle_max_ya = particle_max_ya;
                                           this->particle_min_rotv = particle_min_rotv;
                                           this->particle_max_rotv = particle_max_rotv;
                                           this->particle_min_rota = particle_min_rota;
                                           this->particle_max_rota = particle_max_rota;
                                           this->particle_min_width = particle_min_width;
                                           this->particle_max_width = particle_max_width;
                                           this->particle_min_widthv = particle_min_widthv;
                                           this->particle_max_widthv = particle_max_widthv;
                                           this->particle_min_widtha = particle_min_widtha;
                                           this->particle_max_widtha = particle_max_widtha;
                                           this->particle_min_height = particle_min_height;
                                           this->particle_max_height = particle_max_height;
                                           this->particle_min_heightv = particle_min_heightv;
                                           this->particle_max_heightv = particle_max_heightv;
                                           this->particle_min_heighta = particle_min_heighta;
                                           this->particle_max_heighta = particle_max_heighta;
                                           this->particle_middle_t = particle_middle_t;
                                           this->particle_middle_len = particle_middle_len;
                                           this->particle_start_r = particle_start_r;
                                           this->particle_start_g = particle_start_g;
                                           this->particle_start_b = particle_start_b;
                                           this->particle_start_a = particle_start_a;
                                           this->particle_middle_r = particle_middle_r;
                                           this->particle_middle_g = particle_middle_g;
                                           this->particle_middle_b = particle_middle_b;
                                           this->particle_middle_a = particle_middle_a;
                                           this->particle_end_r = particle_end_r;
                                           this->particle_end_g = particle_end_g;
                                           this->particle_end_b = particle_end_b;
                                           this->particle_end_a = particle_end_a;*/
                entity_editor_UIElements.push_back( new UI_Button("Okay/Close",debug_font, SCREEN_WIDTH/2-40,SCREEN_HEIGHT/2+100,&entity_editor_is_open));
                entity_editor_is_initialized = true;
            }

        }
        else if(entity_types[active_entity_type_index] == "SignType") //SIGN_TYPE ENTITIES
        {
            Entity_SignType *active_entity = reinterpret_cast<Entity_SignType*>(working_level.layers[active_layer].entities[entity_editor_active_index]);
            if(entity_editor_is_initialized==false)
            {
                entity_editor_UIElements.push_back( new UI_TextField("Fade in Delay(in milliseconds)",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE,400,DEBUG_FONT_SIZE*2,&(active_entity->fade_delay_ms)));
                entity_editor_UIElements.push_back( new UI_Button("Okay/Close",debug_font, SCREEN_WIDTH/2-40,SCREEN_HEIGHT/2+100,&entity_editor_is_open));
                entity_editor_is_initialized = true;
            }

        }
        else if(entity_types[active_entity_type_index] == "TreeType")
        {
            Entity_TreeType *active_entity = reinterpret_cast<Entity_TreeType*>(working_level.layers[active_layer].entities[entity_editor_active_index]);
            if(entity_editor_is_initialized==false)
            {
                entity_editor_UIElements.push_back( new UI_TreeEditor(SCREEN_WIDTH/16,SCREEN_HEIGHT/16,(int)(SCREEN_WIDTH*.9),(int)(SCREEN_HEIGHT*.85),active_entity));
                entity_editor_UIElements.push_back(  new UI_Button("Okay/Close",debug_font, SCREEN_WIDTH/2-60,(int)(SCREEN_HEIGHT*.96),&entity_editor_is_open));
                entity_editor_is_initialized = true;
            }

        }
        else if(entity_types[active_entity_type_index] == "MarkerType")
        {
            //Entity_MarkerType *active_entity = reinterpret_cast<Entity_MarkerType*>(working_level.layers[active_layer].entities[entity_editor_active_index]);
            if(entity_editor_is_initialized==false)
            {
                //entity_editor_UIElements.push_back( new UI_TextField("Marker Index (Use this for references to the marker)",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE,400,DEBUG_FONT_SIZE*2,&(active_entity->fade_delay_ms)));
                stringstream index;
                index<<"Marker Index: "<<entity_editor_active_index<<" Use this to reference the marker from other entities. (Click to Close)";
                entity_editor_UIElements.push_back(  new UI_Button(index.str(),debug_font, SCREEN_WIDTH/2-240,SCREEN_HEIGHT/2,&entity_editor_is_open));
                entity_editor_is_initialized = true;
            }

        }
        else if(entity_types[active_entity_type_index] == "CollisionType_Puzzle_Barrier")
        {
            Entity_CollisionType_Puzzle_Barrier *active_entity = reinterpret_cast<Entity_CollisionType_Puzzle_Barrier*>(working_level.layers[active_layer].entities[entity_editor_active_index]);
            if(entity_editor_is_initialized==false)
            {
                //stringstream index;
                entity_editor_UIElements.push_back( new UI_TextField("Index of Respawn Zone",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE*5,400,DEBUG_FONT_SIZE*2,&(active_entity->tied_respawn_zone_index)));
                entity_editor_UIElements.push_back( new UI_TextField("Index of this",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE,400,DEBUG_FONT_SIZE*2,&(active_entity->index)));
                entity_editor_UIElements.push_back(  new UI_Button("Okay/Close",debug_font, SCREEN_WIDTH/2-40,SCREEN_HEIGHT/2+100,&entity_editor_is_open));
                entity_editor_is_initialized = true;
            }

        }
        else if(entity_types[active_entity_type_index] == "RespawnZoneType")
        {
            Entity_RespawnZoneType *active_entity = reinterpret_cast<Entity_RespawnZoneType*>(working_level.layers[active_layer].entities[entity_editor_active_index]);
            if(entity_editor_is_initialized==false)
            {
                //stringstream index;
                entity_editor_UIElements.push_back( new UI_TextField("Index of this",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE,400,DEBUG_FONT_SIZE*2,&(active_entity->index)));
                entity_editor_UIElements.push_back(  new UI_Button("Okay/Close",debug_font, SCREEN_WIDTH/2-60,(int)(SCREEN_HEIGHT*.96),&entity_editor_is_open));
                entity_editor_is_initialized = true;
            }

        }
        else if(entity_types[active_entity_type_index] == "ExitZoneType")
        {
            Entity_ExitZoneType *active_entity = reinterpret_cast<Entity_ExitZoneType*>(working_level.layers[active_layer].entities[entity_editor_active_index]);
            if(entity_editor_is_initialized==false)
            {

                entity_editor_UIElements.push_back( new UI_TextField("Index of Puzzle_Barrier (Use -1 to skip to the next level)",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE,400,DEBUG_FONT_SIZE*2,&(active_entity->puzzle_barrier_index)));
                entity_editor_UIElements.push_back( new UI_Button("Okay/Close",debug_font, SCREEN_WIDTH/2-40,SCREEN_HEIGHT/2+100,&entity_editor_is_open));
                entity_editor_is_initialized = true;
            }
        }
        else{
            entity_editor_is_open = false;
        }
        //First we update all of the active UI elements, if any
            for(unsigned int i = 0; i<entity_editor_UIElements.size(); i++)
            {
                entity_editor_UIElements[i]->update(event);
            }

            //Handle copy/paste settings
            if(should_copy_static_settings)
            {
                Clipboard_Static = reinterpret_cast<Static*>(working_level.layers[active_layer].statics[entity_editor_active_index]);
                should_copy_static_settings = false;            }

            if(should_paste_static_settings)
            {
                Static *active_static = reinterpret_cast<Static*>(working_level.layers[active_layer].statics[entity_editor_active_index]);
                active_static->r_mult = Clipboard_Static->r_mult;
                active_static->g_mult = Clipboard_Static->g_mult;
                active_static->b_mult = Clipboard_Static->b_mult;
                active_static->a_mult = Clipboard_Static->a_mult;

                active_static->r_add = Clipboard_Static->r_add;
                active_static->g_add = Clipboard_Static->g_add;
                active_static->b_add = Clipboard_Static->b_add;
                active_static->a_add = Clipboard_Static->a_add;

                active_static->r_blend = Clipboard_Static->r_blend;
                active_static->g_blend = Clipboard_Static->g_blend;
                active_static->b_blend = Clipboard_Static->b_blend;
                active_static->a_blend = Clipboard_Static->a_blend;

                active_static->hue = Clipboard_Static->hue;
                active_static->saturation = Clipboard_Static->saturation;
                active_static->lightness = Clipboard_Static->lightness;
                active_static->brightness = Clipboard_Static->brightness;
                active_static->contrast = Clipboard_Static->contrast;

                active_static->is_using_additive_tinting = Clipboard_Static->is_using_additive_tinting;
                active_static->is_using_colorize_hue = Clipboard_Static->is_using_colorize_hue;

                should_paste_static_settings = false;
            }

        //If one of those elements closed the entity editor, or the user pressed the ESC key, we close the editor and clean up
        if(pressedkey[SDLK_ESCAPE]||!entity_editor_is_open){
            entity_editor_is_open = false;
            entity_editor_is_initialized = false;
            is_actually_editing_a_static = false;
            is_actually_editing_global_settings = false;
            for(unsigned int i = 0; i<entity_editor_UIElements.size();i++)
            {
                delete entity_editor_UIElements[i];
            }
            entity_editor_UIElements.clear();
        }

}

void HandleLoadingModeInput()
{
    using namespace main_editor;
        if(event.type == SDL_MOUSEMOTION){
            mx = event.motion.x;
            my = event.motion.y;
        }
        typing_handler.update(event);
            if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_RETURN)
            {
                if(input_mode == INPUT_LOAD_TEXTURE){ //we're loading a texture
                    string filename_for_testing =typing_handler.return_string();
                    // RANGE INDICATOR CRAP
                    string::size_type position_of_range_indicator = filename_for_testing.find(':');
                    string::size_type position_of_folder_indicator = filename_for_testing.find("//");
                    //string::size_type loading_file = filename_for_testing.find(".tga");
                    if(position_of_range_indicator!=string::npos)
                    {
                        string::size_type position_of_bracket = filename_for_testing.find(">");
                        if(position_of_bracket!=string::npos){

                            string first_number;
                            first_number.assign(filename_for_testing,position_of_range_indicator+1,position_of_bracket-(position_of_range_indicator+1));
                            string second_number;
                            second_number.assign(filename_for_testing,position_of_bracket+1,position_of_bracket+3);
                            unsigned int range_lower_number, range_higher_number;
                            stringstream(first_number) >> range_lower_number;
                            stringstream(second_number) >> range_higher_number;
                            for(unsigned int i = range_lower_number; i <= range_higher_number; i++)
                            {
                             string temp_filename;
                             temp_filename.assign(filename_for_testing,0,position_of_range_indicator);
                             stringstream number;
                             number<<i;
                             temp_filename = "img/"+temp_filename+number.str()+".tga";
                             if(LoadTextureFileIntoLevel(temp_filename,working_level)) editor_status_list.add_message("Successfully loaded texture file: "+temp_filename);
                             else editor_status_list.add_message("ERROR: FAILED TO LOAD TEXTURE FILE: "+temp_filename);
                            }
                            typing_handler.clear();
                            is_taking_typing_input = false;
                        }
                        else{
                            typing_handler.clear();
                            is_taking_typing_input = false;
                        }
                    }
                    else if(position_of_folder_indicator!=string::npos) //load entire folder crap
                    {
                        string path;
                        path.assign(filename_for_testing,0,position_of_folder_indicator+1);
                        path = "img/"+path;

                        struct dirent *directory_entry;
                        DIR *dir_ptr;

                        dir_ptr = opendir(path.c_str());
                        //dir_ptr = opendir("img/");
                        if(dir_ptr == NULL) //we couldn't open that directory
                        {
                            typing_handler.clear();
                            is_taking_typing_input = false;
                        }
                        else
                        {
                            while((directory_entry = readdir(dir_ptr))) { //<--yes that is supposed to be an assignment, not a evaluation
                                fprintf(stdout, "directory_entry = %s\n", directory_entry->d_name);
                                stringstream file;
                                file<<directory_entry->d_name;
                                string temp_filename = path+file.str();
                                string::size_type png_loc = temp_filename.find(".tga");
                                if(png_loc!=string::npos){
                                if(LoadTextureFileIntoLevel(temp_filename,working_level)) editor_status_list.add_message("Successfully loaded texture file: "+temp_filename);
                                else editor_status_list.add_message("ERROR: FAILED TO LOAD TEXTURE FILE: "+temp_filename);
                                }
                                //puts(directory_entry->d_name);
                            }
                            closedir(dir_ptr);
                            typing_handler.clear();
                            is_taking_typing_input = false;
                        }
                    }
                    else{
                        string temp_filename = "img/"+typing_handler.return_string()+".tga";
                        if(LoadTextureFileIntoLevel(temp_filename,working_level)) editor_status_list.add_message("Successfully loaded texture file: "+temp_filename);
                        else editor_status_list.add_message("ERROR: FAILED TO LOAD TEXTURE FILE: "+temp_filename);
                        typing_handler.clear();
                        is_taking_typing_input = false;
                    }
                }
                if(input_mode == INPUT_LOAD_LEVEL){
                    string temp_filename = "lvl/"+typing_handler.return_string()+".lvl";
                    if(!LoadLeveltoStructure(temp_filename,working_level))
                    {
                        editor_status_list.add_message("ERROR: LEVEL NOT LOADED: "+temp_filename);
                        typing_handler.clear();
                        is_taking_typing_input = false;
                    }
                    else{
                        editor_status_list.add_message("Level Loaded: "+temp_filename);
                        loaded_filename.clear();
                        loaded_filename = temp_filename;
                        typing_handler.clear();
                        is_taking_typing_input = false;
                    }
                }
                if(input_mode == INPUT_SAVE_LEVEL){
                    //knock the temporary characters from the end of the string
                    string temp_filename = "lvl/"+typing_handler.return_string()+".lvl";
                    if(SaveLeveltoFile(temp_filename,working_level)) editor_status_list.add_message("File Saved: "+temp_filename);
                    else editor_status_list.add_message("ERROR: FILE WASN'T SAVED: "+temp_filename);
                    loaded_filename = temp_filename;
                    typing_handler.clear();
                    is_taking_typing_input = false;
                }
            }
        if(pressedkey[SDLK_DELETE]||pressedkey[SDLK_ESCAPE]){
            is_taking_typing_input = false;
            typing_handler.clear();
            editor_status_list.add_message("Aborted");
        }
}

void HandleStandardMode_GlobalInput()
{
    using namespace main_editor;
            if(event.type == SDL_QUIT||pressedkey[SDLK_ESCAPE]) quit = true;

            in_game_overlay_offset_x = (int)(SCREEN_WIDTH/2-((ACTUAL_GAME_SCREEN_WIDTH*camera_zoom)/2));
            in_game_overlay_offset_y = (int)(SCREEN_HEIGHT/2-((ACTUAL_GAME_SCREEN_HEIGHT*camera_zoom)/2));
            in_game_camera_x = int(in_game_overlay_offset_x/camera_zoom);
            in_game_camera_y = int(in_game_overlay_offset_y/camera_zoom);

            active_layer_parallax_mod = working_level.layers[active_layer].parallax_modifier;


            if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_SLASH)
            {
              if(text_info_state == DEBUG_TEXT) text_info_state = CONTROLS;
              else text_info_state = DEBUG_TEXT;
            }

            if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_TAB){
                if(info_text_is_hidden!=true) info_text_is_hidden = true;
                else info_text_is_hidden = false;
            }

            if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_g) is_game_preview_mode_on = (is_game_preview_mode_on)? false: true;
            if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_h) are_collision_boxes_always_shown = (are_collision_boxes_always_shown)? false: true;

            if(pressedkey[SDLK_LSHIFT]||pressedkey[SDLK_RSHIFT]) shift_is_pressed = true;
            else(shift_is_pressed = false);

            if(pressedkey[SDLK_LALT]||pressedkey[SDLK_RALT]) alt_is_pressed = true;
            else(alt_is_pressed = false);

            if(event.type == SDL_MOUSEMOTION){
                mx = event.motion.x;
                my = event.motion.y;
                mx_inworld = (int)((mx/camera_zoom)+(camera_x*active_layer_parallax_mod)-in_game_camera_x);//active_layer_parallax_mod;
                my_inworld = (int)((my/camera_zoom)+(camera_y*active_layer_parallax_mod)-in_game_camera_y);//active_layer_parallax_mod;
            }

            if(pressedkey[SDLK_F1]) {
                input_mode = INPUT_LOAD_LEVEL;
                is_taking_typing_input = true;
            }

            if(pressedkey[SDLK_F2]) {
                input_mode = INPUT_SAVE_LEVEL;
                is_taking_typing_input = true;
            }

            if(pressedkey[SDLK_F5]) {
                working_level.background_r = 1.0;
                working_level.background_g = 1.0;
                working_level.background_b = 1.0;
                working_level.textures.clear();
                working_level.collision_series.clear();
                for(unsigned int i=0;i<=7;i++)
                {
                    DeleteObjectinLevel(working_level.layers[i].statics,DELETE_ALL);
                    DeleteObjectinLevel(working_level.layers[i].entities,DELETE_ALL);
                }
                loaded_filename=NO_FILE;
                editor_status_list.add_message("Level Cleared");
            }

            if(pressedkey[SDLK_F12]&&loaded_filename!=NO_FILE) { //quickly save the opened level
                if(SaveLeveltoFile(loaded_filename,working_level)) editor_status_list.add_message("File saved: "+loaded_filename);
                else editor_status_list.add_message("ERROR: FILE NOT SAVED: "+loaded_filename);
            }


            if(pressedkey[SDLK_SPACE]&&!space_was_pressed_last_frame) {
                initial_mouse_is_in_pallete = mouse_is_in_pallete;
                space_was_pressed_last_frame = true;
                initial_grab_camera_x = camera_x;
                initial_grab_camera_y = camera_y;
                initial_cam_grab_mx = mx;
                initial_cam_grab_my = my;
                initial_texture_bar_grab_mx = mx;
                initial_pallete_scroll = pallete_scroll;
                }
            else if(!pressedkey[SDLK_SPACE]&&space_was_pressed_last_frame){
                space_was_pressed_last_frame = false;
            }

             if(event.type == SDL_MOUSEBUTTONDOWN&&event.button.button == SDL_BUTTON_WHEELDOWN){
                camera_zoom-=.05;
                if(camera_zoom<ZOOM_OUT_LIMIT) camera_zoom = ZOOM_OUT_LIMIT;
                }
            if(event.type == SDL_MOUSEBUTTONDOWN&&event.button.button == SDL_BUTTON_WHEELUP){
                camera_zoom+=.05;
                if(camera_zoom>ZOOM_IN_LIMIT) camera_zoom = ZOOM_IN_LIMIT;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN&&event.button.button == SDL_BUTTON_MIDDLE){
                camera_zoom=1.0;
            }

            while(!(SDL_GetAppState() & SDL_APPINPUTFOCUS)){ //Might want to move this, it isn't really an "input" per se, more of a lack thereof
                SDL_Delay(100);
                SDL_PollEvent(&event);
                if(event.type == SDL_VIDEOEXPOSE)
                {
                    SDL_GL_SwapBuffers();
                }
            }
}

void HandleStandardMode_CollisionVectorInput()
{
    using namespace main_editor;

        //////////////////////////////////////////////////////////////
        //STANDARD INPUT MODE - COLLISION VECTORS (Deprecated through a skip in the CTRL change mode code)
        //////////////////////////////////////////////////////////////
        if(object_editing_type==COLLISION_VECTOR&&!is_taking_typing_input&&!entity_editor_is_open){
            //active_layer = 0; //the player level
            mouse_is_in_pallete = false;
            ActiveCollisionSeriesIndex = NO_COLLISION_SERIES_ACTIVE;
            Closest_Collision_Point_Index = 0;
            Closest_Collision_Point = NULL;
            Hovered_Collision_Point = NULL;
            Hovered_Collision_Series = NULL;
            ObjectHoveredOverIndex = NO_OBJECT_HOVERED_OVER;
            ObjectGrabbedIndex = NO_OBJECT_GRABBED;
            Hovered_Object = NULL;
            is_object_hovered = false;
            Grabbed_Object = NULL;
            is_object_grabbed = false;
            if((event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_LCTRL)||(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_RCTRL)) object_editing_type = STATIC;
            if(working_level.collision_series.size()!=0){
                int closest_series_distance_squared = -1;
                for(unsigned int i = 0; i <working_level.collision_series.size();i++)
                {
                    int closest_point_index_in_this_series = working_level.collision_series[i].return_closest_point_index(mx_inworld,my_inworld);
                    CollisionPoint *temp = &working_level.collision_series[i].points[closest_point_index_in_this_series];
                    int this_series_distance_squared = GetDistanceSquared(temp->x,temp->y,mx_inworld,my_inworld);
                    if(this_series_distance_squared<closest_series_distance_squared||closest_series_distance_squared==-1){
                        if(this_series_distance_squared<20000){
                            closest_series_distance_squared = this_series_distance_squared;
                            Closest_Collision_Point_Index = closest_point_index_in_this_series;
                            ActiveCollisionSeriesIndex = i;
                            Closest_Collision_Point = temp;
                        }
                    }
                }
            }
            if(Closest_Collision_Point&&!is_adding_point){
                if(GetDistanceSquared(mx_inworld,my_inworld,Closest_Collision_Point->x,Closest_Collision_Point->y)<100) Hovered_Collision_Point = Closest_Collision_Point;
            }
            if(Hovered_Collision_Point&&!Grabbed_Collision_Point&&shift_is_pressed&&!is_adding_point)
            {
                Hovered_Collision_Series = &working_level.collision_series[ActiveCollisionSeriesIndex];
            }
            if(event.type == SDL_MOUSEBUTTONDOWN&&event.button.button == SDL_BUTTON_LEFT&&!is_adding_point)
            {
                if(Hovered_Collision_Series&&!Grabbed_Collision_Series){
                    Grabbed_Collision_Series = &working_level.collision_series[ActiveCollisionSeriesIndex];
                    initial_grab_offset_x = mx_inworld-Grabbed_Collision_Series->points[0].x;
                    initial_grab_offset_y = my_inworld-Grabbed_Collision_Series->points[0].y;
                }
                else if(!shift_is_pressed&&Hovered_Collision_Point&&!Grabbed_Collision_Point){
                    Grabbed_Collision_Point = Hovered_Collision_Point;
                }
            }
            if(event.type == SDL_MOUSEBUTTONUP&&event.button.button == SDL_BUTTON_LEFT&&!is_adding_point)
            {
                Grabbed_Collision_Point = NULL;
                Grabbed_Collision_Series = NULL;
            }
            if(pressedkey[SDLK_BACKSPACE]) is_adding_point = false;
            if(event.type == SDL_MOUSEBUTTONDOWN&&event.button.button == SDL_BUTTON_RIGHT&&!Grabbed_Collision_Point)
            {
                is_adding_point = true;
            }
            if(event.type == SDL_MOUSEBUTTONUP&&event.button.button == SDL_BUTTON_RIGHT&&!Grabbed_Collision_Point&&is_adding_point)
            {
                if(ActiveCollisionSeriesIndex!=NO_COLLISION_SERIES_ACTIVE) working_level.collision_series[ActiveCollisionSeriesIndex].addPoint(mx_inworld,my_inworld);
                else working_level.collision_series.push_back(CollisionSeries(mx_inworld,my_inworld));
                is_adding_point = false;
            }

            if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_DELETE&&Hovered_Collision_Point&&!Grabbed_Collision_Point){
                if(working_level.collision_series[ActiveCollisionSeriesIndex].points.size()==1||shift_is_pressed) working_level.collision_series.erase(working_level.collision_series.begin()+ActiveCollisionSeriesIndex);
                else working_level.collision_series[ActiveCollisionSeriesIndex].points.erase(working_level.collision_series[ActiveCollisionSeriesIndex].points.begin()+Closest_Collision_Point_Index);
            }

        }
}

void AddNewEntityToWorld()
{
    using namespace main_editor;
        int new_obj_x = mx_inworld;
        int new_obj_y = my_inworld;
        if(grid_snapping_is_on){
            new_obj_x = floor(new_obj_x/32)*32+16;
            new_obj_y = floor(new_obj_y/32)*32+16;
        }
        if(entity_types[active_entity_type_index] == "CollisionType"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_White_Button"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_White_Button(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_Black_Button"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_Black_Button(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_Grey_Button"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_Grey_Button(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_Puzzle_Barrier"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_Puzzle_Barrier(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_Red"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_Red(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_Green"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_Green(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_Death"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_Death(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_Death_Red"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_Death_Red(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "CollisionType_Death_Green"){
            working_level.layers[active_layer].entities.push_back( new Entity_CollisionType_Death_Green(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "ButtonType_White"){
            working_level.layers[active_layer].entities.push_back( new Entity_ButtonType_White(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "ButtonType_Black"){
            working_level.layers[active_layer].entities.push_back( new Entity_ButtonType_Black(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "ButtonType_Grey"){
            working_level.layers[active_layer].entities.push_back( new Entity_ButtonType_Grey(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "PlayerStartType_Red"){
            working_level.layers[active_layer].entities.push_back( new Entity_PlayerStartType_Red(new_obj_x,new_obj_y,38,32));
        }
        if(entity_types[active_entity_type_index] == "PlayerStartType_Green"){
            working_level.layers[active_layer].entities.push_back( new Entity_PlayerStartType_Green(new_obj_x,new_obj_y,32,50));
        }
        if(entity_types[active_entity_type_index] == "MarkerType"){
            working_level.layers[active_layer].entities.push_back( new Entity_MarkerType(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "ExitZoneType"){
            working_level.layers[active_layer].entities.push_back( new Entity_ExitZoneType(new_obj_x,new_obj_y,128,128));
        }
        if(entity_types[active_entity_type_index] == "DeathZoneType"){
            working_level.layers[active_layer].entities.push_back( new Entity_DeathZoneType(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "DeathLaserType"){
            working_level.layers[active_layer].entities.push_back( new Entity_DeathLaserType(new_obj_x,new_obj_y,32,32));
        }
        if(entity_types[active_entity_type_index] == "RespawnZoneType"){
            working_level.layers[active_layer].entities.push_back( new Entity_RespawnZoneType(new_obj_x,new_obj_y,128,128));
        }
        if(entity_types[active_entity_type_index] == "GrassType"&&working_level.textures.size()>0){
            Texture &Active_Texture = working_level.textures[active_texture_index];
            working_level.layers[active_layer].entities.push_back( new Entity_GrassType(new_obj_x,new_obj_y,Active_Texture.w,Active_Texture.h,active_texture_index));
        }
        //Particle system insertion is not complete yet, it just inserts a single system with set parameters.
        if(entity_types[active_entity_type_index] == "ParticleSystemType"&&working_level.textures.size()>0){
            Texture &Active_Texture = working_level.textures[active_texture_index];
            //Entity_ParticleSystemType test(new_obj_x,new_obj_y,Active_Texture.w,Active_Texture.h,active_texture_index);
            //test.set_parameters(1, 0, 10, 1, 3, 0, 2, 1, 10, 0, 2, 0, 199, 0, 24, 0, 1, 10, 20, 0, 3, 0, 1, 10, 20, 0, 3, 1, 3, 30, 30, 1.0, 0.0, 0.0, 0.0,
                                //1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);

            working_level.layers[active_layer].entities.push_back( new Entity_ParticleSystemType(new_obj_x,new_obj_y,Active_Texture.w,Active_Texture.h,active_texture_index));
        }

        if(entity_types[active_entity_type_index] == "SignType"&&working_level.textures.size()>0){
            Texture &Active_Texture = working_level.textures[active_texture_index];
            working_level.layers[active_layer].entities.push_back( new Entity_SignType(new_obj_x,new_obj_y,Active_Texture.w,Active_Texture.h,0,active_texture_index,1000));
            entity_editor_is_open = true;
            entity_editor_active_index = working_level.layers[active_layer].entities.size()-1;
        }
        if(entity_types[active_entity_type_index] == "TreeType"){
            working_level.layers[active_layer].entities.push_back( new Entity_TreeType(new_obj_x,new_obj_y,20));
            entity_editor_is_open = true;
            entity_editor_active_index = working_level.layers[active_layer].entities.size()-1;
        }
        //add the active entity type
}

void HandleStandardMode_StaticAndEntityInput()
{
    using namespace main_editor;
    if(object_editing_type==STATIC||object_editing_type==ENTITY)
        {
            if(!under_object_modifier_is_active){
                ObjectHoveredOverIndex = NO_OBJECT_HOVERED_OVER;
                Hovered_Object = NULL;
                is_object_hovered = false;
            }

        //Change Active Layer
        if(event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_COMMA) active_layer-=1;
        if(event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_PERIOD) active_layer+=1;
        active_layer = max(0, active_layer); //we need to clamp the active layer within the size of  array of layers
        active_layer = min(active_layer, 7);

        //Change Mouse Manipulation Mode
        if(pressedkey[SDLK_m]) manipulation_mode = MOVE;
        if(pressedkey[SDLK_r]) manipulation_mode = ROTATE;
        if(pressedkey[SDLK_s]) manipulation_mode = SCALE;
        if(!alt_is_pressed&&pressedkey[SDLK_c]) manipulation_mode = CLIP;


        //Change Between Entity Types
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_p&&object_editing_type==ENTITY){
            active_entity_type_index++;
            if(active_entity_type_index>NUMBER_OF_ENTITY_TYPES-1) active_entity_type_index = 0;
        }
        else if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_o&&object_editing_type==ENTITY){
            if(active_entity_type_index==0) active_entity_type_index = NUMBER_OF_ENTITY_TYPES-1;
            else active_entity_type_index--;

        }

        //Change Between Static and Entity Editing
        if((event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_LCTRL)||(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_RCTRL)){
            ObjectHoveredOverIndex = NO_OBJECT_HOVERED_OVER;
            ObjectGrabbedIndex = NO_OBJECT_GRABBED;
            Hovered_Object = NULL;
            is_object_hovered = false;
            Grabbed_Object = NULL;
            is_object_grabbed = false;
            under_object_modifier_is_active = false;
            if(object_editing_type == STATIC) object_editing_type = ENTITY;
            else object_editing_type = STATIC;
            //else object_editing_type = COLLISION_VECTOR; deprecating the collision vector mode for duet
        }
        if(object_editing_type == STATIC) working_level_statics_or_entities = &working_level.layers[active_layer].statics;
        else if(object_editing_type == ENTITY) working_level_statics_or_entities = &working_level.layers[active_layer].entities;
        if(entity_types[active_entity_type_index] != "t"||object_editing_type == STATIC){ //need to make sure the entity type isn't a tree
            if(my>SCREEN_HEIGHT-104&&!is_object_grabbed) {
                mouse_is_in_pallete = true;
                ObjectHoveredOverIndex = NO_OBJECT_HOVERED_OVER;
                Hovered_Object = NULL;
                is_object_hovered = false;
                under_object_modifier_is_active = false;
            }
            else mouse_is_in_pallete = false;
        }
        else mouse_is_in_pallete = false;


        //We want to find the object which is currently hovered over with the mouse

        //first, we need to clear out this variables so we can determine their truth value shortly
        left_side_is_hovered = false;
        right_side_is_hovered = false;
        top_side_is_hovered = false;
        bottom_side_is_hovered = false;

        if(under_object_modifier_is_active)
        {
            if(!IsPointInRectangle(mx_inworld,my_inworld,working_level_statics_or_entities->at(ObjectHoveredOverIndex)->bounds)) under_object_modifier_is_active = false;
            else
            {

                int temp_mx_inworld = (int)((mx/camera_zoom)+(camera_x*active_layer_parallax_mod*(Hovered_Object->parallax_offset))-in_game_camera_x);
                int temp_my_inworld = (int)((my/camera_zoom)+(camera_y*active_layer_parallax_mod*(Hovered_Object->parallax_offset))-in_game_camera_y);
                //we want to see which of the handles are currently hovered over by the mouse, so that we can draw them highlighted to aid usage
                if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,Hovered_Object->bounds.x,Hovered_Object->bounds.y,10,Hovered_Object->bounds.h))
                {
                    left_side_is_hovered = true;
                }
                if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,Hovered_Object->bounds.x,Hovered_Object->bounds.y,Hovered_Object->bounds.w,10))
                {
                    top_side_is_hovered = true;
                }
                if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,Hovered_Object->bounds.x+Hovered_Object->bounds.w-10,Hovered_Object->bounds.y,10,Hovered_Object->bounds.h))
                {
                    right_side_is_hovered = true;
                }
                if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,Hovered_Object->bounds.x,Hovered_Object->bounds.y+Hovered_Object->bounds.h-10,Hovered_Object->bounds.w,10))
                {
                    bottom_side_is_hovered = true;
                }
            }
            if(event.type == SDL_KEYUP&&event.key.keysym.sym == SDLK_UP) {
                for(unsigned int i=ObjectHoveredOverIndex+1;i<working_level_statics_or_entities->size()-1;i++)
                {
                    int temp_mx_inworld = (int)((mx/camera_zoom)+(camera_x*active_layer_parallax_mod*(working_level_statics_or_entities->at(i)->parallax_offset))-in_game_camera_x);
                    int temp_my_inworld = (int)((my/camera_zoom)+(camera_y*active_layer_parallax_mod*(working_level_statics_or_entities->at(i)->parallax_offset))-in_game_camera_y);
                    if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,working_level_statics_or_entities->at(i)->bounds)){
                        if(working_level_statics_or_entities->at(i)->type==entity_types[active_entity_type_index]||object_editing_type==STATIC){
                            is_object_hovered = true;
                            under_object_modifier_is_active = true;
                            Hovered_Object = working_level_statics_or_entities->at(i);
                            ObjectHoveredOverIndex = i;

                            break;
                        }
                }
            }
            }
        }
        else if(!mouse_is_in_pallete){
                for(int i=working_level_statics_or_entities->size()-1;i>-1;i--)
                {
                    int temp_mx_inworld = (int)((mx/camera_zoom)+(camera_x*active_layer_parallax_mod*(working_level_statics_or_entities->at(i)->parallax_offset))-in_game_camera_x);
                    int temp_my_inworld = (int)((my/camera_zoom)+(camera_y*active_layer_parallax_mod*(working_level_statics_or_entities->at(i)->parallax_offset))-in_game_camera_y);
                    if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,working_level_statics_or_entities->at(i)->bounds)){
                        if(working_level_statics_or_entities->at(i)->type==entity_types[active_entity_type_index]||object_editing_type==STATIC){
                            is_object_hovered = true;
                            Hovered_Object = working_level_statics_or_entities->at(i);
                            ObjectHoveredOverIndex = i;
                            //we want to see which of the handles are currently hovered over by the mouse, so that we can draw them highlighted to aid usage
                            if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,Hovered_Object->bounds.x,Hovered_Object->bounds.y,10,Hovered_Object->bounds.h))
                            {
                                left_side_is_hovered = true;
                            }
                            if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,Hovered_Object->bounds.x,Hovered_Object->bounds.y,Hovered_Object->bounds.w,10))
                            {
                                top_side_is_hovered = true;
                            }
                            if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,Hovered_Object->bounds.x+Hovered_Object->bounds.w-10,Hovered_Object->bounds.y,10,Hovered_Object->bounds.h))
                            {
                                right_side_is_hovered = true;
                            }
                            if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,Hovered_Object->bounds.x,Hovered_Object->bounds.y+Hovered_Object->bounds.h-10,Hovered_Object->bounds.w,10))
                            {
                                bottom_side_is_hovered = true;
                            }
                            break;
                        }
                    }
                }
        }

        //Change the currently selected object to the one that it is underneath the hovered one
        if(event.type == SDL_KEYUP&&event.key.keysym.sym == SDLK_DOWN&&is_object_hovered&&!is_object_grabbed){
            for(int i=ObjectHoveredOverIndex-1;i>-1;i--)
            {
                int temp_mx_inworld = (int)((mx/camera_zoom)+(camera_x*active_layer_parallax_mod*(working_level_statics_or_entities->at(i)->parallax_offset))-in_game_camera_x);
                int temp_my_inworld = (int)((my/camera_zoom)+(camera_y*active_layer_parallax_mod*(working_level_statics_or_entities->at(i)->parallax_offset))-in_game_camera_y);
                if(IsPointInRectangle(temp_mx_inworld,temp_my_inworld,working_level_statics_or_entities->at(i)->bounds)){
                    if(working_level_statics_or_entities->at(i)->type==entity_types[active_entity_type_index]||object_editing_type==STATIC){
                        is_object_hovered = true;
                        under_object_modifier_is_active = true;
                        Hovered_Object = working_level_statics_or_entities->at(i);
                        ObjectHoveredOverIndex = i;

                        break;
                    }
                }
            }

        }

        //Delete either the currently hovered object or a texture from the texture palette
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_DELETE)
        {
            if(mouse_is_in_pallete and working_level.textures.size())
            {
                int temp_hovered_texture = (mx + pallete_scroll) / texture_pallete_image_size;
                working_level.textures.erase(working_level.textures.begin() + temp_hovered_texture);
                //temp_hovered_texture+=1;
            for(int layer=7;layer>-1;layer--)
            {
                for(unsigned int i=0;i<working_level.layers[layer].statics.size();i++)
                {
                    Object *temp_static = working_level.layers[layer].statics[i];
                    if((temp_static->texture_index)>temp_hovered_texture) temp_static->texture_index-=1;
                    else if(temp_static->texture_index==temp_hovered_texture)
                    {
                        DeleteObjectinLevel((working_level.layers[layer].statics),i);
                        i-=1;
                    }
                }
            }


            }
            else if(is_object_hovered&&!is_object_grabbed){
                DeleteObjectinLevel(*working_level_statics_or_entities,ObjectHoveredOverIndex); //could cause an glitch/error if the * is not what I need
                if(under_object_modifier_is_active) under_object_modifier_is_active = false;
                Hovered_Object = NULL;
                ObjectHoveredOverIndex = NO_OBJECT_HOVERED_OVER;
                is_object_hovered = false;
            }
        }

        //Shift objects up and down in the depth queue
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_PAGEDOWN&&is_object_hovered&&!is_object_grabbed&&ObjectHoveredOverIndex!=0){
            Object* temp_object = working_level_statics_or_entities->at(ObjectHoveredOverIndex);
            working_level_statics_or_entities->at(ObjectHoveredOverIndex) = working_level_statics_or_entities->at(ObjectHoveredOverIndex-1);
            working_level_statics_or_entities->at(ObjectHoveredOverIndex-1) = temp_object;
        }
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_PAGEUP&&is_object_hovered&&!is_object_grabbed&&(unsigned)ObjectHoveredOverIndex!=working_level_statics_or_entities->size()-1){
            Object* temp_object = working_level_statics_or_entities->at(ObjectHoveredOverIndex);
            working_level_statics_or_entities->at(ObjectHoveredOverIndex) = working_level_statics_or_entities->at(ObjectHoveredOverIndex+1);
            working_level_statics_or_entities->at(ObjectHoveredOverIndex+1)= temp_object;
        }
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_END&&is_object_hovered&&!is_object_grabbed&&ObjectHoveredOverIndex!=0){
            Object* temp_object = working_level_statics_or_entities->at(ObjectHoveredOverIndex);
            working_level_statics_or_entities->erase(working_level_statics_or_entities->begin() + ObjectHoveredOverIndex);
            working_level_statics_or_entities->insert(working_level_statics_or_entities->begin(),temp_object);
        }
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_HOME&&is_object_hovered&&!is_object_grabbed&&(unsigned)ObjectHoveredOverIndex!=working_level.layers[active_layer].statics.size()-1){
            Object* temp_object = working_level_statics_or_entities->at(ObjectHoveredOverIndex);
            working_level_statics_or_entities->erase(working_level_statics_or_entities->begin() + ObjectHoveredOverIndex);
           working_level_statics_or_entities->insert(working_level_statics_or_entities->end(),temp_object);
        }

        //Duplicate the hovered object
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_w&&is_object_hovered&&!is_object_grabbed){
            if(object_editing_type == STATIC){
                Static* temp_object = reinterpret_cast<Static*>(working_level.layers[active_layer].statics.at(ObjectHoveredOverIndex));
                //working_level_statics_or_entities->insert(ObjectHoveredOverIndex+1,*temp_object);
                //this is probably hokey as hell and I'm missing something, but I'm just making a copy of the hovered object, just seems like the only good way to do it
                //is to reinterpret_cast the Static pointer in the statics array as a Static object and then
                working_level.layers[active_layer].statics.insert(working_level_statics_or_entities->begin() + ObjectHoveredOverIndex, new Static(*temp_object));
            }
            //working_level_statics_or_entities->at(ObjectHoveredOverIndex) = working_level_statics_or_entities->at(ObjectHoveredOverIndex+1);

            //working_level_statics_or_entities->at(ObjectHoveredOverIndex+1)= temp_object;
        }

            //Handle copy/paste settings behaviors
            if(alt_is_pressed&&event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_c)
            {
                Clipboard_Static = reinterpret_cast<Static*>(working_level.layers[active_layer].statics[ObjectHoveredOverIndex]);
                should_copy_static_settings = false;            }

            if(alt_is_pressed&&event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_v)
            {
                Static *active_static = reinterpret_cast<Static*>(working_level.layers[active_layer].statics[ObjectHoveredOverIndex]);
                active_static->r_mult = Clipboard_Static->r_mult;
                active_static->g_mult = Clipboard_Static->g_mult;
                active_static->b_mult = Clipboard_Static->b_mult;
                active_static->a_mult = Clipboard_Static->a_mult;

                active_static->r_add = Clipboard_Static->r_add;
                active_static->g_add = Clipboard_Static->g_add;
                active_static->b_add = Clipboard_Static->b_add;
                active_static->a_add = Clipboard_Static->a_add;

                active_static->r_blend = Clipboard_Static->r_blend;
                active_static->g_blend = Clipboard_Static->g_blend;
                active_static->b_blend = Clipboard_Static->b_blend;
                active_static->a_blend = Clipboard_Static->a_blend;

                active_static->hue = Clipboard_Static->hue;
                active_static->saturation = Clipboard_Static->saturation;
                active_static->lightness = Clipboard_Static->lightness;
                active_static->brightness = Clipboard_Static->brightness;
                active_static->contrast = Clipboard_Static->contrast;

                active_static->is_using_additive_tinting = Clipboard_Static->is_using_additive_tinting;
                active_static->is_using_colorize_hue = Clipboard_Static->is_using_colorize_hue;

                should_paste_static_settings = false;
            }


        //Open the entity editor
        else if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_e&&object_editing_type==ENTITY&&is_object_hovered&&!is_object_grabbed){
            entity_editor_is_open = true;
            entity_editor_active_index = ObjectHoveredOverIndex;
            //is_actually_editing_a_static = false;
        }
        else if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_e&&object_editing_type==STATIC&&is_object_hovered&&!is_object_grabbed){
            entity_editor_is_open = true;
            entity_editor_active_index = ObjectHoveredOverIndex;
            is_actually_editing_a_static = true;
        }
        else if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_e&&!is_object_grabbed){
            entity_editor_is_open = true;
            //entity_editor_active_index = ObjectHoveredOverIndex;
            is_actually_editing_global_settings = true;
        }

        //Toggle Grid Snapping
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_EQUALS){
            grid_snapping_is_on = (grid_snapping_is_on) ? false : true;

        }

        //Flip the hovered object horizontally
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_f){
            if(is_object_hovered){
                working_level_statics_or_entities->at(ObjectHoveredOverIndex)->is_flipped_horizontally = (working_level_statics_or_entities->at(ObjectHoveredOverIndex)->is_flipped_horizontally) ? false: true;
            }
        }

        //Adjust the parallax offset of the currently hovered object
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_9 && is_object_hovered)
        {
            //we want to make sure the object remains in relatively the same place on screen when the user adjusts the parallax
            int initial_x = Hovered_Object->x-(camera_x*active_layer_parallax_mod*Hovered_Object->parallax_offset);
            int initial_y = Hovered_Object->y-(camera_y*active_layer_parallax_mod*Hovered_Object->parallax_offset);

            if(shift_is_pressed) Hovered_Object->parallax_offset-=.1;
            else Hovered_Object->parallax_offset-=.01;

            int new_x = Hovered_Object->x-(camera_x*active_layer_parallax_mod*Hovered_Object->parallax_offset);
            int new_y = Hovered_Object->y-(camera_y*active_layer_parallax_mod*Hovered_Object->parallax_offset);

            Hovered_Object->move(Hovered_Object->x-(new_x-initial_x),Hovered_Object->y-(new_y-initial_y));
        }
        if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_0 && is_object_hovered)
        {
            //we want to make sure the object remains in relatively the same place on screen when the user adjusts the parallax
            int initial_x = Hovered_Object->x-(camera_x*active_layer_parallax_mod*Hovered_Object->parallax_offset);
            int initial_y = Hovered_Object->y-(camera_y*active_layer_parallax_mod*Hovered_Object->parallax_offset);

            if(shift_is_pressed) Hovered_Object->parallax_offset+=.1;
            else Hovered_Object->parallax_offset+=.01;

            int new_x = Hovered_Object->x-(camera_x*active_layer_parallax_mod*Hovered_Object->parallax_offset);
            int new_y = Hovered_Object->y-(camera_y*active_layer_parallax_mod*Hovered_Object->parallax_offset);

            Hovered_Object->move(Hovered_Object->x-(new_x-initial_x),Hovered_Object->y-(new_y-initial_y));
        }

        //Handle Object Grabbing
        if(event.type == SDL_MOUSEBUTTONDOWN&&event.button.button == SDL_BUTTON_LEFT){
            if(is_object_hovered&&!is_object_grabbed){
                ObjectGrabbedIndex = ObjectHoveredOverIndex; //Pick up the static, or rotate, or whatever
                Grabbed_Object = working_level_statics_or_entities->at(ObjectHoveredOverIndex);
                is_object_grabbed = true;
                if(manipulation_mode==MOVE){
                    initial_grab_offset_x = mx_inworld-Grabbed_Object->x;
                    initial_grab_offset_y = my_inworld-Grabbed_Object->y;
                }
                if(manipulation_mode==SCALE){
                    initial_grab_mx=mx_inworld;
                    initial_grab_my=my_inworld;
                    initial_grab_offset_x = mx_inworld-Grabbed_Object->x;
                    initial_grab_offset_y = my_inworld-Grabbed_Object->y;
                    initial_grab_object_w = Grabbed_Object->w;
                    initial_grab_object_h = Grabbed_Object->h;
                    initial_grab_object_bounds = Grabbed_Object->bounds;
                    initial_grab_object_x = Grabbed_Object->x;
                    initial_grab_object_y = Grabbed_Object->y;

                    left_side_is_grabbed = false;
                    right_side_is_grabbed = false;
                    top_side_is_grabbed = false;
                    bottom_side_is_grabbed = false;

                    if(left_side_is_hovered) left_side_is_grabbed = true;
                    if(top_side_is_hovered) top_side_is_grabbed = true;
                    if(right_side_is_hovered) right_side_is_grabbed = true;
                    if(bottom_side_is_hovered) bottom_side_is_grabbed = true;

                }
                if(manipulation_mode==ROTATE){
                    initial_grab_mx=mx_inworld;
                    initial_grab_my=my_inworld;
                    initial_grab_angle=atan2deg((my_inworld-Grabbed_Object->y),(mx_inworld-Grabbed_Object->x));
                    initial_grab_object_rotation = Grabbed_Object->rotation;
                }

                if(manipulation_mode==CLIP){
                    /*if(shift_is_pressed)
                    {
                        initial_grab_u = Grabbed_Object->u2;
                        initial_grab_v = Grabbed_Object->v2;
                        is_grabbing_secondary_uv = true;

                    }
                    else
                    {
                        initial_grab_u = Grabbed_Object->u;
                        initial_grab_v = Grabbed_Object->v;
                        is_grabbing_secondary_uv = false;
                    }*/
                    initial_grab_mx = mx_inworld;
                    initial_grab_my = my_inworld;
                    initial_grab_object_u = Grabbed_Object->u;
                    initial_grab_object_v = Grabbed_Object->v;
                    initial_grab_object_u2 = Grabbed_Object->u2;
                    initial_grab_object_v2 = Grabbed_Object->v2;
                    initial_grab_object_bounds = Grabbed_Object->bounds;
                    initial_grab_object_w = Grabbed_Object->w;
                    initial_grab_object_h = Grabbed_Object->h;
                    initial_grab_offset_x = mx_inworld-Grabbed_Object->x;
                    initial_grab_offset_y = my_inworld-Grabbed_Object->y;
                    initial_grab_object_x = Grabbed_Object->x;
                    initial_grab_object_y = Grabbed_Object->y;

                    //now we want to see which handles we are manipulating. There are four handles, creating a 10px border around the asset
                    //it is possible to grab two handles at once, and grab from a corner.

                    left_side_is_grabbed = false;
                    right_side_is_grabbed = false;
                    top_side_is_grabbed = false;
                    bottom_side_is_grabbed = false;

                    if(left_side_is_hovered) left_side_is_grabbed = true;
                    if(top_side_is_hovered) top_side_is_grabbed = true;
                    if(right_side_is_hovered) right_side_is_grabbed = true;
                    if(bottom_side_is_hovered) bottom_side_is_grabbed = true;

                }
            }
        }

        //Handle Dropping objects and interacting with the texture Palette
        if(event.type == SDL_MOUSEBUTTONUP&&event.button.button == SDL_BUTTON_LEFT)
        {
            if(!mouse_is_in_pallete){
            ObjectGrabbedIndex = NO_OBJECT_GRABBED; //drop the static, stop manipulating it
            Grabbed_Object = NULL;
            is_object_grabbed = false;
            }
            else{
                if(unsigned((mx + pallete_scroll) / texture_pallete_image_size)<=working_level.textures.size()-1&&working_level.textures.size()>0){
                    active_texture_index = (mx + pallete_scroll) / texture_pallete_image_size;
                }
                if(unsigned((mx + pallete_scroll) / texture_pallete_image_size)>working_level.textures.size()-1||working_level.textures.size()==0){
                    is_taking_typing_input = true;
                    input_mode = INPUT_LOAD_TEXTURE;
                }
            }
        }

        //Create new Objects
        if(event.type == SDL_MOUSEBUTTONDOWN&&event.button.button == SDL_BUTTON_RIGHT){
            if(object_editing_type == STATIC&&working_level.textures.size()>0)
            {
                Texture &Active_Texture = working_level.textures[active_texture_index];
                working_level.layers[active_layer].statics.push_back( new Static(mx_inworld,my_inworld,Active_Texture.w,Active_Texture.h,0,active_texture_index));
            }
            else if(object_editing_type == ENTITY)
            {
                AddNewEntityToWorld();
            }
        }
    }
}

void HandleStandardModeInput()
{
    HandleStandardMode_GlobalInput();
    HandleStandardMode_CollisionVectorInput();
    HandleStandardMode_StaticAndEntityInput();
}

void HandleMultiFrameInput()
{
    using namespace main_editor;
        if(Grabbed_Collision_Point) {
            Grabbed_Collision_Point->move(mx_inworld,my_inworld);
        }
        if(Grabbed_Collision_Series) {
            Grabbed_Collision_Series->move(mx_inworld-initial_grab_offset_x,my_inworld-initial_grab_offset_y);
        }

        if(is_object_grabbed==true){
            Grabbed_Object = working_level_statics_or_entities->at(ObjectGrabbedIndex);
            if(manipulation_mode==MOVE){
                point_2d new_position(mx_inworld-initial_grab_offset_x,my_inworld-initial_grab_offset_y);
                if(grid_snapping_is_on) {
                    new_position.x = mx_inworld-initial_grab_offset_x-Grabbed_Object->w/2;
                    new_position.y = my_inworld-initial_grab_offset_y-Grabbed_Object->h/2;
                    new_position.x = nearbyint((new_position.x)/32)*32+Grabbed_Object->w/2;
                    new_position.y = nearbyint((new_position.y)/32)*32+Grabbed_Object->h/2;
                }
                if(pressedkey[SDLK_x]) Grabbed_Object->move(new_position.x,Grabbed_Object->y);
                else if(pressedkey[SDLK_y]) Grabbed_Object->move(Grabbed_Object->x,new_position.y);
                else Grabbed_Object->move(new_position.x,new_position.y);
            }
            if(manipulation_mode==SCALE){
                if(grid_snapping_is_on)
                {
                    int initial_x1, initial_y1;
                    initial_x1 = (initial_grab_mx-initial_grab_offset_x)-(initial_grab_object_w/2);
                    initial_y1 = (initial_grab_my-initial_grab_offset_y)-(initial_grab_object_h/2);
                    int x1, x2, y1, y2;
                    //x1 = nearbyint((Grabbed_Object->x-(Grabbed_Object->w/2))/32)*32;
                    //y1 = nearbyint((Grabbed_Object->y-(Grabbed_Object->h/2))/32)*32;
                    x1 = nearbyint(initial_x1/32)*32;
                    y1 = nearbyint(initial_y1/32)*32;
                    x2 = nearbyint((mx_inworld)/32)*32;
                    y2 = nearbyint((my_inworld)/32)*32;
                    if(x2==x1) {if(mx_inworld>x1) {x2 = x1+32;} else {x2 = x1-32;}}
                    if(y2==y1) {if(my_inworld>y1) {y2 = y1+32;} else {y2 = y1-32;}}
                    if(x2<x1) {int x3 = x1; x1 = x2; x2 = x3;}
                    if(y2<y1) {int y3 = y1; y1 = y2; y2 = y3;}

                    Grabbed_Object->move((x1+x2)/2,(y1+y2)/2);
                    Grabbed_Object->scale(abs(x2-x1),abs(y2-y1));
                }
                else{
                    if(!left_side_is_grabbed and !right_side_is_grabbed and !top_side_is_grabbed and !bottom_side_is_grabbed){
                        int dist_mx_from_obj_x = abs(Grabbed_Object->x-mx_inworld);
                        int dist_my_from_obj_y = abs(Grabbed_Object->y-my_inworld);
                        int initial_dist_mx_from_obj_x = abs(Grabbed_Object->x-initial_grab_mx);
                        int initial_dist_my_from_obj_y = abs(Grabbed_Object->y-initial_grab_my);
                        if(shift_is_pressed){
                            int overall_mouse_dist_from_object_center = (int)(sqrt((dist_mx_from_obj_x*dist_mx_from_obj_x)+(dist_my_from_obj_y*dist_mx_from_obj_x)));
                            int overall_initial_mouse_dist_from_object_center = (int)(sqrt((initial_dist_mx_from_obj_x*initial_dist_mx_from_obj_x)+(initial_dist_my_from_obj_y*initial_dist_mx_from_obj_x)));
                            int scaled_size = overall_mouse_dist_from_object_center-overall_initial_mouse_dist_from_object_center;
                            Grabbed_Object->scale(initial_grab_object_w+scaled_size,(int)(initial_grab_object_h+(scaled_size*((float)(initial_grab_object_h)/(float)(initial_grab_object_w)))));
                        }
                        else{
                            int scaled_width = abs(initial_grab_object_w+(dist_mx_from_obj_x-initial_dist_mx_from_obj_x));
                            int scaled_height = abs(initial_grab_object_h+(dist_my_from_obj_y-initial_dist_my_from_obj_y));
                            Grabbed_Object->scale(scaled_width,scaled_height);
                        }
                    }
                    else
                    {
                        int new_width = initial_grab_object_w, new_height = initial_grab_object_h, new_center_x = initial_grab_object_x, new_center_y = initial_grab_object_y;

                        if(left_side_is_grabbed)
                        {

                            int starting_x1 = initial_grab_object_bounds.x;
                            int starting_x2 = starting_x1 + initial_grab_object_w;

                            //first, we want to make sure that the object is resized properly when it is grabbed by the edges
                            int x_offset = mx_inworld-initial_grab_mx;
                            int new_x1 = starting_x1 + x_offset;

                            new_width = starting_x2 - new_x1;
                            new_center_x = (new_x1 + (starting_x2))/2;

                        }
                        if(right_side_is_grabbed)
                        {

                            int starting_x1 = initial_grab_object_bounds.x;
                            int starting_x2 = starting_x1 + initial_grab_object_w;

                            //first, we want to make sure that the object is resized properly when it is grabbed by the edges
                            int x_offset = mx_inworld-initial_grab_mx;
                            int new_x2 = starting_x2 + x_offset;

                            new_width =  new_x2 - starting_x1;
                            new_center_x = (starting_x1 + (new_x2))/2;

                        }
                        if(top_side_is_grabbed)
                        {

                            int starting_y1 = initial_grab_object_bounds.y;
                            int starting_y2 = starting_y1 + initial_grab_object_h;

                            //first, we want to make sure that the object is resized properly when it is grabbed by the edges
                            int y_offset = my_inworld-initial_grab_my;
                            int new_y1 = starting_y1 + y_offset;

                            new_height = starting_y2 - new_y1;
                            new_center_y = (new_y1 + (starting_y2))/2;

                        }
                        if(bottom_side_is_grabbed)
                        {

                            int starting_y1 = initial_grab_object_bounds.y;
                            int starting_y2 = starting_y1 + initial_grab_object_h;

                            //first, we want to make sure that the object is resized properly when it is grabbed by the edges
                            int y_offset = my_inworld-initial_grab_my;
                            int new_y2 = starting_y2 + y_offset;

                            new_height = new_y2 - starting_y1;
                            new_center_y = (starting_y1 + (new_y2))/2;

                        }

                        new_width = abs(new_width);
                        new_height = abs(new_height);

                        Grabbed_Object->scale(new_width,new_height);
                        Grabbed_Object->move(new_center_x, new_center_y);

                    }
                }
            }
            if(manipulation_mode==ROTATE){
                float adjusted_rotation = (atan2deg((my_inworld-Grabbed_Object->y),(mx_inworld-Grabbed_Object->x)));
                adjusted_rotation=adjusted_rotation-initial_grab_angle+initial_grab_object_rotation;
                if(adjusted_rotation>360) adjusted_rotation-=360;
                if(adjusted_rotation<0) adjusted_rotation+=360;
                if(shift_is_pressed) adjusted_rotation=(int)((adjusted_rotation/45))*45;
                Grabbed_Object->rotate(adjusted_rotation);
            }

            if(manipulation_mode==CLIP){

                int new_width = initial_grab_object_w, new_height = initial_grab_object_h, new_center_x = initial_grab_object_x, new_center_y = initial_grab_object_y;
                float amount_to_adjust_u = 0, amount_to_adjust_v = 0, amount_to_adjust_u2 = 0, amount_to_adjust_v2 = 0;
                if(!left_side_is_grabbed and !right_side_is_grabbed and !top_side_is_grabbed and !bottom_side_is_grabbed)
                {
                    float u_width = initial_grab_object_u2 - initial_grab_object_u;
                    float v_height = initial_grab_object_v2 - initial_grab_object_v;
                    point_2d new_position(mx_inworld-initial_grab_offset_x,my_inworld-initial_grab_offset_y);
                    float u_offset = ((float)(Grabbed_Object->x-new_position.x)/Grabbed_Object->w)*u_width;
                    if(Grabbed_Object->is_flipped_horizontally) u_offset*=-1;
                    float v_offset = ((float)(Grabbed_Object->y-new_position.y)/Grabbed_Object->h)*v_height;
                    Grabbed_Object->u = u_offset+initial_grab_object_u;
                    Grabbed_Object->v = v_offset+initial_grab_object_v;
                    Grabbed_Object->u2 = Grabbed_Object->u+u_width;
                    Grabbed_Object->v2 = Grabbed_Object->v+v_height;

                }
                else{
                    if(left_side_is_grabbed)
                    {

                        int starting_x1 = initial_grab_object_bounds.x;
                        int starting_x2 = starting_x1 + initial_grab_object_w;

                        //first, we want to make sure that the object is resized properly when it is grabbed by the edges
                        int x_offset = mx_inworld-initial_grab_mx;
                        int new_x1 = starting_x1 + x_offset;

                        new_width = starting_x2 - new_x1;

                        new_center_x = (new_x1 + (starting_x2))/2;

                        //secondly, we need to account for the resize by adjusting the relative uv coordinate
                        //this way, the image stays the same size on screen, and is just clipped
                        //without this, it's just a scaling operation, as usual
                        float percent_of_width_change = (float)x_offset/(float)initial_grab_object_w;
                        float u_width = initial_grab_object_u2 - initial_grab_object_u;

                        amount_to_adjust_u = percent_of_width_change*u_width;
                    }
                    if(right_side_is_grabbed)
                    {

                        int starting_x1 = initial_grab_object_bounds.x;
                        int starting_x2 = starting_x1 + initial_grab_object_w;

                        //first, we want to make sure that the object is resized properly when it is grabbed by the edges
                        int x_offset = mx_inworld-initial_grab_mx;
                        int new_x2 = starting_x2 + x_offset;

                        new_width =  new_x2 - starting_x1;

                        new_center_x = (starting_x1 + (new_x2))/2;

                        //secondly, we need to account for the resize by adjusting the relative uv coordinate
                        //this way, the image stays the same size on screen, and is just clipped
                        //without this, it's just a scaling operation, as usual
                        float percent_of_width_change = (float)x_offset/(float)initial_grab_object_w;
                        float u_width = initial_grab_object_u2 - initial_grab_object_u;

                        amount_to_adjust_u2 = percent_of_width_change*u_width;
                    }
                    if(top_side_is_grabbed)
                    {

                        int starting_y1 = initial_grab_object_bounds.y;
                        int starting_y2 = starting_y1 + initial_grab_object_h;

                        //first, we want to make sure that the object is resized properly when it is grabbed by the edges
                        int y_offset = my_inworld-initial_grab_my;
                        int new_y1 = starting_y1 + y_offset;

                        new_height = starting_y2 - new_y1;

                        new_center_y = (new_y1 + (starting_y2))/2;

                        //secondly, we need to account for the resize by adjusting the relative uv coordinate
                        //this way, the image stays the same size on screen, and is just clipped
                        //without this, it's just a scaling operation, as usual
                        float percent_of_height_change = (float)y_offset/(float)initial_grab_object_h;
                        float v_height = initial_grab_object_v2 - initial_grab_object_v;

                        amount_to_adjust_v = percent_of_height_change*v_height;

                    }
                    if(bottom_side_is_grabbed)
                    {

                        int starting_y1 = initial_grab_object_bounds.y;
                        int starting_y2 = starting_y1 + initial_grab_object_h;

                        //first, we want to make sure that the object is resized properly when it is grabbed by the edges
                        int y_offset = my_inworld-initial_grab_my;
                        int new_y2 = starting_y2 + y_offset;

                        new_height = new_y2 - starting_y1;

                        new_center_y = (starting_y1 + (new_y2))/2;

                        //secondly, we need to account for the resize by adjusting the relative uv coordinate
                        //this way, the image stays the same size on screen, and is just clipped
                        //without this, it's just a scaling operation, as usual
                        float percent_of_height_change = (float)y_offset/(float)initial_grab_object_h;
                        float v_height = initial_grab_object_v2 - initial_grab_object_v;

                        amount_to_adjust_v2 = percent_of_height_change*v_height;


                    }

                        //if(new_width<4) new_width = 4;
                        //if(new_height<4) new_height = 4;
                        Grabbed_Object->scale(new_width,new_height);
                        Grabbed_Object->move(new_center_x, new_center_y);
                        if(Grabbed_Object->is_flipped_horizontally)
                        {
                            Grabbed_Object->u=initial_grab_object_u-amount_to_adjust_u2;
                            Grabbed_Object->u2=initial_grab_object_u2-amount_to_adjust_u;
                        }
                        else
                        {
                            Grabbed_Object->u=initial_grab_object_u+amount_to_adjust_u;
                            Grabbed_Object->u2=initial_grab_object_u2+amount_to_adjust_u2;
                        }
                        Grabbed_Object->v=initial_grab_object_v+amount_to_adjust_v;
                        Grabbed_Object->v2=initial_grab_object_v2+amount_to_adjust_v2;
                }

            }

        }
        if(space_was_pressed_last_frame){
            if(!initial_mouse_is_in_pallete){
                camera_x = initial_grab_camera_x - ((mx-initial_cam_grab_mx)/camera_zoom);
                camera_y = initial_grab_camera_y - ((my-initial_cam_grab_my)/camera_zoom);
            }
            else if(working_level.textures.size()>0) {
                pallete_scroll = initial_pallete_scroll - ((mx-initial_texture_bar_grab_mx)*2.5);
                if(pallete_scroll<0) pallete_scroll = 0;
                if((unsigned)pallete_scroll>(working_level.textures.size()-4)*texture_pallete_image_size) pallete_scroll = (working_level.textures.size()-4)*texture_pallete_image_size;
            }
        }

}

void HandleInput()
{
    using namespace main_editor;
            pressedkey = SDL_GetKeyState(NULL);

        while(SDL_PollEvent(&event)){
            if(entity_editor_is_open&&!is_taking_typing_input){
                    HandleEntityEditorModeInput();
            }
            else if (is_taking_typing_input == true){
                    HandleLoadingModeInput();
            }
            else if(!is_taking_typing_input&&!entity_editor_is_open){
                    HandleStandardModeInput();
            }
        }
    HandleMultiFrameInput();
}

void DrawStaticsAndEntities()
{

    using namespace main_editor;
        //draw the level
        for(int layer=7;layer>-1;layer--)
        {

            glColor4f(1.0,1.0,1.0,1.0);
            //Draw all of the statics
            for(int i=0;(unsigned int)(i)<working_level.layers[layer].statics.size();i++)
            {
                glLoadIdentity();
                //adjust for the camera
                Static* active_static = reinterpret_cast<Static*>(working_level.layers[layer].statics[i]);
                glScalef(camera_zoom,camera_zoom,0);
                glTranslatef(-camera_x*(working_level.layers[layer].parallax_modifier)*(active_static->parallax_offset),-camera_y*(working_level.layers[layer].parallax_modifier)*(active_static->parallax_offset),0);
                glTranslatef(in_game_camera_x,in_game_camera_y,0);
                glPushMatrix();
                glTranslatef(active_static->x,active_static->y,0);
                glRotatef( active_static->rotation, 0.0, 0.0, 1.0 );
                if(active_static->is_flipped_horizontally) glScalef(-(active_static->w),active_static->h,0);
                else glScalef(active_static->w,active_static->h,0);
                glColor4f(1.0,1.0,1.0,1.0);

                if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                {
                    if(layer==active_layer&&object_editing_type==STATIC&&!entity_editor_is_open){
                        glDisable(GL_TEXTURE_2D);
                        glColor4f(0.0,1.0,0.0,0.2);
                        glCallList(square);
                    }
                }

                if(layer!=2 and object_editing_type==ENTITY and active_entity_type_index <=6 ) glColor4f(1.0,1.0,1.0,0.2);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, working_level.textures[active_static->texture_index].GLtexture);

                    glUseProgram(simple_glsl_program);

                    glUniform4f(mult_color,active_static->r_mult, active_static->g_mult, active_static->b_mult, active_static->a_mult);
                    glUniform4f(add_color,active_static->r_add, active_static->g_add, active_static->b_add, active_static->a_add);
                    glUniform3f(blend_color,active_static->r_blend, active_static->g_blend, active_static->b_blend);
                    glUniform1f(blend_density,active_static->a_blend);

                    glUniform3f(hsl,active_static->hue, active_static->saturation, active_static->lightness);
                    glUniform1f(brightness, active_static->brightness);
                    glUniform1f(contrast, active_static->contrast);

                    glUniform1f(colorize, active_static->is_using_colorize_hue);

                    if(!is_game_preview_mode_on)
                    {
                        if(layer!=active_layer) glUniform1f(brightness,0.1);
                        if(layer<active_layer) glUniform4f(mult_color,0.3, 0.3, 0.3, 0.0);
                        if(layer==active_layer and i!=ObjectHoveredOverIndex) glUniform1f(brightness,active_static->brightness*0.7);
                    }
                    else
                    {
                        //if(layer==active_layer and i==ObjectHoveredOverIndex) glUniform1f(brightness,1.2);
                    }
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
            //Entities need to be drawn differently based on type
            for(int i=0;(unsigned int)(i)<working_level.layers[layer].entities.size();i++)
            {
                glLoadIdentity();
                glColor4f(1.0,1.0,1.0,1.0);
                if(!is_game_preview_mode_on){
                    if(working_level.layers[layer].entities[i]->type!=entity_types[active_entity_type_index]&&object_editing_type==ENTITY) glColor4f(0.8,0.9,0.8,0.5);
                    else if(layer!=active_layer) glColor4f(0.3,0.3,0.3,1.0);
                }
                if(working_level.layers[layer].entities[i]->type=="CollisionType" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_TYPE
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.1);
                            glCallList(square);
                            //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            //glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    glColor4f(1.0,1.0,1.0,0.6);
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(1.0,1.0,1.0,0.4);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, collision_box.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_White_Button" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_WHITE_BUTTON
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, collision_box_white.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_Black_Button" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_BLACK_BUTTON
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, collision_box_black.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_Grey_Button" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_GREY_BUTTON
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, collision_box_grey.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_Red" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_RED
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, collision_box_red.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_Green" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_GREEN
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, collision_box_green.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_Death" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_GREEN
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    //glColor4f(1.0f,0.0f,0.0f,1.0f);
                    glBindTexture(GL_TEXTURE_2D, death_zone.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_Death_Red" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_GREEN
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glColor4f(1.0f,0.0f,0.0f,1.0f);
                    glBindTexture(GL_TEXTURE_2D, death_zone.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_Death_Green" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_GREEN
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glColor4f(0.0f,1.0f,0.0f,1.0f);
                    glBindTexture(GL_TEXTURE_2D, death_zone.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="CollisionType_Puzzle_Barrier" and ((object_editing_type==ENTITY and active_entity_type_index <9) or are_collision_boxes_always_shown)){ //COLLISION_GREEN
                    Entity_CollisionType_Puzzle_Barrier* active_entity = reinterpret_cast<Entity_CollisionType_Puzzle_Barrier*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, collision_box.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="ButtonType_White" and object_editing_type==ENTITY){ //BUTTON_WHITE
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, button_white_up.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="ButtonType_Black" and object_editing_type==ENTITY){ //BUTTON_BLACK
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, button_black_up.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="ButtonType_Grey" and object_editing_type==ENTITY){ //BUTTON_GREY
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, button_grey_up.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="PlayerStartType_Red" and object_editing_type==ENTITY){
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, player_red.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="PlayerStartType_Green" and object_editing_type==ENTITY){
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, player_green.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="MarkerType" and object_editing_type==ENTITY){
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, marker.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="RespawnZoneType" and object_editing_type==ENTITY){
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, respawn_zone.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="DeathZoneType" and object_editing_type==ENTITY){
                    Entity_DeathZoneType* active_entity = reinterpret_cast<Entity_DeathZoneType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, death_zone.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="ExitZoneType" and object_editing_type==ENTITY){
                    Entity_CollisionType* active_entity = reinterpret_cast<Entity_CollisionType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, exit_zone.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="DeathLaserType" and object_editing_type==ENTITY){
                    Entity_DeathLaserType* active_entity = reinterpret_cast<Entity_DeathLaserType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, death_laser.GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }

                else if(working_level.layers[layer].entities[i]->type=="ParticleSystemType"){ //PARTICLE_SYSTEM
                    Entity_ParticleSystemType* active_entity = reinterpret_cast<Entity_ParticleSystemType*>(working_level.layers[layer].entities[i]);
                    active_entity->update();
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glPushMatrix();
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                        if(layer==active_layer&&object_editing_type==ENTITY&&!is_game_preview_mode_on){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.1);
                            glCallList(square);
                            glColor4f(1.0,1.0,1.0,1.0);
                        }
                    glPopMatrix();
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY&&!is_game_preview_mode_on){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.3);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            //glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                 //if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, working_level.textures[active_entity->texture_index].GLtexture);
                    for(unsigned int i = 0; i<active_entity->particles.size()+1;i++)
                    {
                        if(active_entity->particles.empty()) break;
                    glPushMatrix();
                    glTranslatef(active_entity->particles[i].x,active_entity->particles[i].y,0);
                    glRotatef( active_entity->particles[i].rot, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->particles[i].w,active_entity->particles[i].h,0);
                    glColor4f(1.0,1.0,1.0,1.0);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                    }
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="GrassType"){ //GRASS_TYPE
                    Entity_GrassType* active_entity = reinterpret_cast<Entity_GrassType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, working_level.textures[active_entity->texture_index].GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="SignType"){ //SIGN_TYPE
                    Entity_SignType* active_entity = reinterpret_cast<Entity_SignType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glPushMatrix();
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glRotatef( active_entity->rotation, 0.0, 0.0, 1.0 );
                    glScalef(active_entity->w,active_entity->h,0);
                    if((i==ObjectHoveredOverIndex&&!is_object_grabbed)||(i==ObjectGrabbedIndex))
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(0.0,0.0,1.0,0.2);
                            glCallList(square);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, working_level.textures[active_entity->texture_index].GLtexture);
                    glCallList(square);
                    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
                    glPopMatrix();
                }
                else if(working_level.layers[layer].entities[i]->type=="TreeType"){ //TREE_TYPE
                    Entity_TreeType* active_entity = reinterpret_cast<Entity_TreeType*>(working_level.layers[layer].entities[i]);
                    glScalef(camera_zoom,camera_zoom,0);
                    glTranslatef(-camera_x*working_level.layers[layer].parallax_modifier,-camera_y*working_level.layers[layer].parallax_modifier,0);
                    glTranslatef(in_game_camera_x,in_game_camera_y,0);
                    glTranslatef(active_entity->x,active_entity->y,0);
                    glPushMatrix();
                    /*if(i==ObjectHoveredOverIndex&&!is_object_grabbed||i==ObjectGrabbedIndex)
                    {
                        if(layer==active_layer&&object_editing_type==ENTITY){
                            DrawGLRectSolidFromCurrentMatrix(square2,-active_entity->bounds.w/2,-active_entity->bounds.h/2,active_entity->bounds.w,active_entity->bounds.h,0.0,0.0,1.0,0.2);
                            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
                            glColor4f(0.05f,0.05f,0.05f,1.0);
                        }
                    }*/
                    if(i==ObjectGrabbedIndex&&layer==active_layer&&object_editing_type==ENTITY) glColor4f(0.1,0.1,0.1,0.9);
                    active_entity->draw(square2,1.0,1.0,0.0,1.0);
                    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
                    glPopMatrix();

                }

            }
            glColor4f(1.0,1.0,1.0,1.0);
        }
}

void DrawCollisionVectors()
{
    using namespace main_editor;
     //Draw Collision Vectors
    if(object_editing_type == COLLISION_VECTOR)
    {
        glLoadIdentity();
        glScalef(camera_zoom,camera_zoom,0);
        glTranslatef(-camera_x,-camera_y,0);
        glTranslatef(in_game_camera_x,in_game_camera_y,0);
        if(working_level.collision_series.size()>0){
            for(unsigned int i = 0; i<working_level.collision_series.size();i++)
            {
                working_level.collision_series[i].draw(square2);
            }
            if(Hovered_Collision_Series&&!Grabbed_Collision_Series) Hovered_Collision_Series->draw(square2,1.0,0.0,0.0,0.8);
            if(Grabbed_Collision_Series) Grabbed_Collision_Series->draw(square2,1.0,0.0,0.0,0.8);
            if(Hovered_Collision_Point&&!Hovered_Collision_Series) DrawGLRectSolidFromCurrentMatrix(square2,(Hovered_Collision_Point->x)-6,(Hovered_Collision_Point->y)-6,12,12,1.0,0.0,0.0,0.8);
            if(is_adding_point&&Closest_Collision_Point){
                CollisionPoint temp_point = working_level.collision_series[ActiveCollisionSeriesIndex].points[Closest_Collision_Point_Index];
                if(Closest_Collision_Point_Index == 0 || (unsigned)Closest_Collision_Point_Index == working_level.collision_series[ActiveCollisionSeriesIndex].points.size()-1){
                    DrawGLLineFromCurrentMatrix(mx_inworld,my_inworld,temp_point.x,temp_point.y,0.0,1.0,1.0,0.6);
                }
                else{
                    DrawGLLineFromCurrentMatrix(working_level.collision_series[ActiveCollisionSeriesIndex].points[Closest_Collision_Point_Index-1].x,working_level.collision_series[ActiveCollisionSeriesIndex].points[Closest_Collision_Point_Index-1].y,
                    temp_point.x,temp_point.y,0.0,0.0,0.0,1.0);
                    DrawGLLineFromCurrentMatrix(mx_inworld,my_inworld,temp_point.x,temp_point.y,0.0,1.0,1.0,0.6);
                    DrawGLLineFromCurrentMatrix(mx_inworld,my_inworld,working_level.collision_series[ActiveCollisionSeriesIndex].points[Closest_Collision_Point_Index-1].x,working_level.collision_series[ActiveCollisionSeriesIndex].points[Closest_Collision_Point_Index-1].y,0.0,1.0,1.0,0.6);
                }
                DrawGLRectSolidFromCurrentMatrix(square2,mx_inworld-4,my_inworld-4,8,8);
            }
        }
        if(is_adding_point) DrawGLRectSolidFromCurrentMatrix(square2,mx_inworld-4,my_inworld-4,8,8);
        glLoadIdentity();

    }
}

void DrawWorld()
{
    DrawStaticsAndEntities();
    DrawCollisionVectors();
}

void DrawEditingHelpers()
{
    using namespace main_editor;
            if(grid_snapping_is_on and camera_zoom == 1.0){

            //FIX THIS SHIT
            for(int x = 0; x<SCREEN_WIDTH+256; x+=512)
            {
                for(int y = 0; y<SCREEN_HEIGHT+256; y+=512){
                    int offset_x = ((int)((-camera_x+in_game_camera_x)/32)*32)-(-camera_x+in_game_camera_x);
                    int offset_y = ((int)((-camera_y+in_game_camera_y)/32)*32)-(-camera_y+in_game_camera_y);
                    //glColor4f(1.0,1.0,1.0,0.5);
                    snap_grid_bg.draw((x-offset_x),(y-offset_y),0,1.0);
                    glColor4f(1.0,1.0,1.0,1.0);
                }
            }
            glTranslatef(-camera_x,-camera_y,0);
            glTranslatef(in_game_camera_x,in_game_camera_y,0);
            snap_grid_overlay.draw_from_current_matrix((int)nearbyint((mx_inworld/32)*32)+16,(int)nearbyint((my_inworld/32)*32)+16,0,1.0);
        }

        if(ObjectHoveredOverIndex!=NO_OBJECT_HOVERED_OVER)
        {
            SDL_Rect &temp = working_level_statics_or_entities->at(ObjectHoveredOverIndex)->bounds;
            if(object_editing_type==ENTITY and active_entity_type_index <=6) {
                DrawGLRect(square2,1,(int)(((temp.x-(camera_x*active_layer_parallax_mod))*camera_zoom)+in_game_overlay_offset_x),
                            (int)(((temp.y-(camera_y*active_layer_parallax_mod))*camera_zoom)+in_game_overlay_offset_y),
                            (int)(temp.w*camera_zoom),(int)(temp.h*camera_zoom),0.0,1.0,0.0,1.0);
            }
            else {
                DrawGLRect(square2,10,(int)(((temp.x+10-(camera_x*active_layer_parallax_mod*Hovered_Object->parallax_offset))*camera_zoom)+in_game_overlay_offset_x),
                            (int)(((temp.y+10-(camera_y*active_layer_parallax_mod*Hovered_Object->parallax_offset))*camera_zoom)+in_game_overlay_offset_y),
                            (int)((temp.w-20)*camera_zoom),(int)((temp.h-20)*camera_zoom),0.0,1.0,0.0,0.3);
            }
            if(manipulation_mode==CLIP or manipulation_mode == SCALE)
            {
                glLoadIdentity();
                glScalef(camera_zoom,camera_zoom,0);
                glTranslatef(-camera_x*active_layer_parallax_mod*Hovered_Object->parallax_offset,-camera_y*active_layer_parallax_mod*Hovered_Object->parallax_offset,0);
                glTranslatef(in_game_camera_x,in_game_camera_y,0);
                if(left_side_is_hovered) DrawGLRectSolidFromCurrentMatrix(square2,temp.x,temp.y,10,temp.h,1.0,1.0,1.0,0.4);
                if(top_side_is_hovered) DrawGLRectSolidFromCurrentMatrix(square2,temp.x,temp.y,temp.w,10,1.0,1.0,1.0,0.4);
                if(right_side_is_hovered) DrawGLRectSolidFromCurrentMatrix(square2,temp.x+temp.w-10,temp.y,10,temp.h,1.0,1.0,1.0,0.4);
                if(bottom_side_is_hovered) DrawGLRectSolidFromCurrentMatrix(square2,temp.x,temp.y+temp.h-10,temp.w,10,1.0,1.0,1.0,0.4);
                glLoadIdentity();
            }
        }
        if(ObjectGrabbedIndex!=NO_OBJECT_GRABBED&&manipulation_mode==ROTATE)
        {
            Grabbed_Object = working_level_statics_or_entities->at(ObjectGrabbedIndex);
            DrawGLLine(mx,my,(int)(((Grabbed_Object->x-(camera_x*active_layer_parallax_mod*Grabbed_Object->parallax_offset))*camera_zoom)+in_game_overlay_offset_x),
                       (int)(((Grabbed_Object->y-(camera_y*active_layer_parallax_mod*Grabbed_Object->parallax_offset))*camera_zoom))+in_game_overlay_offset_y);
        }
        if(active_layer!=7){ //layer 3 is screen-sized, and will also cause a division by zero error
            int tempx = (int)((((-(camera_x*active_layer_parallax_mod))*camera_zoom)+in_game_overlay_offset_x)-DEBUG_FONT_SIZE*8);
            if(tempx<0) tempx = 0;
            if(tempx>SCREEN_WIDTH-DEBUG_FONT_SIZE*8) tempx = SCREEN_WIDTH-DEBUG_FONT_SIZE*8;
            int tempy = (int)((((-(camera_y*active_layer_parallax_mod))*camera_zoom)+in_game_overlay_offset_y)-DEBUG_FONT_SIZE*2);
            if(tempy<0) tempy = 0;
            if(tempy>SCREEN_HEIGHT-DEBUG_FONT_SIZE*2) tempy = SCREEN_HEIGHT-DEBUG_FONT_SIZE*2;
            Text_Outlined(debug_font,tempx,(int)((((-(camera_y*active_layer_parallax_mod))*camera_zoom)+in_game_overlay_offset_y)-DEBUG_FONT_SIZE*2),"Level Border",white,black);
            Text_Outlined(debug_font,(int)((((-(camera_x*active_layer_parallax_mod))*camera_zoom)+in_game_overlay_offset_x)-DEBUG_FONT_SIZE*8),tempy,"Level Border",white,black);
            DrawGLRect(square2,3,(int)(((-(camera_x*active_layer_parallax_mod))*camera_zoom)+in_game_overlay_offset_x),(int)(((-(camera_y*active_layer_parallax_mod))*camera_zoom)+in_game_overlay_offset_y),(int)((working_level.width*active_layer_parallax_mod)*camera_zoom),(int)((working_level.height*active_layer_parallax_mod)*camera_zoom),1.0,1.0,1.0,0.3);
        }
}

void DrawTexturePallete()
{
    using namespace main_editor;
        // DRAW TEXTURE PALLETE
        if((object_editing_type!=COLLISION_VECTOR&&entity_types[active_entity_type_index] != "t")||(object_editing_type==STATIC)){
            int current_x = 0 - pallete_scroll;
            Text_Outlined(debug_font,0,SCREEN_HEIGHT-texture_pallete_image_size*1.5-DEBUG_FONT_SIZE-6,"Loaded Textures:",white,black);
            DrawGLLine(0,SCREEN_HEIGHT-texture_pallete_image_size*1.5-3,SCREEN_WIDTH,SCREEN_HEIGHT-texture_pallete_image_size*1.5-3);
            DrawGLRectSolid(square2,0,SCREEN_HEIGHT-texture_pallete_image_size*1.5,SCREEN_WIDTH,texture_pallete_image_size*1.5,1.0,1.0,1.0,0.5);
            for(unsigned int i = 0; i < working_level.textures.size(); i++){
                current_x+=texture_pallete_image_size/2;
                working_level.textures[i].draw_force_size(current_x,SCREEN_HEIGHT-texture_pallete_image_size*0.8,texture_pallete_image_size,texture_pallete_image_size,0);
                current_x+=texture_pallete_image_size/2;
            }
            DrawGLRect(square2,1,active_texture_index*texture_pallete_image_size - pallete_scroll,SCREEN_HEIGHT-texture_pallete_image_size*1.5,texture_pallete_image_size,texture_pallete_image_size*1.5,0.0,1.0,0.0,0.3);
            Text_Outlined(debug_font,(active_texture_index*texture_pallete_image_size)-pallete_scroll,SCREEN_HEIGHT-DEBUG_FONT_SIZE-4,"ACTIVE",white,black);
            if(mouse_is_in_pallete){
                int temp_box_position = (((mx + pallete_scroll) / texture_pallete_image_size)*texture_pallete_image_size)-pallete_scroll;
                DrawGLRectSolid(square2,temp_box_position,SCREEN_HEIGHT-texture_pallete_image_size*1.5,texture_pallete_image_size,texture_pallete_image_size*1.5,1.0,1.0,1.0,0.5);
                if((unsigned((mx + pallete_scroll) / texture_pallete_image_size)>working_level.textures.size()-1)||working_level.textures.size()==0){
                    Text_Outlined(debug_font,temp_box_position,SCREEN_HEIGHT-texture_pallete_image_size*0.8-DEBUG_FONT_SIZE,"LOAD",white,black);
                }
                //Draw the hovered texture in higher detail in the center of the screen.
                if(working_level.textures.size())
                {
                    unsigned int hovered_texture_index = ((mx + pallete_scroll) / texture_pallete_image_size);
                    if(hovered_texture_index<working_level.textures.size()) working_level.textures[hovered_texture_index].draw(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0,1.0);
                }

                if(pallete_scroll>0) Text_Outlined(debug_font,0,SCREEN_HEIGHT-texture_pallete_image_size*0.8-DEBUG_FONT_SIZE,"<<",white,black);
                if((working_level.textures.size()*texture_pallete_image_size)-pallete_scroll>(unsigned)SCREEN_WIDTH) Text_Outlined(debug_font,SCREEN_WIDTH-DEBUG_FONT_SIZE*2,SCREEN_HEIGHT-texture_pallete_image_size*0.8-DEBUG_FONT_SIZE,">>",white,black);
            }
        }

}

void DrawActualScreenSizeOverlay()
{
    using namespace main_editor;
        //DRAW ACTUAL SCREEN SIZE OVERLAY
        if(is_actual_screen_size_shown) {
                if(is_game_preview_mode_on){
                    DrawGLRectSolid(square2,0,0,in_game_overlay_offset_x,SCREEN_HEIGHT,0.0,0.0,0.0,0.6);
                    DrawGLRectSolid(square2,in_game_overlay_offset_x,0,SCREEN_WIDTH,in_game_overlay_offset_y,0.0,0.0,0.0,0.6);
                    DrawGLRectSolid(square2,in_game_overlay_offset_x+(int)(ACTUAL_GAME_SCREEN_WIDTH*camera_zoom),in_game_overlay_offset_y,SCREEN_WIDTH,(int)(ACTUAL_GAME_SCREEN_HEIGHT*camera_zoom),0.0,0.0,0.0,0.6);
                    DrawGLRectSolid(square2,in_game_overlay_offset_x,in_game_overlay_offset_y+(int)(ACTUAL_GAME_SCREEN_HEIGHT*camera_zoom),SCREEN_WIDTH,SCREEN_HEIGHT,0.0,0.0,0.0,0.6);
                    }
                DrawGLRect(square2,8,in_game_overlay_offset_x,in_game_overlay_offset_y,(int)(ACTUAL_GAME_SCREEN_WIDTH*camera_zoom),(int)(ACTUAL_GAME_SCREEN_HEIGHT*camera_zoom),1.0,1.0,1.0,0.2);
        }
}

void DrawHotkeyInstructions()
{
    using namespace main_editor;

     if(ObjectHoveredOverIndex!=NO_OBJECT_HOVERED_OVER)
            {
                Object *temp_obj = working_level_statics_or_entities->at(ObjectHoveredOverIndex);
                stringstream debuginfo;
                debuginfo.str( "" );
                debuginfo<<temp_obj->parallax_offset;
                Text_Outlined(debug_font,mx+20,my,debuginfo.str(),white,black);
            }

            // DRAW CONTROLS OR DEBUG INFO
        if(text_info_state == CONTROLS&&!info_text_is_hidden){
                    stringstream debuginfo;
            if(object_editing_type==STATIC||object_editing_type==ENTITY){
                if(object_editing_type==STATIC) {
                    Text_Outlined(debug_font,0,0,"EDITING STATICS",white,black);
                    Text_Outlined(debug_font,0,DEBUG_FONT_SIZE,"CTRL: Edit Entities | E: Bring up Properties Editor | ?: Show debug info | TAB: turn off text",white,black);
                }
                if(object_editing_type==ENTITY) {
                    debuginfo.str("");
                    debuginfo<<"EDITING ";
                    /*
                    switch(entity_types[active_entity_type_index]){
                        case "c":debuginfo<<"COLLISION BOX";
                        break;
                        case "1":debuginfo<<"COLLISION (WHITE BUTTON)";
                        break;
                        case "2":debuginfo<<"COLLISION (BLACK BUTTON)";
                        break;
                        case "3":debuginfo<<"COLLISION (GREY BUTTON)";
                        break;
                        case "4":debuginfo<<"COLLISION (RED PLAYER ONLY)";
                        break;
                        case "5":debuginfo<<"COLLISION (GREEN PLAYER ONLY)";
                        break;
                        case "6":debuginfo<<"BUTTON (WHITE)";
                        break;
                        case "7":debuginfo<<"BUTTON (BLACK)";
                        break;
                        case "8":debuginfo<<"BUTTON (GREY)";
                        break;
                        case "p": debuginfo<<"PARTICLE SYSTEM";
                        break;
                        case "g": debuginfo<<"GRASS";
                        break;
                        case "s": debuginfo<<"SIGN";
                        break;
                        case "t": debuginfo<<"TREE";
                        break;
                        default: debuginfo<<"UNKNOWN TYPE";
                        break;
                    }*/
                    debuginfo<<entity_types[active_entity_type_index];
                    debuginfo<<" ENTITIES ("<<active_entity_type_index+1<<" of "<<NUMBER_OF_ENTITY_TYPES<<")";
                    Text_Outlined(debug_font,0,0,debuginfo.str(),white,black);
                    Text_Outlined(debug_font,0,DEBUG_FONT_SIZE,"CTRL: Edit Statics | O/P: Edit other entity types | E: Bring up Properties Editor | ?: Show debug info | TAB: turn off text",white,black);
                }

                if(manipulation_mode==MOVE){
                    Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*2,"Current Tool: MOVE - click+drag an object to move it, Hold X/Y to move on those axes only",white,black);
                }
                if(manipulation_mode==ROTATE){
                    Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*2,"Current Tool: ROTATE - click+drag an object to rotate it, Hold SHIFT - snap to 45 degree increments",white,black);
                }
                if(manipulation_mode==SCALE){
                    Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*2,"Current Tool: SCALE - click+drag an object or it's handles to scale it, Hold SHIFT - scale proportionally",white,black);
                }
                if(manipulation_mode==CLIP){
                    Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*2,"Current Tool: CLIP - click+drag an object or it's handles to adjust the clip mask",white,black);
                }
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*3,"PAGE UP/PAGE DOWN/HOME/END Bring Forward/Push Back selected object",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*4,"LMB: Use Tool, +: Toggle Grid Snapping",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*5,"UP/DOWN Select Object behind/in front of selected object",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*6,"RMB: Add Object, 9/0: Adjust Object Parallax",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*7,"Middle Mouse Wheel: Zoom In/Out",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*8,"SPACE: Pan camera/Scroll Texture Pallete",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*9,"M/R/S/C: Switch Tool, ALT+C/V: Copy/Paste Color Settings",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*10,"G: Toggle Game Preview, H: Toggle Always Show Collision",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*11,"DELETE: Delete Object, W: Duplicate Object, F: Flip Object",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*12,"ESC: Close Editor",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*13,"F1: Load | F2: Save As | F5: New | F12: Save",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*14,"Comma and Period: Change Active Layer",white,black);
                debuginfo.str( "" );
                debuginfo<<"Active Layer: "<<active_layer;
                if(active_layer > 2) debuginfo<<" (Background)";
                if(active_layer == 2) debuginfo<<" (Foreground: Behind Players)";
                if(active_layer == 1) debuginfo<<" (Foreground: In Front of Players)";
                if(active_layer == 0) debuginfo<<" (Pre-Foreground)";
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*15,debuginfo.str(),white,black);
                debuginfo.str("");
            }
            else if(object_editing_type==COLLISION_VECTOR){
                Text_Outlined(debug_font,0,0,"EDITING COLLISION VECTORS",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE,"CTRL: Edit Statics | ?: Show debug info | TAB: turn off text",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*2,"RMB: Add Points, hold it for a preview of where the point will be added, BACKSPACE to cancel adding",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*3,"LMB: Move, Hold SHIFT to move entire series",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*4,"DELETE: Delete Point, Hold SHIFT to delete entire series",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*5,"Middle Mouse Wheel: Zoom In/Out",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*6,"ESC: Close Editor",white,black);
                Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*7,"F1: Load | F2: Save As | F5: New | F12: Save",white,black);
            }
        }
}

void DrawDebugText()
{
    using namespace main_editor;
    if(text_info_state == DEBUG_TEXT&&!info_text_is_hidden){
            if(is_actual_screen_size_shown) {
                Text_Outlined(debug_font,(int)(SCREEN_WIDTH/2-((ACTUAL_GAME_SCREEN_WIDTH*camera_zoom)/2)),(int)(SCREEN_HEIGHT/2-((ACTUAL_GAME_SCREEN_HEIGHT*camera_zoom)/2)-8-DEBUG_FONT_SIZE),"Actual Game Screen Size",white,black);
            }
            Text_Outlined(debug_font,0,0,"?: Show Controls | TAB: turn off text",white,black);
            int total_statics_count = 0;
            for(int i = 0;i<5;i++){
                total_statics_count+=working_level.layers[i].statics.size();
            }
            int total_entities_count = 0;
            for(int i = 0;i<5;i++){
                total_entities_count+=working_level.layers[i].entities.size();
            }
            stringstream debuginfo;
            debuginfo.str( "" );
            debuginfo<<"Number of Statics - (in Layer:) "<<working_level.layers[active_layer].statics.size()<<" (in Level:) "<<total_statics_count<<" Number of Entities - (in Layer:) "<<working_level.layers[active_layer].entities.size()<<" (in Level:) "<<total_entities_count;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE,debuginfo.str(),white,black);
            debuginfo.str( "" );
            debuginfo<<"Mouse X(on screen): "<<mx<<" Mouse Y(on screen): "<<my;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
            debuginfo.str( "" );
            debuginfo<<"Mouse X(in world): "<<mx_inworld<<" Mouse Y(in world): "<<my_inworld;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*3,debuginfo.str(),white,black);
            debuginfo.str( "" );
            if(working_level.textures.size()>0){
            debuginfo<<"Filename of active texture: "<<working_level.textures[active_texture_index].filename;
            }
            debuginfo<<"NO TEXTURES LOADED";
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*4,debuginfo.str(),white,black);
            debuginfo.str( "" );
            debuginfo<<"is_object_hovered== "<<is_object_hovered<<" is_object_grabbed== "<<is_object_grabbed;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*5,debuginfo.str(),white,black);
            debuginfo.str( "" );
            debuginfo<<"Camera X: "<<camera_x<<" Camera Y: "<<camera_y<<" Camera Zoom: "<<camera_zoom;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*6,debuginfo.str(),white,black);
            debuginfo.str( "" );
            debuginfo<<"In Game Camera X: "<<in_game_camera_x<<" In Game Camera Y: "<<in_game_camera_y;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*7,debuginfo.str(),white,black);
            debuginfo.str( "" );
            debuginfo<<"mouse_is_in_pallete: "<<mouse_is_in_pallete<<"  Current Desktop Resolution: "<<DesktopVideoInfo->current_w<<"x"<<DesktopVideoInfo->current_h;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*8,debuginfo.str(),white,black);
            debuginfo.str( "" );
            debuginfo<<"under_object_modifier_is_active = "<<under_object_modifier_is_active;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*9,debuginfo.str(),white,black);
            debuginfo.str( "" );
            debuginfo<<"ActiveCollisionSeriesIndex: "<<ActiveCollisionSeriesIndex<<" Closest_Collision_Point_Index: "<<Closest_Collision_Point_Index;
            Text_Outlined(debug_font,0,DEBUG_FONT_SIZE*10,debuginfo.str(),white,black);
            if(ObjectHoveredOverIndex!=NO_OBJECT_HOVERED_OVER&&!is_object_grabbed)
            {
                Object *temp_obj = working_level_statics_or_entities->at(ObjectHoveredOverIndex);
                debuginfo.str( "" );
                debuginfo<<"Object Index: "<<ObjectHoveredOverIndex;
                Text_Outlined(debug_font,mx+20,my,debuginfo.str(),white,black);
                debuginfo.str( "" );
                debuginfo<<"X: "<<temp_obj->x<<"  Y: "<<temp_obj->y<<"  W:  "<<temp_obj->w<<"  H:  "<<temp_obj->h;
                Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE,debuginfo.str(),white,black);
                debuginfo.str( "" );
                debuginfo<<"Rotation(deg): "<<temp_obj->rotation;
                if(temp_obj->type == "$") { //static
                    debuginfo<<"  X Scaling: "<<FindStaticScalePercentage(Width,*(reinterpret_cast<Static*>(temp_obj)),working_level)<<"%  Y Scaling:  "<<FindStaticScalePercentage(Height,*(reinterpret_cast<Static*>(temp_obj)),working_level)<<"%";
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
                    debuginfo.str( "" );
                    debuginfo<<"Texture: "<<working_level.textures[reinterpret_cast<Static*>(temp_obj)->texture_index].filename;
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*3,debuginfo.str(),white,black);
                }
                else if(temp_obj->type == "GrassType") { //GRASS TYPE ENTITY
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
                    debuginfo.str("");
                    debuginfo<<"Texture: "<<working_level.textures[reinterpret_cast<Entity_GrassType*>(temp_obj)->texture_index].filename;
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*3,debuginfo.str(),white,black);
                }
                else if(temp_obj->type == "SignType") { //SIGN TYPE ENTITY
                        Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
                        debuginfo.str("");
                        Entity_SignType* hovered_entity = reinterpret_cast<Entity_SignType*>(temp_obj);
                        debuginfo<<"Fade in Delay (in Milliseconds): "<<(hovered_entity->fade_delay_ms);
                        Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*3,debuginfo.str(),white,black);
                    }
                else{
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
                }
            }
            else if(is_object_grabbed)
            {
                Object *temp_obj = working_level_statics_or_entities->at(ObjectGrabbedIndex);
                debuginfo.str( "" );
                debuginfo<<"Object Index: "<<ObjectGrabbedIndex;
                Text_Outlined(debug_font,mx+20,my,debuginfo.str(),white,black);
                debuginfo.str( "" );
                debuginfo<<"X: "<<temp_obj->x<<"  Y: "<<temp_obj->y<<"  W:  "<<temp_obj->w<<"  H:  "<<temp_obj->h<<"  U:  "<<temp_obj->u<<"  V:  "<<temp_obj->v;
                Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE,debuginfo.str(),white,black);
                debuginfo.str( "" );
                debuginfo<<"Rotation(deg): "<<temp_obj->rotation<<"  U2:  "<<temp_obj->u2<<"  V2:  "<<temp_obj->v2;
                if(temp_obj->type == "$") {
                    debuginfo<<"  X Scaling: "<<FindStaticScalePercentage(Width,*(reinterpret_cast<Static*>(temp_obj)),working_level)<<"%  Y Scaling:  "<<FindStaticScalePercentage(Height,*(reinterpret_cast<Static*>(temp_obj)),working_level)<<"%";
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
                    debuginfo.str( "" );
                    debuginfo<<"Texture: "<<working_level.textures[reinterpret_cast<Static*>(temp_obj)->texture_index].filename;
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*3,debuginfo.str(),white,black);
                }
                else if(temp_obj->type == "GrassType") { //GRASS TYPE ENTITY
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
                    debuginfo.str("");
                    debuginfo<<"Texture: "<<working_level.textures[reinterpret_cast<Entity_GrassType*>(temp_obj)->texture_index].filename;
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*3,debuginfo.str(),white,black);
                }
                else if(temp_obj->type == "SignType") { //SIGN TYPE ENTITY
                        Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
                        debuginfo.str("");
                        Entity_SignType* hovered_entity = reinterpret_cast<Entity_SignType*>(temp_obj);
                        debuginfo<<"Fade in Delay (in Milliseconds): "<<(hovered_entity->fade_delay_ms);
                        Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*3,debuginfo.str(),white,black);
                    }
                else{
                    Text_Outlined(debug_font,mx+20,my+DEBUG_FONT_SIZE*2,debuginfo.str(),white,black);
                }
            }
        }
}

void DrawTypingModeInputScreen()
{
    using namespace main_editor;
        // DRAW TYPING BOX
        if(is_taking_typing_input)
        {
            DrawGLRectSolid(square2,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,1.0,1.0,1.0,0.8);
            DrawGLRectSolid(square2,0,(SCREEN_HEIGHT/2)-DEBUG_FONT_SIZE/2,SCREEN_WIDTH,DEBUG_FONT_SIZE*2,0.0,0.0,0.0,1.0);
            if(input_mode == INPUT_SAVE_LEVEL)
            {
                Text(debug_font,0,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE*4,"Please type the filename to save to, then press ENTER to save: (Press ESC or DELETE to abort saving)",black);
            }
            else Text(debug_font,0,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE*4,"Please type the filename to load, then press ENTER to load it: (Press ESC or DELETE to abort loading)",black);
            if(input_mode == INPUT_LOAD_TEXTURE){
                Text(debug_font,0,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE*3,"The Directory (img/) and the filetype (.tga) are automatically added",black);
            }
            else Text(debug_font,0,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE*3,"The Directory (lvl/) and the filetype (.lvl) are automatically added",black);
            Text(debug_font,0,SCREEN_HEIGHT/2-DEBUG_FONT_SIZE*2,"so, don't type those :)",black);
            if(input_mode == INPUT_LOAD_TEXTURE){
                Text(debug_font,0,SCREEN_HEIGHT/2+DEBUG_FONT_SIZE*3,"Note: You can batch load several files which begin with the same name: i.e. file1, file2, file3, etc.",black);
                Text(debug_font,0,SCREEN_HEIGHT/2+DEBUG_FONT_SIZE*4,"by using the range indicators ':' and '>'. For example, typing 'file:1>9' will load file1 through file9",black);
                Text(debug_font,0,SCREEN_HEIGHT/2+DEBUG_FONT_SIZE*6,"To load an entire folder, simply type the folder name followed by a double backslash. ex: 'grass//'",black);
                Text(debug_font,0,SCREEN_HEIGHT/2+DEBUG_FONT_SIZE*7,"You can simply type '//' to load all files in the main /img directory. This will not load subfolders. ",black);
            }
            string temp_typed_text;
            temp_typed_text = typing_handler.return_string();
            temp_typed_text.append("<--");
            Text(debug_font,0,SCREEN_HEIGHT/2,temp_typed_text,white);
            if(input_mode == INPUT_SAVE_LEVEL) Text(debug_font,0,SCREEN_HEIGHT/2+DEBUG_FONT_SIZE*2,"SAVING LEVEL",black);
            if(input_mode == INPUT_LOAD_LEVEL) Text(debug_font,0,SCREEN_HEIGHT/2+DEBUG_FONT_SIZE*2,"LOADING LEVEL",black);
            if(input_mode == INPUT_LOAD_TEXTURE) Text(debug_font,0,SCREEN_HEIGHT/2+DEBUG_FONT_SIZE*2,"LOADING TEXTURE",black);

        }
}

void DrawEntityEditor()
{
    using namespace main_editor;
    //DRAW ENTITY EDITOR
        if(entity_editor_is_open)
        {
            //DrawGLRectSolid(square2,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,1.0,1.0,1.0,0.8);
            for(unsigned int i = 0; i<entity_editor_UIElements.size(); i++)
            {
                if(entity_editor_UIElements[i]->type == "TextField") //TEXT_INPUT_BOX
                {
                    UI_TextField* active_UIElement = reinterpret_cast<UI_TextField*>(entity_editor_UIElements[i]);
                    DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,0.0,0.0,0.0,0.8);
                    if(active_UIElement->mouse_is_hovered_over) DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,0.0,0.0,0.0,1.0);
                    DrawGLRectSolid(square2,active_UIElement->x+2,active_UIElement->y+2,active_UIElement->w-4,active_UIElement->h-4,1.0,1.0,1.0,0.6);
                    if(active_UIElement->is_taking_typing) DrawGLRectSolid(square2,active_UIElement->x+2,active_UIElement->y+2,active_UIElement->w-4,active_UIElement->h-4,1.0,1.0,1.0,0.5);
                    if(active_UIElement->label!="");Text_Outlined(debug_font,active_UIElement->x-10,active_UIElement->y-int(DEBUG_FONT_SIZE*1.5),active_UIElement->label,white,black);
                    string temp_typed_text;
                    temp_typed_text = active_UIElement->typed_string;
                    if(active_UIElement->is_taking_typing) temp_typed_text.append("<--");
                    if(temp_typed_text!="")Text_Outlined(debug_font,active_UIElement->x+3,active_UIElement->y,temp_typed_text,white,black);

                }
                else if(entity_editor_UIElements[i]->type == "Button") //BUTTON
                {
                    UI_Button* active_UIElement = reinterpret_cast<UI_Button*>(entity_editor_UIElements[i]);
                    DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,0.0,0.0,0.0,0.8);
                    if(active_UIElement->mouse_is_hovered_over) DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,1.0,1.0,1.0,0.4);
                    DrawGLRectSolid(square2,active_UIElement->x+2,active_UIElement->y+2,active_UIElement->w-4,active_UIElement->h-4,1.0,1.0,1.0,0.6);
                    if(active_UIElement->label!="") Text_Outlined(debug_font,active_UIElement->x+4,active_UIElement->y-1,active_UIElement->label,white,black);

                }
                else if(entity_editor_UIElements[i]->type == "Sliderf")
                {
                    UI_Sliderf* active_UIElement = reinterpret_cast<UI_Sliderf*>(entity_editor_UIElements[i]);
                    DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,0.0,0.0,0.0,0.8);
                    DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,(active_UIElement->current_percent*active_UIElement->w),active_UIElement->h,1.0,1.0,1.0,0.5);
                    if(active_UIElement->mouse_is_hovered_over) DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,1.0,1.0,1.0,0.2);
                    //DrawGLRectSolid(square2,active_UIElement->x+2,active_UIElement->y+2,active_UIElement->w-4,active_UIElement->h-4,1.0,1.0,1.0,0.6);
                    DrawGLRectSolid(square2,active_UIElement->x+(active_UIElement->current_percent*active_UIElement->w)-2,active_UIElement->y,4,active_UIElement->h,0.9,0.9,0.9,1.0);
                    if(active_UIElement->label!="") {
                        stringstream label;
                        label<<active_UIElement->label<<" = ("<<*(active_UIElement->var_to_manipulate)<<")";
                        Text(debug_font,active_UIElement->x-10,active_UIElement->y-int(DEBUG_FONT_SIZE*1.5),label.str(),grey);
                    }
                    else{
                        stringstream value;
                        value<<*(active_UIElement->var_to_manipulate);
                        Text(debug_font,active_UIElement->x-9,active_UIElement->y-int(DEBUG_FONT_SIZE*1.5),value.str(),grey);
                    }

                }
                else if(entity_editor_UIElements[i]->type == "Slideri")
                {
                    UI_Slideri* active_UIElement = reinterpret_cast<UI_Slideri*>(entity_editor_UIElements[i]);
                    DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,0.0,0.0,0.0,0.8);
                    DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,(active_UIElement->current_percent*active_UIElement->w),active_UIElement->h,1.0,1.0,1.0,0.5);
                    if(active_UIElement->mouse_is_hovered_over) DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,1.0,1.0,1.0,0.2);
                    //DrawGLRectSolid(square2,active_UIElement->x+2,active_UIElement->y+2,active_UIElement->w-4,active_UIElement->h-4,1.0,1.0,1.0,0.6);
                    DrawGLRectSolid(square2,active_UIElement->x+(active_UIElement->current_percent*active_UIElement->w)-2,active_UIElement->y,4,active_UIElement->h,0.9,0.9,0.9,1.0);
                    if(active_UIElement->label!="") {
                        stringstream label;
                        label<<active_UIElement->label<<" = ("<<*(active_UIElement->var_to_manipulate)<<")";
                        Text(debug_font,active_UIElement->x-10,active_UIElement->y-int(DEBUG_FONT_SIZE*1.5),label.str(),grey);
                    }
                    else{
                        stringstream value;
                        value<<*(active_UIElement->var_to_manipulate);
                        Text(debug_font,active_UIElement->x-9,active_UIElement->y-int(DEBUG_FONT_SIZE*1.5),value.str(),grey);
                    }

                }
                else if(entity_editor_UIElements[i]->type == "TreeEditor") //TREE EDITOR
                {
                    UI_TreeEditor* active_UIElement = reinterpret_cast<UI_TreeEditor*>(entity_editor_UIElements[i]);
                    DrawGLRectSolid(square2,active_UIElement->x,active_UIElement->y,active_UIElement->w,active_UIElement->h,0.0,0.0,0.0,0.7);
                    if(active_UIElement->is_adding_node == true){
                        DrawGLRectSolid(square2,(active_UIElement->new_node_x)-4,(active_UIElement->new_node_y)-4,8,8,0.0,1.0,0.0,1.0);
                    }
                    glLoadIdentity();
                    glTranslatef((float)active_UIElement->tree_view_origin_x,(float)active_UIElement->tree_view_origin_y,0.0);
                    active_UIElement->Editing_Tree->draw(square2,1.0,1.0,0.0,1.0);
                    if(active_UIElement->Tree_Debug_Info.is_real==true){
                        Tree_Branch* Closest_Branch = active_UIElement->Tree_Debug_Info.Closest_Branch;
                        Closest_Branch->draw(square2,1.0,1.0,1.0,1.0);
                        if(active_UIElement->is_adding_node){
                        point_2d second_point_in_closest_vector((int)Closest_Branch->nodes[active_UIElement->Tree_Debug_Info.index_of_second_node_in_closest_vector].x,(int)Closest_Branch->nodes[active_UIElement->Tree_Debug_Info.index_of_second_node_in_closest_vector].y);
                        point_2d first_point_in_closest_vector((int)Closest_Branch->nodes[active_UIElement->Tree_Debug_Info.index_of_second_node_in_closest_vector-1].x,(int)Closest_Branch->nodes[active_UIElement->Tree_Debug_Info.index_of_second_node_in_closest_vector-1].y);
                        if(unsigned (active_UIElement->Tree_Debug_Info.index_of_closest_node)==active_UIElement->Tree_Debug_Info.Closest_Branch->nodes.size()-1){
                            DrawGLLineFromCurrentMatrix(mx-active_UIElement->tree_view_origin_x,my-active_UIElement->tree_view_origin_y,second_point_in_closest_vector.x,second_point_in_closest_vector.y,0.0,1.0,0.0,1.0);
                        }
                        else{
                            DrawGLLineFromCurrentMatrix(mx-active_UIElement->tree_view_origin_x,my-active_UIElement->tree_view_origin_y,first_point_in_closest_vector.x,first_point_in_closest_vector.y,0.0,1.0,0.0,1.0);
                            DrawGLLineFromCurrentMatrix(mx-active_UIElement->tree_view_origin_x,my-active_UIElement->tree_view_origin_y,second_point_in_closest_vector.x,second_point_in_closest_vector.y,0.0,1.0,0.0,1.0);
                            DrawGLLineFromCurrentMatrix(first_point_in_closest_vector.x,first_point_in_closest_vector.y,second_point_in_closest_vector.x,second_point_in_closest_vector.y,0.0,0.0,0.0,1.0);

                        }
                        }
                    }
                    Text_Outlined(debug_font,active_UIElement->x+4,active_UIElement->y+4,"LMB: Move Node | RMB: Add Node | P: Split End of Branch | W: Subdivide Closest Vector | DELETE: Delete Node", white, black);
                    stringstream temp_info;
                    temp_info.str("");
                    temp_info<<"is_adding_node =="<<active_UIElement->is_adding_node<<" new node x: "<<active_UIElement->new_node_x<<" new node y: "<<active_UIElement->new_node_y;
                    Text_Outlined(debug_font,active_UIElement->x+4,active_UIElement->y+4+DEBUG_FONT_SIZE,temp_info.str(), white, black);

                    glLoadIdentity();
                }
            }
        }
}

void DrawEditorOverlays()
{
    using namespace main_editor;
    DrawEditingHelpers();
    DrawTexturePallete();
    DrawActualScreenSizeOverlay();
    DrawHotkeyInstructions();
    DrawDebugText();
    DrawTypingModeInputScreen();
    DrawEntityEditor();



    stringstream build_info_stream;
    build_info_stream.str("");
    build_info_stream<<"Version "<<AutoVersion::FULLVERSION_STRING;
    build_info_stream<<"  Built "<<AutoVersion::DATE<<"/"<<AutoVersion::MONTH<<"/"<<AutoVersion::YEAR<<"  #"<<AutoVersion::BUILDS_COUNT;
    string build_info = build_info_stream.str();
    //size_t ver_length = build_info.size();
    int build_info_text_width, throwaway;
    SizeText(debug_font, build_info, &build_info_text_width, &throwaway);

    Text(debug_font,SCREEN_WIDTH-build_info_text_width-10,SCREEN_HEIGHT-DEBUG_FONT_SIZE-6,build_info,white);

    //Draw the FPS display
    if(FPS_IS_SHOWN){
        stringstream fps_text;
        fps_text.str("");
        fps_text<<"FPS: "<<current_frames_per_second;
        Text_Outlined(debug_font,SCREEN_WIDTH-(DEBUG_FONT_SIZE)*5,4,fps_text.str(),white,black);
    }


    //DRAW AND UPDATE THE STATUS LIST
    editor_status_list.update();
    editor_status_list.draw(debug_font,SCREEN_WIDTH/2,0,white,black);

    //DRAW THE MOUSE CURSOR
    if(!is_taking_typing_input&&object_editing_type!=COLLISION_VECTOR&&!entity_editor_is_open){
    if(manipulation_mode == MOVE) editor_cursor_move.draw(mx+1,my+6,0,1.0);
    if(manipulation_mode == ROTATE) editor_cursor_rotate.draw(mx+1,my+5,0,1.0);
    if(manipulation_mode == SCALE)editor_cursor_scale.draw(mx,my+6,0,1.0);
    if(manipulation_mode == CLIP)editor_cursor_clip.draw(mx+3,my+6,0,1.0);
    }
    else editor_cursor.draw(mx+5,my+9,0,1.0);

}

void Draw()
{
    using namespace main_editor;

        /////////////////////////////////////////////////////////////////////////
        //          BEGIN DRAW BLOCK
        /////////////////////////////////////////////////////////////////////////

        glClear(GL_COLOR_BUFFER_BIT);

        DrawGLRectSolid(square2,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,working_level.background_r,working_level.background_g,working_level.background_b,1.0);

        //draw the transparency substitute (a grid which helps me see both black and white statics)
        if(!is_game_preview_mode_on)
        {
            //load the identity matrix, which sets 0,0 to the top left corner of the screen
            glLoadIdentity();
            //make sure textures are enabled
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, editor_bg.GLtexture);
            //create some variables which represent the size relationship between the
            //screen and the texture in a floating point variable
            float temp_w = (float)SCREEN_WIDTH/(float)editor_bg.w;
            float temp_h = (float)SCREEN_HEIGHT/(float)editor_bg.h;
            //then we draw the screen-sized texture, mapping the texture coords to the previously
            //established relationship
            glBegin(GL_TRIANGLE_STRIP);
                glTexCoord2f(0,0);  glVertex2f(0, 0);
                glTexCoord2f(temp_w,0);  glVertex2f(SCREEN_WIDTH, 0);
                glTexCoord2f(0,temp_h);  glVertex2f(0, SCREEN_HEIGHT);
                glTexCoord2f(temp_w,temp_h);  glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);

            glEnd();
        }
        DrawWorld();
        DrawEditorOverlays();


        SDL_GL_SwapBuffers();
        //glFinish();

}

void LimitFPS()
{
    using namespace main_editor;
    //Handle FPS Limiting
    if( ( FPS_IS_CAPPED == true ) && ( frame_time.GetTicks() < 1000 / FRAMES_PER_SECOND_CAP ) ) { //Sleep the remaining frame time
        SDL_Delay( ( 1000 / FRAMES_PER_SECOND_CAP ) - frame_time.GetTicks() );
    }
}

void SetPolymorphicStaticsOrEntitiesVector()
{
    using namespace main_editor;
    //we make a pointer to access the level's array of the currently selected object type (ENTITY or STATIC) in a polymorphic way
    //so that I don't have to duplicate a bunch of code
    if(object_editing_type == STATIC) working_level_statics_or_entities = &working_level.layers[active_layer].statics;
    else if(object_editing_type == ENTITY) working_level_statics_or_entities = &working_level.layers[active_layer].entities;

}

void UpdateMainEditor()
{
    using namespace main_editor;
    GetFPSSampleForThisFrame();
    SetPolymorphicStaticsOrEntitiesVector();
    HandleInput();
    Draw();
    LimitFPS();
    stringstream fps_text;
    fps_text.str("");
    fps_text<<"FPS: "<<current_frames_per_second;
    string fps_string = fps_text.str();

}

#endif // MAIN_EDITOR_FUNCTIONS_HPP_INCLUDED


