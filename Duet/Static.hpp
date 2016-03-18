#ifndef STATIC_HPP_INCLUDED
#define STATIC_HPP_INCLUDED
#include "Object.hpp"

using namespace std;

class Static : public Object
{
    public:
    Static(int x, int y, int w, int h, int rotation, int texture_index);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
    void update(){}
};


#endif // STATIC_HPP_INCLUDED
