#ifndef UI_SLIDERF_HPP_INCLUDED
#define UI_SLIDERF_HPP_INCLUDED
#include "BaseUIElement.hpp"
#include <string>
//#include <SDL/SDL_ttf.h>
#include "GL_DrawingPrimitives.hpp"

using namespace std;

class UI_Sliderf : public BaseUIElement
{
    public:
    string label;
    int mx, my;
    float low, high;
    float current_percent;
    bool slider_is_grabbed;
    //bool *bool_to_toggle;
    float *var_to_manipulate;
    UI_Sliderf(string label, struct Font* font, int x, int y, int w, int h, float *var_to_manipulate, float low, float high);
    virtual ~UI_Sliderf(){}
    void update(SDL_Event event);
};


#endif
