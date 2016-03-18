#ifndef ENTITY_GRASSTYPE_HPP_INCLUDED
#define ENTITY_GRASSTYPE_HPP_INCLUDED
#include "Entity.hpp"
#include "Object.hpp"

class Entity_GrassType : public Entity
{
    public:
    int texture_index;
    int sway_sin_value;
    int breeze_strength_sin_value;
    void update();
    Entity_GrassType(int x, int y, int w, int h, int texture_index);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_GRASSTYPE_HPP_INCLUDED
