#ifndef FONT_HPP_INCLUDED
#define FONT_HPP_INCLUDED

//Much of the following code was copied from Ryan Pridgeon's Lightweight TTF Text Rendering library, thanks buddy.

/*All code copyright to Ryan Pridgeon, 2009
If you wish to use this code, you must include these credits
in your code, and give credit in your application/documentation
Thankyou
*/

#include <SDL/SDL.h>
#include <string>
#include <GL/glew.h>

struct Font{
    GLuint face_texture;

    GLubyte* face_texture_data;

    int face_tex_w, face_tex_h;
    GLuint displaylist;
    float h;

    struct glyph_info{
        unsigned int width, rows, w, h;
        int left, top;
        float u1, v1, u2, v2, xadvance;
    };

    glyph_info glyph_data[128];

    //int size;
};

struct Font* LoadFont(std::string, int);

void SerializeFont(std::string filename, Font* font);
struct Font* UnserializeFont(std::string filename);

int PrintFont(struct Font*, std::string);

void SizeText(struct Font*, std::string, int*, int*);

int DeleteFont(struct Font*);

void Text(struct Font*, int x, int y, std::string whattosay, SDL_Color &dacolor, float alpha = 1.0);
void Text_Outlined(struct Font*, int x, int y, std::string whattosay, SDL_Color &text_color, SDL_Color &outline_color, float alpha = 1.0);
void Text_Outlined_Current_Matrix(struct Font*, int x, int y, std::string whattosay, SDL_Color &text_color, SDL_Color &outline_color, float alpha = 1.0);

#endif // FONT_HPP_INCLUDED
