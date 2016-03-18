#ifndef TRIG_FUNCTIONS_DEGREES_HPP_INCLUDED
#define TRIG_FUNCTIONS_DEGREES_HPP_INCLUDED
#include <math.h>
#include <SDL/SDL.h>

double deg2rad(int deg);
double rad2deg(double radian);
double atan2deg(double opp,double adj);
double sin2deg(double angle);
double cos2deg(double angle);
double cosdeg2deg(int angle_in_degrees);
double sindeg2deg(int angle_in_degrees);




int GetDistanceSquared(int x1, int y1, int x2, int y2);
bool DoBoxesIntersect(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);
bool DoBoxesIntersectf(float b1_x, float b1_y, float b1_w, float b1_h, float b2_x, float b2_y, float b2_w, float b2_h);

bool IsPointInRectangle(int x,int y,SDL_Rect rect);
bool IsPointInRectangle(int x,int y,int rx, int ry, int rw, int rh);

#endif // TRIG_FUNCTIONS_DEGREES_HPP_INCLUDED
