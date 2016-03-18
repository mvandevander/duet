#ifndef PARTICLE_HPP_INCLUDED
#define PARTICLE_HPP_INCLUDED

class Particle{
    public:
    float x, y, w, h; //position is relative to the particle system
    float rot, xv, yv, xa, ya, rotv, rota, widthv, widtha, heightv, heighta;
    int max_life, remaining_life;
        void update();
    Particle(float x, float y, float w, float h, float rot, int max_life, float xv, float yv, float xa, float ya, float rotv, float rota, float widthv, float widtha, float heightv, float heighta);
};


#endif // POINT_2D_HPP_INCLUDED
