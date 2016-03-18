#ifndef ENTITY_COLLISIONTYPE_PUZZLE_BARRIER_H_INCLUDED
#define ENTITY_COLLISIONTYPE_PUZZLE_BARRIER_H_INCLUDED
#include "Entity.hpp"
//#include "game_elements.hpp"

class Entity_CollisionType_Puzzle_Barrier : public Entity
{
    public:
    int texture_index, index, tied_respawn_zone_index;
    bool is_open;
    void update() {}
    //void update(Camera camera);
    Entity_CollisionType_Puzzle_Barrier(int x, int y, int w, int h);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_COLLISIONTYPE_H_INCLUDED
