#ifndef GL_DRAWINGPRIMITIVES_HPP_INCLUDED
#define GL_DRAWINGPRIMITIVES_HPP_INCLUDED
#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>
#include <string>

#include <GL/glew.h>

void DrawGLLine(int x1, int y1, int x2, int y2, float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
void DrawGLRect(GLuint displaylist,int line_width, int x, int y, int w, int h, float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
void DrawGLCircle(int x, int y, int radius, float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
void DrawGLRectSolid(GLuint displaylist,int x, int y, int w, int h, float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
void DrawGLLineFromCurrentMatrix(int x1, int y1, int x2, int y2, float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
void DrawGLCircleFromCurrentMatrix(int x, int y, int radius, float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
void DrawGLRectSolidFromCurrentMatrix(GLuint displaylist,int x,int y,int w,int h,float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);


#endif // GL_DRAWINGPRIMITIVES_HPP_INCLUDED
