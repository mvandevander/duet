#ifndef STATUS_LIST_HPP_INCLUDED
#define STATUS_LIST_HPP_INCLUDED
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include "GL_DrawingPrimitives.hpp"
//#include <SDL/SDL_ttf.h>
#include "Timer.hpp"

using namespace std;

class Status_List{
    public:
    Timer message_removal_timer;
    vector<string> current_messages;
    void add_message(string message);
    void update();
    void draw(struct Font* font, int x, int y, SDL_Color text_color, SDL_Color outline_color);
};


#endif // STATUS_LIST_HPP_INCLUDED
