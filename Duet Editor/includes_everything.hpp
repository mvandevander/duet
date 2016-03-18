//-----------------------------------------------------includes_everything.hpp-----------------------------------------
//
//THIS FILE INCLUDES ALL OF THE OTHER HEADER FILES AND ALSO DECLARES OUR GLOBAL VARIABLES AND NAMESPACES
//
//------------------------------------------------------------------------------------------------------

#ifndef INCLUDES_EVERYTHING_HPP_INCLUDED
#define INCLUDES_EVERYTHING_HPP_INCLUDED

#include "version.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#if defined(_WIN32)
    #include <GL/wglew.h>
#endif

#include <SDL/SDL_image.h>
#include "SDL/SDL_mixer.h"

#include <dirent.h>
#include <string>
#include <sstream>

#include "trig_functions_degrees.hpp"
#include "random_number_generators.hpp"

#include <vector>
#include <fstream>


#include "Timer.hpp"
#include "SDLTypingHandler.hpp"
#include "Level.hpp"

#include "BaseUIElement.hpp"
#include "UI_TextField.hpp"
#include "UI_Button.hpp"
#include "UI_TreeEditor.hpp"
#include "UI_Sliderf.hpp"
#include "UI_Slideri.hpp"
#include "Status_List.hpp"
#include "GL_DrawingPrimitives.hpp"
#include "Font.hpp"
#include "GL_ShaderFuncs.hpp"
#include "random_number_generators.hpp"
#include "VSync.hpp"


#include "global_vars.hpp"


#include "NAMESPACE_main_editor.hpp"
#include "main_editor_functions.hpp"



#endif // INCLUDES_EVERYTHING_HPP_INCLUDED
