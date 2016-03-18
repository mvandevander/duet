#ifndef COLLISIONSERIES_HPP_INCLUDED
#define COLLISIONSERIES_HPP_INCLUDED
#include <vector>
#include "CollisionPoint.hpp"
#include <fstream>
#include <GL/glew.h>

using namespace std;

class CollisionSeries
{
    public:
    CollisionSeries(int first_point_x, int first_point_y);
    vector <CollisionPoint> points;
    void move(int new_x, int new_y);
    void draw(GLuint displaylist,float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
    void addPoint(int new_point_x, int new_point_y);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
    CollisionPoint* return_closest_point(int pt_x, int pt_y);
    int return_closest_point_index(int pt_x, int pt_y);

};

#endif // COLLISIONSERIES_HPP_INCLUDED
