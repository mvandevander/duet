#include "Status_List.hpp"
#include <vector>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include "Font.hpp"

void Status_List::add_message(string message)
{
    current_messages.push_back(message);
    message_removal_timer.Start(); //reset the timer
}

void Status_List::update()
{
    if((current_messages.size()&&message_removal_timer.GetTicks()>1500)||current_messages.size()>10) //2 seconds passed since last message posted
    {
        current_messages.erase(current_messages.begin());
        message_removal_timer.Start();
    }
    else if(current_messages.size()==0)
    {
        message_removal_timer.Stop();
    }

}

void Status_List::draw(struct Font* font, int x, int y, SDL_Color text_color, SDL_Color outline_color)
{
        if(current_messages.size()){
            for(unsigned int i = 0; i<current_messages.size(); i++)
            {
                //int height = font->h;
                //if(i==0) glColor4f(1.0,1.0,1.0,(1.0-);
                //else glColor4f(1.0,1.0,1.0,1.0);
                if (i==0) Text(font,x,y+(i*font->h),current_messages[i],text_color, 1.0-((float)message_removal_timer.GetTicks()/1500));
                else Text(font,x,y+(i*font->h),current_messages[i],text_color);
            }
        }
        glColor4f(1.0,1.0,1.0,1.0);
}

