#include "CollisionPoint.hpp"

CollisionPoint::CollisionPoint(int x, int y)
{
    this->x = x;
    this->y = y;
}

void CollisionPoint::move(int new_x, int new_y)
{
    x = new_x;
    y = new_y;
}

bool CollisionPoint::serialize(fstream &file)
{
    if(!file.is_open()) return false;
    int collision_point_version = 1;
    file.write((char*) &collision_point_version, sizeof(collision_point_version));
    file.write((char*) &x, sizeof(x));
    file.write((char*) &y, sizeof(y));
    return true;
}

bool CollisionPoint::unserialize(ifstream &file)
{
    if(!file.is_open()) return false;
    int collision_point_version;
    file.read((char*) &collision_point_version, sizeof(int));
    file.read((char*) &x, sizeof(int));
    file.read((char*) &y, sizeof(int));
    return true;
}
