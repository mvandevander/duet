#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED
#include <SDL/SDL.h>
#include "Object.hpp"


class Entity : public Object
{
    public:
    virtual void update()=0;
    virtual bool serialize(fstream& file)=0;
    virtual bool unserialize(ifstream& file)=0;
};


#endif // ENTITY_HPP_INCLUDED
