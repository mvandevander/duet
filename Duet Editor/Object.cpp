#include "Object.hpp"
#include <SDL/SDL.h>

void Object::move(int new_x, int new_y)
{
    this->x = new_x;
    this->y = new_y;
    this->bounds.x = this->x-(w/2);
    this->bounds.y = this->y-(h/2);
}

void Object::rotate(float new_rotation)
{
    this->rotation = new_rotation;
}

void Object::scale(int new_w, int new_h)
{
    this->w = new_w;
    this->h = new_h;
    this->bounds.x = x-(w/2);
    this->bounds.y = y-(h/2);
    this->bounds.w = w;
    this->bounds.h = h;
}
