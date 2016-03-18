#include "trig_functions_degrees.hpp"
#include <SDL/SDL.h>

using namespace std;

double atan2deg(double opp,double adj)
{
    double adjusted_var = atan2(opp,adj)*57.29578;
    return adjusted_var;
}

double sin2deg(double angle_in_radians)
{
    double adjusted_var = sin(angle_in_radians)*57.29578;
    return adjusted_var;
}

double cos2deg(double angle_in_radians)
{
    double adjusted_var = cos(angle_in_radians)*57.29578;
    return adjusted_var;
}


double cosdeg2deg(int angle_in_degrees)
{
    double adjusted_var = cos(deg2rad(angle_in_degrees))*57.29578;
    return adjusted_var;
}

double sindeg2deg(int angle_in_degrees)
{
    double adjusted_var = sin(deg2rad(angle_in_degrees))*57.29578;
    return adjusted_var;
}

double rad2deg(double radian)
{
   return radian*57.29578;
}

double deg2rad(int deg)
{
    return deg*0.017453;
}


int GetDistanceSquared(int x1, int y1, int x2, int y2)
{
        int dist_x = abs(x1-x2);
        int dist_y = abs(y1-y2);
        return ((dist_x*dist_x)+(dist_y*dist_y));
}

bool DoBoxesIntersect(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h)
{
    int b1_l = b1_x;
    int b2_l = b2_x;
    int b1_r = b1_x+b1_w;
    int b2_r = b2_x+b2_w;
    int b1_t = b1_y;
    int b2_t = b2_y;
    int b1_b = b1_y+b1_h;
    int b2_b = b2_y+b2_h;
    if(b1_l<b2_r&&b2_l<b1_r&&b1_t<b2_b&&b2_t<b1_b) return true;
    else return false;
}

bool DoBoxesIntersectf(float b1_x, float b1_y, float b1_w, float b1_h, float b2_x, float b2_y, float b2_w, float b2_h)
{
    float b1_l = b1_x;
    float b2_l = b2_x;
    float b1_r = b1_x+b1_w;
    float b2_r = b2_x+b2_w;
    float b1_t = b1_y;
    float b2_t = b2_y;
    float b1_b = b1_y+b1_h;
    float b2_b = b2_y+b2_h;
    if(b1_l<b2_r&&b2_l<b1_r&&b1_t<b2_b&&b2_t<b1_b) return true;
    else return false;
}



bool IsPointInRectangle(int x,int y,SDL_Rect rect)
{
    if((x > rect.x) && (y > rect.y) && (x < rect.x + rect.w) && (y < rect.y + rect.h)){
            return true;
    }
    else return false;
}

bool IsPointInRectangle(int x,int y,int rx, int ry, int rw, int rh)
{
    SDL_Rect rect;
    rect.x = rx;
    rect.y = ry;
    rect.w = rw;
    rect.h = rh;
    if((x > rect.x) && (y > rect.y) && (x < rect.x + rect.w) && (y < rect.y + rect.h)){
            return true;
    }
    else return false;
}

