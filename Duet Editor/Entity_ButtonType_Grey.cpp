#include "Entity_ButtonType_Grey.hpp"
#include <fstream>

Entity_ButtonType_Grey::Entity_ButtonType_Grey(int x, int y, int w, int h)
{
    this->type = "ButtonType_Grey"; //we need the type because it counteracts the ambiguity of polymorphism
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



bool Entity_ButtonType_Grey::serialize(fstream& file)
{
    if(!file.is_open()) return false;
    string::size_type type_length = type.size();
    file.write((char*) &type_length, sizeof type_length);
    file.write(type.data(), type_length); //this is so when we open the file, we can know who this entity is. (Basically my shitty programming, not knowing of Templates)
    int Entity_ButtonType_Grey_version = 1;
    file.write((char*) &Entity_ButtonType_Grey_version, sizeof(Entity_ButtonType_Grey_version));
    file.write((char*) &x, sizeof(x));
    file.write((char*) &y, sizeof(y));
    file.write((char*) &w, sizeof(w));
    file.write((char*) &h, sizeof(h));
    return true;
}

bool Entity_ButtonType_Grey::unserialize(ifstream& file)
{
    if(!file.is_open()) return false;
    int Entity_ButtonType_Grey_version;
    file.read((char*) &Entity_ButtonType_Grey_version, sizeof(int));
    if(Entity_ButtonType_Grey_version==1){
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
