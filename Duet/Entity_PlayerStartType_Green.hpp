#ifndef ENTITY_PLAYERSTARTTYPE_GREEN_H_INCLUDED
#define ENTITY_PLAYERSTARTTYPE_GREEN_H_INCLUDED
#include "Entity.hpp"
//#include "game_elements.hpp"

class Entity_PlayerStartType_Green : public Entity
{
    public:
    //int texture_index;
    void update() {}
    //void update(Camera camera);
    Entity_PlayerStartType_Green(int x, int y, int w, int h);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_COLLISIONTYPE_H_INCLUDED
