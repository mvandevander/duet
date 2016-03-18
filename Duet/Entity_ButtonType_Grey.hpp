#ifndef ENTITY_BUTTONTYPE_GREY_H_INCLUDED
#define ENTITY_BUTTONTYPE_GREY_H_INCLUDED
#include "Entity.hpp"
//#include "game_elements.hpp"

class Entity_ButtonType_Grey : public Entity
{
    public:
    int texture_index;
    void update() {}
    //void update(Camera camera);
    Entity_ButtonType_Grey(int x, int y, int w, int h);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_COLLISIONTYPE_H_INCLUDED
