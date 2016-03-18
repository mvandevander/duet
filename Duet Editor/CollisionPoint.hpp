#ifndef COLLISIONPOINT_HPP_INCLUDED
#define COLLISIONPOINT_HPP_INCLUDED
#include <fstream>
using namespace std;

class CollisionPoint
{
    public:
    int x, y;
    CollisionPoint(int x, int y);
    bool serialize(fstream &file);
    bool unserialize(ifstream &file);
    void move(int new_x, int new_y);

};


#endif // COLLISIONPOINT_HPP_INCLUDED
