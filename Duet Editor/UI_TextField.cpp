#include "BaseUIElement.hpp"
#include "UI_TextField.hpp"
#include <string>
#include <sstream>


using namespace std;

UI_TextField::UI_TextField(string label, int x, int y, int w, int h, int* tied_value)
{
    is_taking_typing = false;
    this->type = "TextField"; //counteract ambiguity due to polymorphism
    this->label = label;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->tied_value = tied_value;
    stringstream temp;
    temp<<*tied_value;
    this->typed_string=temp.str();
}

/*UI_TextField::UI_TextField(string label, int x, int y, int w, int h, string* tied_value)
{
    is_taking_typing = false;
    this->type = 't'; //counteract ambiguity due to polymorphism
    this->label = label;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->tied_value = tied_value;
    this->typed_string=*tied_value;
}*/

UI_TextField::~UI_TextField()
{
    tied_value = NULL;
}

void UI_TextField::update(SDL_Event event)
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
    if(event.type == SDL_MOUSEBUTTONUP&&event.button.button == SDL_BUTTON_LEFT)
    {
        if(mouse_is_hovered_over) is_taking_typing = true;
        else{
            stringstream temp;
            temp<<*tied_value;
            typed_string=temp.str();
            typing_handler.clear();
            is_taking_typing = false;
        }
    }
    if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_RETURN&&is_taking_typing)
    {
        is_taking_typing = false;
        //stringstream(typed_string) >> *tied_value;
        //stringstream temp;
        //temp<<*tied_value;
        //typed_string=temp.str();
        typing_handler.clear();
    }
    if(is_taking_typing){
        typing_handler.update(event);
        typed_string.assign(typing_handler.return_string());
        //fprintf(stderr,typed_string.c_str());
        stringstream(typed_string) >> *tied_value;
        stringstream temp;
        temp<<*tied_value;
    }
}
