#include "Entity_GrassType.hpp"
#include <fstream>

Entity_GrassType::Entity_GrassType(int x, int y, int w, int h, int texture_index)
{
    this->type = "GrassType"; //we need the type because it counteracts the ambiguity of polymorphism
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->texture_index = texture_index;
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

void Entity_GrassType::update()
{

}



bool Entity_GrassType::serialize(fstream& file)
{
    if(!file.is_open()) return false;
    string::size_type type_length = type.size();
    file.write((char*) &type_length, sizeof type_length);
    file.write(type.data(), type_length); //this is so when we open the file, we can know who this entity is. (Basically my shitty programming, not knowing of Templates)

    int entity_grasstype_version = 1;
    file.write((char*) &entity_grasstype_version, sizeof(entity_grasstype_version));
    file.write((char*) &x, sizeof(x));
    file.write((char*) &y, sizeof(y));
    file.write((char*) &w, sizeof(w));
    file.write((char*) &h, sizeof(h));
    file.write((char*) &texture_index, sizeof(texture_index));
    return true;
}

bool Entity_GrassType::unserialize(ifstream& file)
{
    if(!file.is_open()) return false;
    int entity_grasstype_version;
    file.read((char*) &entity_grasstype_version, sizeof(int));
    if(entity_grasstype_version==1){
        file.read((char*) &x, sizeof(x));
        file.read((char*) &y, sizeof(y));
        file.read((char*) &w, sizeof(w));
        file.read((char*) &h, sizeof(h));
        file.read((char*) &texture_index, sizeof(texture_index));
        bounds.x = x-(w/2);
        bounds.y = y-(h/2);
        bounds.w = w;
        bounds.h = h;
        return true;
    }
    else return false;
}
