#ifndef ANIMATION_FRAME_HPP_INCLUDED
#define ANIMATION_FRAME_HPP_INCLUDED
#include "Texture.hpp"
//#include "Animation.hpp"

class Animation_Frame{
    public:
        Animation_Frame(Texture loaded_texture, int delay_in_ms);
        Texture texture;
        short int delay_in_ms;
};

#endif // ANIMATION_FRAME_HPP_INCLUDED
