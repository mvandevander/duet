#ifndef ENTITY_COLLISIONTYPE_WHITE_BUTTON_H_INCLUDED
#define ENTITY_COLLISIONTYPE_WHITE_BUTTON_H_INCLUDED
#include "Entity.hpp"
//#include "game_elements.hpp"

class Entity_CollisionType_White_Button : public Entity
{
    public:
    int texture_index;
    void update() {}
    //void update(Camera camera);
    Entity_CollisionType_White_Button(int x, int y, int w, int h);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_COLLISIONTYPE_H_INCLUDED
