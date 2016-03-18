#ifndef ENTITY_GRASSTYPE_HPP_INCLUDED
#define ENTITY_GRASSTYPE_HPP_INCLUDED
#include "Entity.hpp"
#include "Object.hpp"
#include "game_elements.hpp"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

class Entity_GrassType : public Entity
{
    public:
    int texture_index;
    double sin_input;
    double sin_input2;
    double sway_sin_value;
    int player_extra_breeze;
    double breeze_strength_sin_value;
    void update() {}
    void update(Player &player,float wind_strength);
    Entity_GrassType(int x, int y, int w, int h, int texture_index);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_GRASSTYPE_HPP_INCLUDED
