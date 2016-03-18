#include "Entity_ParticleSystemType.hpp"
#include <fstream>
#include <vector>
#include "random_number_generators.hpp"

Entity_ParticleSystemType::Entity_ParticleSystemType(int x, int y, int w, int h, int texture_index)
{
    this->type = 'p'; //we need the type because it counteracts the ambiguity of polymorphism
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->texture_index = texture_index;
    this->bounds.x = this->x-(this->w/2);
    this->bounds.y = this->y-(this->h/2);
    this->bounds.w = this->w;
    this->bounds.h = this->h;
}

Entity_ParticleSystemType::~Entity_ParticleSystemType()
{

}

    void Entity_ParticleSystemType::set_parameters(int particle_frequency, int particle_min_life, int particle_max_life, int particle_min_xv, int particle_max_xv, int particle_min_xa,
                                                   int particle_max_xa, int particle_min_yv, int particle_max_yv, int particle_min_ya, int particle_max_ya, int particle_min_rot, int particle_max_rot,
                                                   int particle_min_rotv, int particle_max_rotv, int particle_min_rota, int particle_max_rota, int particle_min_width,
                                                   int particle_max_width, int particle_min_widthv, int particle_max_widthv, int particle_min_widtha, int particle_max_widtha,
                                                   int particle_min_height, int particle_max_height, int particle_min_heightv, int particle_max_heightv, int particle_min_heighta, int particle_max_heighta,
                                                   int particle_middle_t, int particle_middle_len, float particle_start_r, float particle_start_g, float particle_start_b,
                                                   float particle_start_a, float particle_middle_r, float particle_middle_g, float particle_middle_b, float particle_middle_a,
                                                   float particle_end_r, float particle_end_g, float particle_end_b, float particle_end_a)
                                                   {
                                                       this->particle_frequency = particle_frequency;
                                                       this->particle_min_life = particle_min_life;
                                                       this->particle_max_life = particle_max_life;
                                                       this->particle_min_xv = particle_min_xv;
                                                       this->particle_max_xv = particle_max_xv;
                                                       this->particle_min_xa = particle_min_xa;
                                                       this->particle_max_xa = particle_max_xa;
                                                       this->particle_min_yv = particle_min_yv;
                                                       this->particle_max_yv = particle_max_xv;
                                                       this->particle_min_ya = particle_min_ya;
                                                       this->particle_max_ya = particle_max_ya;
                                                       this->particle_min_rotv = particle_min_rotv;
                                                       this->particle_max_rotv = particle_max_rotv;
                                                       this->particle_min_rota = particle_min_rota;
                                                       this->particle_max_rota = particle_max_rota;
                                                       this->particle_min_width = particle_min_width;
                                                       this->particle_max_width = particle_max_width;
                                                       this->particle_min_widthv = particle_min_widthv;
                                                       this->particle_max_widthv = particle_max_widthv;
                                                       this->particle_min_widtha = particle_min_widtha;
                                                       this->particle_max_widtha = particle_max_widtha;
                                                       this->particle_min_height = particle_min_height;
                                                       this->particle_max_height = particle_max_height;
                                                       this->particle_min_heightv = particle_min_heightv;
                                                       this->particle_max_heightv = particle_max_heightv;
                                                       this->particle_min_heighta = particle_min_heighta;
                                                       this->particle_max_heighta = particle_max_heighta;
                                                       this->particle_middle_t = particle_middle_t;
                                                       this->particle_middle_len = particle_middle_len;
                                                       this->particle_start_r = particle_start_r;
                                                       this->particle_start_g = particle_start_g;
                                                       this->particle_start_b = particle_start_b;
                                                       this->particle_start_a = particle_start_a;
                                                       this->particle_middle_r = particle_middle_r;
                                                       this->particle_middle_g = particle_middle_g;
                                                       this->particle_middle_b = particle_middle_b;
                                                       this->particle_middle_a = particle_middle_a;
                                                       this->particle_end_r = particle_end_r;
                                                       this->particle_end_g = particle_end_g;
                                                       this->particle_end_b = particle_end_b;
                                                       this->particle_end_a = particle_end_a;
                                                   }
void Entity_ParticleSystemType::update()
{
    for(int i = particle_frequency; i>0; i--)
    {
        particles.push_back( Particle(random(-w/2,w/2), random(-h/2,h/2), random(particle_min_width,particle_max_width), random(particle_min_height,particle_max_height),
                                         random(particle_min_rot,particle_max_rot), random(particle_min_life,particle_max_life), random(particle_min_xv,particle_max_xv),
                                         random(particle_min_yv,particle_max_yv), random(particle_min_xa,particle_max_xa), random(particle_min_ya,particle_max_ya),
                                         random(particle_min_rotv,particle_max_rotv),random(particle_min_rota,particle_max_rota),random(particle_min_widthv,particle_max_widthv),
                                         random(particle_min_widtha,particle_max_widtha),random(particle_min_heightv,particle_max_heightv),
                                         random(particle_min_heighta,particle_max_heighta)));
    }

    for(unsigned int i=0;i<particles.size();i++)
    {
        particles[i].update();
        if(particles[i].remaining_life == 0)
        {
            //do some more work here, particles seem to be leaking through that should be killed
                    //delete particles[i]; //the data stored at the pointer must be deleted
                    particles.erase(particles.begin() + i);
                    i-=1;
        }
    }
}

bool Entity_ParticleSystemType::serialize(fstream& file)
{
    if(!file.is_open()) return false;
    file.write((char*) &type, sizeof(type)); //so that when we read in the file, we can tell which type of entity it is.
    int entity_signtype_version = 2;
    file.write((char*) &entity_signtype_version, sizeof(entity_signtype_version));
    file.write((char*) &x, sizeof(x));
    file.write((char*) &y, sizeof(y));
    file.write((char*) &w, sizeof(w));
    file.write((char*) &h, sizeof(h));
    file.write((char*) &texture_index, sizeof(texture_index));
    return true;
}

bool Entity_ParticleSystemType::unserialize(ifstream& file)
{
    if(!file.is_open()) return false;
    int entity_signtype_version;
    file.read((char*) &entity_signtype_version, sizeof(int));
    if(entity_signtype_version==1){
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
    else if(entity_signtype_version==2){
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
