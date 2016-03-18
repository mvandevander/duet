#include "Vsync.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>

#if defined(_WIN32)
    #include <GL/wglew.h>
#endif

#include <string>


bool GPU_HAS_VSYNC_SUPPORT = true;

//When (if?) the game gets ported to other platforms, this file will need to be updated to
//support the platform specific VSync implementation.

//function pointer typdefs

/*typedef void (GLAPIENTRY * PFNWGLEXTSWAPCONTROLPROC) (int);
//GLAPI void GLAPIENTRY glAccum (GLenum op, GLfloat value);
typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);

//declare functions
PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = NULL;
PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT = NULL;
*/

//wglGetSwapIntervalEXT

//init VSync func
void InitVSync()
{
   //get extensions of graphics card

   char* extensions = (char*)glGetString(GL_EXTENSIONS);

   //is WGL_EXT_swap_control in the string? VSync switch possible?
    if (strstr(extensions,"WGL_EXT_swap_control"))
   {
       fprintf(stdout, "Vertical Synchronization was enabled successfully! :)\n");
       //VSync is possible, so it can be enabled as usual
       GPU_HAS_VSYNC_SUPPORT = true;
   }
   else
   {
       //The GPU doesn't support Vertical Sync, how is this POSSIBLE???
       GPU_HAS_VSYNC_SUPPORT = false; //we set Vsync to be safely disabled, so that all commands will just be ignored.
       fprintf(stderr, "Vertical Syncronization is not supported on your graphics card, it has been disabled%s/n",SDL_GetError());
   }

}

bool VSyncEnabled()
{
   //tests to see if vsync is supported and returns whether it is enabled if it is supported
   if(GPU_HAS_VSYNC_SUPPORT) return (wglGetSwapIntervalEXT() != 0);
   else return false;
   //sreturn true;
}

void SetVSyncState(bool enable)
{
    if(GPU_HAS_VSYNC_SUPPORT) //We must test to see if the main bool allows for VSync functionality.
    {
        if (enable)
           wglSwapIntervalEXT(true); //set interval to 1
        else
           wglSwapIntervalEXT(false); //disable
    }
}
