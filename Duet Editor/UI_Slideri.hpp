#ifndef UI_SLIDERI_HPP_INCLUDED
#define UI_SLIDERI_HPP_INCLUDED
#include "BaseUIElement.hpp"
#include <string>
//#include <SDL/SDL_ttf.h>
#include "GL_DrawingPrimitives.hpp"

using namespace std;

class UI_Slideri : public BaseUIElement
{
    public:
    string label;
    int mx, my;
    int low, high;
    float current_percent;
    bool slider_is_grabbed;
    //bool *bool_to_toggle;
    int *var_to_manipulate;
    UI_Slideri(string label, struct Font* font, int x, int y, int w, int h, int *var_to_manipulate, int low, int high);
    virtual ~UI_Slideri(){}
    void update(SDL_Event event);
};



#endif // UI_BUTTON_HPP_INCLUDED
