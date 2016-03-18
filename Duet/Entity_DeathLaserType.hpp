#ifndef Entity_DeathLaserType_H_INCLUDED
#define Entity_DeathLaserType_H_INCLUDED
#include "Entity.hpp"
//#include "game_elements.hpp"

class Entity_DeathLaserType : public Entity
{
    public:
    int texture_index;
    bool is_triggered, is_firing;
    short firing_delay_time, laser_lifetime;
    int laser_beam_length;

    void update() {}
    //void update(Camera camera);
    Entity_DeathLaserType(int x, int y, int w, int h);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_COLLISIONTYPE_H_INCLUDED
