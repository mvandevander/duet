#include "UI_Slideri.hpp"
#include <string>


UI_Slideri::UI_Slideri(string label, struct Font* font, int x, int y, int w, int h, int *var_to_manipulate, int low, int high)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->low = low;
    this->high = high;
    //TTF_SizeText(font,label.c_str(),&w,&h);
    //w+=8;
    //h+=8;
    this->label = label;
    slider_is_grabbed = false;
    this->var_to_manipulate = var_to_manipulate;
    current_percent = (*var_to_manipulate - low)/high;
    this->type = "Slideri";
}

void UI_Slideri::update(SDL_Event event)
{
    int total_range = high-low;
    current_percent = (float)((*var_to_manipulate - low))/total_range;
    if(event.type == SDL_MOUSEMOTION)
    {
        mx = event.motion.x;
        my = event.motion.y;
        if(is_mouse_over(mx,my))
        {
            mouse_is_hovered_over = true;
        }
        else mouse_is_hovered_over = false;
    }

    if(event.type == SDL_MOUSEBUTTONDOWN&&event.button.button == SDL_BUTTON_LEFT&&mouse_is_hovered_over)
    {
        slider_is_grabbed = true;
    }
    if(event.type == SDL_MOUSEBUTTONUP&&event.button.button == SDL_BUTTON_LEFT)
    {
        slider_is_grabbed = false;
    }
    if(slider_is_grabbed){
        //so we just want to find the offset of the mouse cursor from the left side of the slider
        //and then convert it to a floating point number and map it to the range of the var_to_manipulate
        //as specified by the low and high variables
        float offset = mx-x;
        float percent;
        percent = offset/w;
        if(offset<0) percent = 0;
        else if(offset>w) percent = 1.0;
        int final_val = int((percent*total_range)+low);
        *var_to_manipulate = final_val;
    }

}
