#include "Game.hpp"

using namespace std;

bool DEBUG_MODE_IS_ON = false;
const bool FPS_IS_SHOWN = true;
bool FPS_IS_CAPPED = true;
bool VSYNC_IS_ENABLED = true;
const bool CULLED_DRAWING_IS_ON = true;
bool FULLSCREEN_MODE_IS_ON = true;

const short FPS_MAX = 60;

int main(int argc, char* args[])
{


    if(argc>0){
        fprintf(stdout,"Arguments:\n");
        for(int i=1;i<argc;i++)
        {
            string argument;
            argument.assign(args[i]);
            if(argument == "-windowed") FULLSCREEN_MODE_IS_ON = false;
            if(argument == "-novsync") VSYNC_IS_ENABLED = false;
            if(argument == "-debug") DEBUG_MODE_IS_ON = true;
            if(argument == "-nofpslimit") FPS_IS_CAPPED = false;
            fprintf(stdout, "Arg: %s\n",args[i]);
        }
    }

    Game Duet;

    if(FULLSCREEN_MODE_IS_ON) Duet.fullscreen_mode = true;
    else Duet.fullscreen_mode = false;

    if(!Duet.initialize()) return 1;

    if(DEBUG_MODE_IS_ON) Duet.toggle_debug_mode();
    if(FPS_IS_SHOWN) Duet.toggle_show_fps();
    if(!FPS_IS_CAPPED) Duet.toggle_limit_fps();
    if(!VSYNC_IS_ENABLED) Duet.toggle_vsync();
    if(!CULLED_DRAWING_IS_ON) Duet.toggle_culled_drawing();

    Duet.change_fps_limit(FPS_MAX);

    while(Duet.has_quit==false)
    {
        Duet.update();
        Duet.draw();
    }
    Duet.quit();
    return 0;
}
