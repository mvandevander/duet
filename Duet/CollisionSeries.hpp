#ifndef COLLISIONSERIES_HPP_INCLUDED
#define COLLISIONSERIES_HPP_INCLUDED
#include <vector>
#include "CollisionPoint.hpp"
#include <fstream>
#include "vec_2d.hpp"
#include "point_2d.hpp"


#include <GL/glew.h>

using namespace std;

class CollisionSeries
{
    public:
    CollisionSeries(int first_point_x, int first_point_y);
    vector <CollisionPoint> points;
    void move(int new_x, int new_y);
    void draw(GLuint displaylist, int cam_x, int cam_y, int cam_width, int cam_height, float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
    void addPoint(int new_point_x, int new_point_y);
    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
    CollisionPoint* return_closest_point(int pt_x, int pt_y);
    int return_closest_point_index(int pt_x, int pt_y);
    vec_2d return_closest_segment(point_2d checking_point);
    vec_2d return_closest_segment(int pt_x, int py_y);
    vector<vec_2d> return_all_segments_directly_below(point_2d checking_point);
    vec_2d return_segment_directly_below(point_2d checking_point);

};

#endif // COLLISIONSERIES_HPP_INCLUDED
