#include "UI_Button.hpp"
#include <string>
#include "Font.hpp"

UI_Button::UI_Button(string label, struct Font* font, int x, int y, bool *bool_to_toggle)
{
    this->x = x;
    this->y = y;

    SizeText(font,label.c_str(),&w,&h);

    w+=8;
    h+=10;
    this->label = label;
    this->bool_to_toggle = bool_to_toggle;
    this->type = "Button";
}

void UI_Button::update(SDL_Event event)
{
    if(event.type == SDL_MOUSEMOTION)
    {
        int mx = event.motion.x;
        int my = event.motion.y;
        if(is_mouse_over(mx,my))
        {
            mouse_is_hovered_over = true;
        }
        else mouse_is_hovered_over = false;
    }

    if(event.type == SDL_MOUSEBUTTONUP&&event.button.button == SDL_BUTTON_LEFT&&mouse_is_hovered_over)
    {
     *bool_to_toggle = (*bool_to_toggle) ? false : true;
    }

}
