#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED
#include <string>
#include <GL/glew.h>

//#include <SDL/SDL_opengl.h>
using namespace std;

class Texture{
    public:
    bool load(std::string filename);
    void draw_force_size(int x, int y, int w, int h, int rot);
    void draw(int x, int y, int rot, float scale,bool flip_x = false);
    void draw(float x, float y, int rot, float scale, bool flip_x = false);
    void draw_from_current_matrix(float x, float y, int rot, float scale, bool flip_x = false);
    void draw_from_current_matrix(int x, int y, int rot, float scale, bool flip_x = false);
	GLuint GLtexture;
	std::string filename;
	int w;
	int h;
};

Texture LoadTexture(std::string filename); //A non-class function for creating a Texture object and loading it at the same time;



#endif // TEXTURE_HPP_INCLUDED
