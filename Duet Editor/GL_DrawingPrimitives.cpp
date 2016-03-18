#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>

#include <SDL/SDL.h>
#include <math.h>
//#include <SDL/SDL_ttf.h>
//#include <SDL/SDL_opengl.h>

#include "GL_DrawingPrimitives.hpp"
#include "trig_functions_degrees.hpp"


void DrawGLLine(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
        glColor4f(r,g,b,a);
        glVertex2i(x1,y1);
        glVertex2i(x2,y2);
    glEnd();
    glColor4f(1.0,1.0,1.0,1.0);
}

void DrawGLCircle(int x, int y, int radius, float r, float g, float b, float a)
{
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glColor4f(r,g,b,a);
    glTranslatef(x,y,0);
    for(int a = 0; a<360;a+=1)
        {
        glRotatef(1.0,0.0,0.0,1.0);
        glBegin(GL_LINES);
            glVertex2i(0,radius);
            glVertex2i(5,radius);
        glEnd();
        }
    glColor4f(1.0,1.0,1.0,1.0);
}



void DrawGLRect(GLuint displaylist,int line_width,int x,int y,int w,int h,float r, float g, float b, float a)
{

    glDisable(GL_TEXTURE_2D);
    glLoadIdentity();
    glPushMatrix();
    glColor4f(r,g,b,a);
    glTranslatef(x-line_width,y-line_width,0);
    //glPushMatrix();
    glScalef(line_width,h+line_width,0);
        glCallList(displaylist);
    glPopMatrix();
    //glLoadIdentity();
    glPushMatrix();
    glTranslatef(x,y-line_width,0);
    glScalef(w,line_width,0);
        glCallList(displaylist);
    glPopMatrix();
    //glLoadIdentity();
    glPushMatrix();
    glTranslatef(x+w,y-line_width,0);
    glScalef(line_width,h+(line_width*2),0);
        glCallList(displaylist);
    glPopMatrix();
    //glLoadIdentity();
    glPushMatrix();
    glTranslatef(x-line_width,y+h,0);
    glScalef(w+line_width,line_width,0);
        glCallList(displaylist);
    glPopMatrix();
    glColor4f(1.0,1.0,1.0,1.0);


}

void DrawGLRectSolid(GLuint displaylist,int x,int y,int w,int h,float r, float g, float b, float a)
{

    glDisable(GL_TEXTURE_2D);
    glColor4f(r,g,b,a);
    glLoadIdentity();
    glTranslatef(x,y,0);
    //glPushMatrix();
    glScalef(w,h,0);
        glCallList(displaylist);
    //glPopMatrix();
    glColor4f(1.0,1.0,1.0,1.0);

}

void DrawGLRectSolidFromCurrentMatrix(GLuint displaylist,int x,int y,int w,int h,float r, float g, float b, float a)
{

    glDisable(GL_TEXTURE_2D);
    glColor4f(r,g,b,a);
    glPushMatrix();
    glTranslatef(x,y,0);;
    glScalef(w,h,0);
    glCallList(displaylist);
    glPopMatrix();
    glColor4f(1.0,1.0,1.0,1.0);
}

void DrawGLLineFromCurrentMatrix(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
        glColor4f(r,g,b,a);
        glVertex2i(x1,y1);
        glVertex2i(x2,y2);
    glEnd();
    glColor4f(1.0,1.0,1.0,1.0);
}

void DrawGLCircleFromCurrentMatrix(int x, int y, int radius, float r, float g, float b, float a)
{
    glDisable(GL_TEXTURE_2D);
    glColor4f(r,g,b,a);
    glPushMatrix();
    glTranslatef(x,y,0);
    for(int a = 0; a<360;a+=1)
        {
        glRotatef(1.0,0.0,0.0,1.0);
        glBegin(GL_LINES);
            glVertex2i(0,radius);
            glVertex2i(5,radius);
        glEnd();
        }
        glPopMatrix();
    glColor4f(1.0,1.0,1.0,1.0);
}
