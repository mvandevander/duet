#ifndef GL_SHADERFUNCS_HPP_INCLUDED
#define GL_SHADERFUNCS_HPP_INCLUDED
#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
//#include "GLee.h"
#include <GL/glew.h>
//#include <SDL/SDL_opengl.h>
#include <string>
#include <io.h>

using namespace std;
bool AreShadersSupported();
//GLhandleARB MakeShader(string filename);
unsigned long GetProgramSize(ifstream& file);
//int LoadShader(string filename, GLchar* *ShaderSource, GLint* len);
GLuint LoadAndBuildShader(GLenum type, const char *filename);
void UnloadShader(GLubyte** ShaderSource);

//GLhandleARB vertex_shader = glCreateShaderObjectARB





#endif // GL_SHADERFUNCS_HPP_INCLUDED
