#include "SDLTypingHandler.hpp"
#include <SDL/SDL.h>
#include <string>

using namespace std;

void SDLTypingHandler::update(SDL_Event event)
{
    if(SDL_EnableUNICODE(-1)) unicode_enabled_globally = true;
    else {
        unicode_enabled_globally = false;
        SDL_EnableUNICODE(1);
    }
	if(event.type == SDL_KEYDOWN){
		Uint16 key = event.key.keysym.unicode;
		if(key == (Uint16)' ') inputted_string+=(char)key;
		else if(key>=(Uint16)'!'&& key<=(Uint16)'~') inputted_string+=(char)key;
		if(event.key.keysym.sym == SDLK_BACKSPACE&&inputted_string.length() != 0){
			inputted_string.erase(inputted_string.length() - 1);
		}
	}
}

std::string SDLTypingHandler::return_string()
{
return inputted_string;
}

const char* SDLTypingHandler::return_c_string()
{
return inputted_string.c_str();
}

void SDLTypingHandler::clear()
{
    if(!unicode_enabled_globally) SDL_EnableUNICODE(0);
    inputted_string.clear();
}
