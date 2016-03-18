#ifndef BASEUIELEMENT_HPP_INCLUDED
#define BASEUIELEMENT_HPP_INCLUDED
#include <SDL/SDL.h>
#include <string>

using namespace std;

class BaseUIElement{
    public:
    string type;
    int x, y, w, h;
    bool mouse_is_hovered_over;
    bool is_mouse_over(int mx,int my){
            if((mx > x) && (my > y) && (mx < x + w) && (my < y + h)){
            return true;
            }
            else return false;
        }
    virtual void update(SDL_Event event)=0;
    virtual ~BaseUIElement() {}
};

#endif // BASEUIELEMENT_HPP_INCLUDED
