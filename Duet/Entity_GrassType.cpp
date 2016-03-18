#include "Entity_GrassType.hpp"
#include "trig_functions_degrees.hpp"
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

Entity_GrassType::Entity_GrassType(int x, int y, int w, int h, int texture_index)
{
    this->type = "GrassType"; //we need the type because it counteracts the ambiguity of polymorphism
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->texture_index = texture_index;
    this->rotation = 0;
    this->bounds.x = this->x-(this->w/2);
    this->bounds.y = this->y-(this->h/2);
    this->bounds.w = this->w;
    this->bounds.h = this->h;
    sin_input = rand();
    sin_input2 = rand();
    player_extra_breeze = 0;
}

void Entity_GrassType::update(Player &player, float wind_strength)
{
    sin_input2+=.34;
    if(GetDistanceSquared(x,y,player.x,player.y)<1000&&player.state==PLAYER_RUNNING)
    {
        player_extra_breeze=200;
    }
    if(sin_input2>=360) sin_input2-=360;
    breeze_strength_sin_value = sindeg2deg((int)sin_input2);
    if(breeze_strength_sin_value<0) breeze_strength_sin_value*=-1;
    sin_input+=((wind_strength/1.7453)*(breeze_strength_sin_value/10));
    sin_input+=(wind_strength/.87);
    if(player_extra_breeze>0){
        sin_input+=(double(player_extra_breeze)/20.94);
        player_extra_breeze-=1;
    }
    if(sin_input>=360.0) sin_input-=360;
    sway_sin_value = sindeg2deg((int)sin_input);
    rotation=(int)sway_sin_value/2;
}





bool Entity_GrassType::serialize(fstream& file)
{
    if(!file.is_open()) return false;
    file.write((char*) &type, sizeof(type)); //so that when we read in the file, we can tell which type of entity it is.
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
