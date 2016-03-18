#ifndef SDLTYPINGHANDLER_HPP_INCLUDED
#define SDLTYPINGHANDLER_HPP_INCLUDED
#include <string>
#include <SDL/SDL.h>

class SDLTypingHandler
{
	private:
	std::string inputted_string;
	bool unicode_enabled_globally;
	public:
	void update(SDL_Event& event);
	void clear();
	std::string return_string();
	const char* return_c_string();
};

#endif //SDLTYPINGHANDLER_HPP_INCLUDED
