#ifndef UI_BUTTON_HPP_INCLUDED
#define UI_BUTTON_HPP_INCLUDED
#include "BaseUIElement.hpp"
#include <string>
//#include <SDL/SDL_ttf.h>
#include "GL_DrawingPrimitives.hpp"

using namespace std;

class UI_Button : public BaseUIElement
{
    public:
    string label;
    bool *bool_to_toggle;
    UI_Button(string label, struct Font* font, int x, int y, bool *bool_to_toggle);
    virtual ~UI_Button(){}
    void update(SDL_Event event);
};


#endif // UI_BUTTON_HPP_INCLUDED
