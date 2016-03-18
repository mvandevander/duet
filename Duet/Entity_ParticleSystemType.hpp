#ifndef ENTITY_PARTICLESYSTEMTYPE_HPP_INCLUDED
#define ENTITY_PARTICLESYSTEMTYPE_HPP_INCLUDED
#include "Particle.hpp"
#include <vector>
#include "Entity.hpp"
#include "Object.hpp"

class Entity_ParticleSystemType : public Entity
{
    public:
    int texture_index;
    vector <Particle> particles;
    //particle creation parameters
    int particle_frequency, particle_min_life, particle_max_life, particle_min_xv, particle_max_xv, particle_min_xa, particle_max_xa, particle_min_yv,
        particle_max_yv, particle_min_ya, particle_max_ya, particle_min_rot, particle_max_rot, particle_min_rotv, particle_max_rotv, particle_min_rota, particle_max_rota,
        particle_min_width, particle_max_width, particle_min_widthv, particle_max_widthv, particle_min_widtha, particle_max_widtha,
        particle_min_height, particle_max_height, particle_min_heightv, particle_max_heightv, particle_min_heighta, particle_max_heighta,
        particle_middle_t, particle_middle_len;
    float particle_start_r, particle_start_g, particle_start_b, particle_start_a, particle_middle_r, particle_middle_g, particle_middle_b, particle_middle_a,
        particle_end_r, particle_end_g, particle_end_b, particle_end_a;
    void update();
    void set_parameters(int particle_frequency, int particle_min_life, int particle_max_life, int particle_min_xv, int particle_max_xv, int particle_min_xa, int particle_max_xa, int particle_min_yv,
        int particle_max_yv, int particle_min_ya, int particle_max_ya, int particle_min_rot, int particle_max_rot, int particle_min_rotv, int particle_max_rotv, int particle_min_rota, int particle_max_rota,
        int particle_min_width, int particle_max_width, int particle_min_widthv, int particle_max_widthv, int particle_min_widtha, int particle_max_widtha,
        int particle_min_height, int particle_max_height, int particle_min_heightv, int particle_max_heightv, int particle_min_heighta, int particle_max_heighta,
        int particle_middle_t, int particle_middle_len, float particle_start_r, float particle_start_g, float particle_start_b, float particle_start_a,
        float particle_middle_r, float particle_middle_g, float particle_middle_b, float particle_middle_a, float particle_end_r, float particle_end_g,
        float particle_end_b, float particle_end_a);
    virtual ~Entity_ParticleSystemType();
    Entity_ParticleSystemType(int x, int y, int w, int h, int texture_index);

    bool serialize(fstream& file);
    bool unserialize(ifstream& file);
};

#endif // ENTITY_SIGNTYPE_H_INCLUDED
