#include "Texture.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
//#include <SDL/SDL_opengl.h>


bool Texture::load(std::string filename, bool wrap)
{
    this->filename = filename;
    GLint numberOfChannels;
    GLuint img;
    GLenum textureFormat = GL_BGRA;
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load(filename.c_str());
    if(loadedImage!=NULL){
        //if((loadedImage->w & (loadedImage->w - 1)) != 0) printf("warning: Loaded image width is not a power of two %s \n",filename.c_str());
        //if((loadedImage->h & (loadedImage->h - 1)) != 0) printf("warning: Loaded image height is not a power of two %s \n",filename.c_str());
        numberOfChannels = loadedImage->format->BytesPerPixel;
        if(numberOfChannels == 4 ){
            optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
            if(optimizedImage->format->Rmask == 0x000000ff) textureFormat = GL_RGBA;
            else textureFormat = GL_BGRA;
        }
        else if(numberOfChannels == 3){
            optimizedImage = SDL_DisplayFormat(loadedImage);
            if(optimizedImage->format->Rmask == 0x000000ff) textureFormat = GL_RGBA;
            else textureFormat = GL_BGRA;
        }
        glGenTextures(1, &img);
        glBindTexture(GL_TEXTURE_2D,img);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        if(wrap)
        {
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER );
        }
        else
        {
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        }
        glTexImage2D(GL_TEXTURE_2D,0,numberOfChannels,optimizedImage->w,optimizedImage->h,0,textureFormat,GL_UNSIGNED_BYTE,optimizedImage->pixels);
        w = optimizedImage->w;
        h = optimizedImage->h;
        SDL_FreeSurface(loadedImage);
        SDL_FreeSurface(optimizedImage);
        GLtexture = img;
        return true;
    }
    else{
        fprintf(stderr, "Image loading failed: %s\n", SDL_GetError());
    }
    if(loadedImage) SDL_FreeSurface(loadedImage);
    if(optimizedImage) SDL_FreeSurface(optimizedImage);
    w = 0;
    h = 0;
    GLtexture = 0;
    return false;
}

void Texture::draw_force_size(int x, int y, int w, int h, int rot)
{
    glLoadIdentity();
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GLtexture);
    glTranslatef(x,y,0);
    glRotatef( rot, 0.0, 0.0, 1.0 );
    glScalef(w,h,0);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2i(0,0);  glVertex2f(-.5, -.5);
            glTexCoord2i(1,0);  glVertex2f(.5, -.5);
            glTexCoord2i(0,1);  glVertex2f(-.5, .5);
            glTexCoord2i(1,1);  glVertex2f(.5, .5);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Texture::draw(int x, int y, int rot, float scale, bool flip_x)
{

    glLoadIdentity();
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GLtexture);
    glTranslatef(x,y,0);
    glRotatef( rot, 0.0, 0.0, 1.0 );
    if(flip_x) glScalef(w*-scale,h*scale,0);
    else glScalef(w*scale,h*scale,0);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2i(0,0);  glVertex2f(-.5, -.5);
            glTexCoord2i(1,0);  glVertex2f(.5, -.5);
            glTexCoord2i(0,1);  glVertex2f(-.5, .5);
            glTexCoord2i(1,1);  glVertex2f(.5, .5);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Texture::draw(float x, float y, int rot, float scale, bool flip_x)
{

    glLoadIdentity();
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GLtexture);
    glTranslatef(x,y,0);
    glRotatef( rot, 0.0, 0.0, 1.0 );
    if(flip_x) glScalef(w*-scale,h*scale,0);
    else glScalef(w*scale,h*scale,0);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2i(0,0);  glVertex2f(-.5, -.5);
            glTexCoord2i(1,0);  glVertex2f(.5, -.5);
            glTexCoord2i(0,1);  glVertex2f(-.5, .5);
            glTexCoord2i(1,1);  glVertex2f(.5, .5);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //glDeleteLists( square, 1 );
}

void Texture::draw_from_current_matrix(int x, int y, int rot, float scale, bool flip_x)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GLtexture);
    glTranslatef(x,y,0);
    glRotatef( rot, 0.0, 0.0, 1.0 );
    if(flip_x) glScalef(w*-scale,h*scale,0);
    else glScalef(w*scale,h*scale,0);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2i(0,0);  glVertex2f(-.5, -.5);
            glTexCoord2i(1,0);  glVertex2f(.5, -.5);
            glTexCoord2i(0,1);  glVertex2f(-.5, .5);
            glTexCoord2i(1,1);  glVertex2f(.5, .5);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void Texture::draw_from_current_matrix(float x, float y, int rot, float scale, bool flip_x)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GLtexture);
    glTranslatef(x,y,0);
    glRotatef( rot, 0.0, 0.0, 1.0 );
    if(flip_x) glScalef(w*-scale,h*scale,0);
    else glScalef(w*scale,h*scale,0);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2i(0,0);  glVertex2f(-.5, -.5);
            glTexCoord2i(1,0);  glVertex2f(.5, -.5);
            glTexCoord2i(0,1);  glVertex2f(-.5, .5);
            glTexCoord2i(1,1);  glVertex2f(.5, .5);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


Texture LoadTexture(std::string filename, bool wrap)
{
    Texture temporary_texture;
    temporary_texture.load(filename, wrap);
    return temporary_texture;
}

