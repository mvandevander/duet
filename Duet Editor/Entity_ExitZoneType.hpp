#ifndef Entity_ExitZoneType_H_INCLUDED
#define Entity_ExitZoneType_H_INCLUDED
#include "Entity.hpp"
//#include "game_elements.hpp"

class Entity_ExitZoneType : public Entity
{
    public:
    int texture_index;
    int puzzle_barrier_index;
    void update() {}
    //void update(Camera camera);
    Entity_ExitZoneType(int x, int y, int w, int h);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_COLLISIONTYPE_H_INCLUDED
