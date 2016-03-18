#include "Animation.hpp"
#include "Texture.hpp"
#include <sstream>

void Animation::load(std::string first_filename, unsigned int num_frames, int delay_in_ms)
{
    //note, this assumes that the files are in png format, if this will be an issue, it must be remedied later
    current_frame = 0;
    this->num_frames = num_frames;
    std::stringstream filename_to_load;
    for(unsigned int i = 1; i<=num_frames; i++){
        filename_to_load.str("");
        filename_to_load<<first_filename<<i<<".png";
        string filename = filename_to_load.str();
        Texture temp_texture;
        temp_texture.load(filename);
        Animation_Frame temp_frame(temp_texture,delay_in_ms);
        frames.push_back(temp_frame);
    }
}

void Animation::draw(int x, int y, int rotation, float scale, bool flip_x)
{
    if(flip_x) frames[current_frame].texture.draw(x,y,rotation,scale, true);
    else frames[current_frame].texture.draw(x,y,rotation,scale);
}

void Animation::update()
{
    if(!frame_timer.IsStarted()) {
        frame_timer.Start();
        initial_frame_time = frame_timer.GetTicks();
    }
    if(initial_frame_time+frames[current_frame].delay_in_ms<frame_timer.GetTicks())
    {
        current_frame++;
        if(current_frame>frames.size()-1) current_frame=0;
        frame_timer.Stop();
    }

}

void Animation::reset()
{
    frame_timer.Stop();
    current_frame = 0;
}
