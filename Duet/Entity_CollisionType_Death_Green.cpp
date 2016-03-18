#include "Entity_CollisionType_Death_Green.hpp"
#include <fstream>

Entity_CollisionType_Death_Green::Entity_CollisionType_Death_Green(int x, int y, int w, int h)
{
    this->type = "CollisionType_Death_Green"; //we need the type because it counteracts the ambiguity of polymorphism
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->rotation = 0;
    this->parallax_offset = 1.0;
    this->u = 0;
    this->v = 0;
    this->is_flipped_horizontally = false;
    this->bounds.x = this->x-(this->w/2);
    this->bounds.y = this->y-(this->h/2);
    this->bounds.w = this->w;
    this->bounds.h = this->h;
}

/*void Entity_SignType::update(Camera camera)
{
    if(GetDistanceSquared(x,y,camera.x,camera.y)<10000&&fade_state==0)
    {
        fade_state = 1;
    }
    else if(GetDistanceSquared(x,y,camera.x,camera.y)>10000&&fade_state==1)
    {
        fade_state = 3;
    }
    if(fade_state==1)
    {
        fade_alpha+=.001;
    }
    else if(fade_state==2)
    {
        fade_alpha-=.001;
    }
}*/



bool Entity_CollisionType_Death_Green::serialize(fstream& file)
{
    if(!file.is_open()) return false;
    string::size_type type_length = type.size();
    file.write((char*) &type_length, sizeof type_length);
    file.write(type.data(), type_length); //this is so when we open the file, we can know who this entity is. (Basically my shitty programming, not knowing of Templates)

    int Entity_CollisionType_Death_Green_version = 1;
    file.write((char*) &Entity_CollisionType_Death_Green_version, sizeof(Entity_CollisionType_Death_Green_version));
    file.write((char*) &x, sizeof(x));
    file.write((char*) &y, sizeof(y));
    file.write((char*) &w, sizeof(w));
    file.write((char*) &h, sizeof(h));
    return true;
}

bool Entity_CollisionType_Death_Green::unserialize(ifstream& file)
{
    if(!file.is_open()) return false;
    int Entity_CollisionType_Death_Green_version;
    file.read((char*) &Entity_CollisionType_Death_Green_version, sizeof(int));
    if(Entity_CollisionType_Death_Green_version==1){
        file.read((char*) &x, sizeof(x));
        file.read((char*) &y, sizeof(y));
        file.read((char*) &w, sizeof(w));
        file.read((char*) &h, sizeof(h));
        bounds.x = x-(w/2);
        bounds.y = y-(h/2);
        bounds.w = w;
        bounds.h = h;
        return true;
    }
    else return false;
}
