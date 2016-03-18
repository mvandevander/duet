#include "Animation_Frame.hpp"
#include "Texture.hpp"

Animation_Frame::Animation_Frame(Texture loaded_texture, int delay_in_ms)
{
    this->texture = loaded_texture;
    this->delay_in_ms  = delay_in_ms;
}
