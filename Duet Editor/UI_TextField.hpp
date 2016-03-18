#ifndef UI_TEXTFIELD_HPP_INCLUDED
#define UI_TEXTFIELD_HPP_INCLUDED
#include "BaseUIElement.hpp"
#include "SDLTypingHandler.hpp"
#include <string>

using namespace std;

class UI_TextField : public BaseUIElement
{
    public:
    int* tied_value;
    //string* tied_value;
    string typed_string, label;
    UI_TextField(string label,int x, int y, int w, int h, int* tied_value);
    //UI_TextField(string label,int x, int y, int w, int h, string* tied_value);
    virtual ~UI_TextField();
    void update(SDL_Event event);
    bool is_taking_typing;
    SDLTypingHandler typing_handler;
};

#endif // UI_TEXTFIELD_HPP_INCLUDED
