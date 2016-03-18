#include "GL_ShaderFuncs.hpp"
#include <stdlib.h>
#include <stdio.h>
//#include "GLee.h"
#include <GL/glew.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_opengl.h>
#include <io.h>
#include <string>
#include <fstream>


using namespace std;

bool AreShadersSupported()
{

    char* extensions = (char*)glGetString(GL_EXTENSIONS);
    if (strstr(extensions,"GL_ARB_vertex_shader") and strstr(extensions,"GL_ARB_fragment_shader") and
        strstr(extensions,"GL_ARB_shader_objects") and strstr(extensions,"GL_ARB_shading_language")){
            return true;
        }
    else return false;
}

unsigned long GetProgramSize(ifstream& file)
{

return 0;
}

GLuint LoadAndBuildShader(GLenum type, const char* filename)
{
    int shader;
    //first, we find the length of the shader source
    size_t size = -1;
    shader = _open(filename,_A_RDONLY);
    if(shader == ENOENT) size = -1;
    size = _lseek(shader, 0, SEEK_END);
    _close(shader);

    FILE *source = fopen(filename,"rb");

    if(source == NULL)
    {
        //something bad has happened, the file doesn't seem to be available
        fprintf(stderr,"Error loading Shader file: %s; it doesn't seem to exist? \n",filename);
    }

    //now we store the shader source in a GLchar array
    GLchar *ProgramSource = (GLchar *)malloc(size*sizeof(GLchar));
    fread(ProgramSource,size,1,source);
    fclose(source);

    GLint length_of_file = size;

    //we build a shader object and send it to the GPU to compile
    GLuint ShaderObject = glCreateShader(type);
    glShaderSource(ShaderObject, 1,(const GLchar **)&ProgramSource,&length_of_file);
    free(ProgramSource);
    glCompileShader(ShaderObject);

    //we test to see if it compiled properly
    GLint compiled;

    glGetObjectParameterivARB(ShaderObject, GL_COMPILE_STATUS, &compiled);
    if(compiled) {//yay
    }
    else{
        GLint blen = 0;
        GLsizei slen = 0;
        glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH, &blen);
        if(blen>0){
            GLchar* compiler_log = (GLchar*)malloc(blen);
            glGetInfoLogARB(ShaderObject, blen, &slen, compiler_log);
            fprintf(stderr, "Shader Error: %s \n", compiler_log);
            free(compiler_log);

        }
    }

    //then we finally return the index of the newly compiled shader :)
    return ShaderObject;
}

void UnloadShader(GLubyte** ShaderSource)
{
    if(*ShaderSource != 0)
    delete[] *ShaderSource;
    *ShaderSource = 0;
}
