#include "vec_2d.hpp"
#include "point_2d.hpp"
#include <math.h>
#include "GL_DrawingPrimitives.hpp"

vec_2d::vec_2d(int p0x, int p0y, int p1x, int p1y)
{
    p0.x = p0x;
    p0.y = p0y;
    p1.x = p1x;
    p1.y = p1y;
    vx = p1.x-p0.x;
    vy = p1.y-p0.y;
    len = sqrt(vx*vx+vy*vy);
    if(len!=0){
        dx = vx/len;
        dy = vy/len;
    }
    else{
        dx = 0;
        dy = 0;
    }
    rx = -vy;
    ry = vx;
    lx = vy;
    ly = -vx;

}

vec_2d::vec_2d(point_2d point0, point_2d point1)
{
    p0 = point0;
    p1 = point1;
    vx = p1.x-p0.x;
    vy = p1.y-p0.y;
    len = sqrt(vx*vx+vy*vy);
    if(len!=0){
        dx = vx/len;
        dy = vy/len;
    }
    else{
        dx = 0;
        dy = 0;
    }
    rx = -vy;
    ry = vx;
    lx = vy;
    ly = -vx;

}


float vec_2d::dot(vec_2d v2)
{
    return (vx*v2.vx + vy*v2.vy);
}

float vec_2d::dot(int v2vx, int v2vy)
{
    return (vx*v2vx + vy*v2vy);
}

float vec_2d::dot(float v2vx, float v2vy)
{
    return (vx*v2vx + vy*v2vy);
}

void vec_2d::draw(float r, float g, float b, float a)
{
    DrawGLLineFromCurrentMatrix(p0.x,p0.y,p1.x,p1.y,r,g,b,a);
    //DrawGLLineFromCurrentMatrix(p0.x,p0.y,p0.x+lx,p0.y+ly,0.3,1.0,1.0,1.0);
    //DrawGLLineFromCurrentMatrix(p0.x,p0.y,p0.x+rx,p0.y+ry,0.0,0.0,1.0,1.0);
}

int vec_2d::find_y_at(int x)
{
    //we must project the offset of the checked x value onto the vector in order to find the point below on the surface of that vector
    vec_2d below_vector_x_offset(p0.x,p0.y,x,p0.y);
    vec_2d below_vector_projected_x_offset = project_vector(below_vector_x_offset,*this);
    return below_vector_projected_x_offset.p1.y;
}


int vec_2d::find_x_at(int y)
{
    //we must project the offset of the checked y value onto the vector in order to find the point below on the surface of that vector
    vec_2d below_vector_y_offset(p0.x,p0.y,p0.x,y);
    vec_2d below_vector_projected_y_offset = project_vector(below_vector_y_offset,*this);
    return below_vector_projected_y_offset.p1.x;
}

vec_2d project_vector(vec_2d v1, vec_2d v2)
{
    float dp = v1.dot(v2.dx,v2.dy);
    vec_2d v3(v2.p0.x,v2.p0.y,int(v2.p0.x+(dp*v2.dx)),int(v2.p0.y+(dp*v2.dy)));
    return v3;

}

/*vec_2d project_vector(vec_2d v1, float v2dx, float v2dy)
{
    float dp = v1.dot(v2dx,v2dy);
    vec_2d v3(v2.p0.x,v2.p0.y,(int)(v2.p0.x+(dp*v2.dx)),(int)(v2.p0.y+(dp*v2.dy)));
    return v3;
}*/


vec_2d find_shortest_vector_from_point_to_line(point_2d point,vec_2d line){
        vec_2d shortest_vector_to_line;
        vec_2d from_line_start_to_point(line.p0.x,line.p0.y,point.x,point.y);
        float dp = from_line_start_to_point.dot(line.dx,line.dy);
        if(dp<0){
            shortest_vector_to_line=from_line_start_to_point;
        }
        else{
            vec_2d from_line_end_to_point(line.p1.x,line.p1.y,point.x,point.y);
            float dp = from_line_end_to_point.dot(line.dx,line.dy);
            if(dp>0){
                shortest_vector_to_line=from_line_end_to_point;
            }
            else{
            vec_2d line_normal(line.p0.x,line.p0.y,line.p0.x+line.lx,line.p0.y+line.ly);
              shortest_vector_to_line = project_vector(from_line_start_to_point,line_normal);
            }
        }
        return shortest_vector_to_line;
}

