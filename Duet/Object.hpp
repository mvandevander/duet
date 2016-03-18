#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED
#include <SDL/SDL.h>
#include <fstream>
#include <string>

using namespace std;

class Object
{
    public:
    int x, y, w, h;
    int texture_index;
    float rotation, parallax_offset, r_mult, g_mult, b_mult, a_mult, r_add, g_add, b_add, a_add,
        r_blend, g_blend, b_blend, a_blend, hue, saturation, lightness, brightness, contrast;
    float u, v, u2, v2;
    bool is_using_additive_tinting, is_using_colorize_hue, is_flipped_horizontally;
    string type;
    SDL_Rect bounds;
    void move(int new_x, int new_y);
    void rotate(float new_rotation);
    void scale(int new_w, int new_h);
    //virtual void update()
    virtual bool serialize(fstream& file)=0;
    virtual bool unserialize(ifstream& file)=0;
    virtual void update()=0;
    virtual ~Object() {}
};


#endif // OBJECT_H_INCLUDED
