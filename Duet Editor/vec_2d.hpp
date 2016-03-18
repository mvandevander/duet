#ifndef VEC_2D_HPP_INCLUDED
#define VEC_2D_HPP_INCLUDED
#include "point_2d.hpp"

class vec_2d{
    public:
    vec_2d(point_2d point1, point_2d point2);
    vec_2d(int p0x = 0, int p0y = 0, int p1x = 0, int p1y = 0);
    float dot(vec_2d v2);
    float dot(int v2vx, int v2vy);
    float dot(float v2vx, float v2vy);
    int find_y_at(int x);
    int find_x_at(int y);
    void draw(float r = 1.00, float g = 1.00, float b = 1.00, float a = 1.00);
    point_2d p0;
    point_2d p1;
    int vx, vy, rx, ry, lx, ly;
    float dx, dy, len;
};

vec_2d find_shortest_vector_from_point_to_line(point_2d point, vec_2d line);
vec_2d project_vector(vec_2d v1, vec_2d v2);
//vec_2d project_vector(vec_2d v1, float v2dx, float v2dy);

#endif // VEC_2D_HPP_INCLUDED
