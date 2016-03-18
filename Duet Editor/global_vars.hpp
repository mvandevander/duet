#ifndef GLOBAL_VARS_HPP_INCLUDED
#define GLOBAL_VARS_HPP_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>

#include "GL_DrawingPrimitives.hpp"

#include <SDL/SDL.h>

#include <SDL/SDL_image.h>
#include "SDL/SDL_mixer.h"
//---------------------------------global_vars.hpp------------------------------------------------------
//
//---------------------------------DECLARATION OF GLOBAL VARIABLES-------------------------------------------
//
//This is just where we declare all of the variables that will be global in scope, if you are looking for
//variables in namespaces, they will be in files named by the following standard NAMESPACE__(name of namespace)__variables.hpp
//for example, all main_editor namespace declarations will be in NAMESPACE__main_editor__variables.hpp
//
//-----------------------------------------------------------------------------------------------------------

//These coords are for the actual window size
//The game should run at 1024x480, but the editor can be as big as possible
int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 750;
const int ACTUAL_GAME_SCREEN_WIDTH = 1280;
const int ACTUAL_GAME_SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND_CAP = 60;

const int DEBUG_FONT_SIZE = 16;

//These coords are for the coordinates of the OpenGL space, so I don't have to resize all my coordinates every time the resolution would change.
//Of course, this is assuming I even implement resolution changes
int LOGICAL_SCREEN_WIDTH = SCREEN_WIDTH;
int LOGICAL_SCREEN_HEIGHT = SCREEN_HEIGHT;

const float SCREEN_RATIO = 0.4687; //LOGICAL_SCREEN_HEIGHT divided by LOGICAL_SCREEN_WIDTH, used for resizing the window

const float ZOOM_OUT_LIMIT = .25;
const float ZOOM_IN_LIMIT = 1.00;

//some constants to change how the game is run
bool FULLSCREEN_IS_ENABLED = true;
const bool WINDOW_IS_RESIZABLE = false;
bool FPS_IS_CAPPED = true;
const bool FPS_IS_SHOWN = true;
bool VSYNC_IS_ENABLED = true;

//const int LAYERS_IN_LEVEL_FORMAT = 5;

SDL_Surface *screen = NULL;
SDL_Color white = {255,255,255};
SDL_Color grey = {180,180,180};
SDL_Color black = {0,0,0};
SDL_Event event;
//TTF_Font *debug_font = NULL;

struct Font* debug_font;

bool quit = false;

GLuint square; //<- this is just the standard display list for a quad. It is used everywhere
GLuint square2;

GLvoid BuildLists() //This function just builds the "square" display list
{
    square=glGenLists(2);
    glNewList(square,GL_COMPILE);
        glBegin(GL_QUADS);
            //glColor4f(1.0,0.0,0.0,1.0);
            glTexCoord2i(0,0);  glVertex2f(-.5, -.5);
            //glColor4f(0.0,1.0,0.0,1.0);
            glTexCoord2i(1,0);  glVertex2f(.5, -.5);
            //glColor4f(0.0,0.0,1.0,1.0);
            glTexCoord2i(1,1);  glVertex2f(.5, .5);
            //glColor4f(1.0,1.0,1.0,1.0);;
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

#endif // GLOBAL_VARS_HPP_INCLUDED
