#ifndef ENTITY_RESPAWNZONETYPE_H_INCLUDED
#define ENTITY_RESPAWNZONETYPE_H_INCLUDED
#include "Entity.hpp"
//#include "game_elements.hpp"

class Entity_RespawnZoneType : public Entity
{
    public:
    //int texture_index;
    void update() {}
    int index;
    //void update(Camera camera);
    Entity_RespawnZoneType(int x, int y, int w, int h);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_COLLISIONTYPE_H_INCLUDED
