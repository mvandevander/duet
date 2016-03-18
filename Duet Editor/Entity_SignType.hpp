#ifndef ENTITY_SIGNTYPE_H_INCLUDED
#define ENTITY_SIGNTYPE_H_INCLUDED
#include "Entity.hpp"
//#include "game_elements.hpp"
#include "Timer.hpp"

class Entity_SignType : public Entity
{
    public:
    short fade_state;
    float fade_alpha;
    Timer FadeDelayTimer;
    int fade_delay_ms;
    int texture_index;
    void update() {}
    //void update(Camera camera);
    Entity_SignType(int x, int y, int w, int h, int rot, int texture_index, int fade_delay_ms);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_SIGNTYPE_H_INCLUDED
