#include "includes_everything.hpp"


int main(int argc, char* args[])
{

    if(argc>1){
        fprintf(stdout,"Arguments:\n");
        for(int i=1;i<argc;i++)
        {
            string argument;
            argument.assign(args[i]);
            if(argument == "-windowed") FULLSCREEN_IS_ENABLED = false;
            if(argument == "-novsync") VSYNC_IS_ENABLED = false;
            //if(argument == "-nofpslimit") FPS_IS_CAPPED = false;
            fprintf(stdout, "Arg: %s\n",args[i]);
        }
    }
    if(InitializeMainEditor()==1) //it returns 1 upon an error
    {
        //we handle the error somehow
        return 1; //in this case, just naively return 1 and kill the program
    }

    while(quit == false)
    {

        UpdateMainEditor();
    }
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
