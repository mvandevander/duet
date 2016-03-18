#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED
#include "Texture.hpp"
#include "Animation_Frame.hpp"
#include <vector>
#include "Timer.hpp"

class Animation
{
    public:
    void load(std::string first_filename, unsigned int num_frames, int delay_in_ms);
    void update();
    void reset();
    void draw(int x, int y, int rotation, float scale, bool flip_x = false);
    Timer frame_timer;
    int initial_frame_time;
    unsigned int num_frames;
    unsigned int current_frame;
    vector <Animation_Frame> frames;
};



#endif // ANIMATION_HPP_INCLUDED
