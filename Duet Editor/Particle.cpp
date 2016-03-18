#include "Particle.hpp"
#include <math.h>

Particle::Particle(float x, float y, float w, float h, float rot, int max_life, float xv, float yv, float xa, float ya, float rotv, float rota, float widthv, float widtha, float heightv, float heighta)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->rot = rot;
    this->max_life = max_life;
    this->remaining_life = max_life+1;
    this->xv = xv;
    this->yv = yv;
    this->xa = xa;
    this->ya = ya;
    this->rotv = rotv;
    this->rota = rota;
    this->widthv = widthv;
    this->widtha = widtha;
    this->heightv = heightv;
    this->heighta = heighta;
}

void Particle::update()
{

    xv+=(xa);
    yv+=(ya);
    rotv+=rota;
    widthv+=widtha;
    heightv+=heighta;

    x+=xv;
    y+=yv;
    rot+=rotv;
    w+=widthv;
    h+=heightv;

    remaining_life-=1;
    if (remaining_life<=0) remaining_life = 0;


}
