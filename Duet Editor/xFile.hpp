#ifndef XFILE_HPP_INCLUDED
#define XFILE_HPP_INCLUDED

/**------------------------------xFILE.hpp------------------------------------//
//This file provides xFile: a handy-dandy structure for interacting with files//
//in a cross-platform friendly way, using SDL of course!----------------------//
//---------------------------------------------------------------------------**/

#include <SDL/SDL.h>
#include <string>
struct xFile
{
    //Internal data
    SDL_RWops *internal_file;
    bool file_is_open, we_are_reading, we_are_writing;


    //opens a file for writing
    int open_to_write(std::string filename)
    {
        if(!file_is_open)
        {
            internal_file = SDL_RWFromFile(filename.c_str(),"wb");
            file_is_open = true;
            we_are_reading = false;
            we_are_writing = true;
            return 0;
        }
        fprintf(stdout, "xFile error: open_to_write called after file was already open. Maybe you forgot to close?\n");
        return 1;

    };

    //opens a file for writing
    int open_to_append(std::string filename)
    {
        if(!file_is_open)
        {
            internal_file = SDL_RWFromFile(filename.c_str(),"ab");
            file_is_open = true;
            we_are_reading = false;
            we_are_writing = true;
            return 0;
        }
        fprintf(stdout, "xFile error: open_to_append called after file was already open. Maybe you forgot to close?\n");
        return 1;

    };

    //opens a file for reading
    int open_to_read(std::string filename)
    {
        if(!file_is_open)
        {
            internal_file = SDL_RWFromFile(filename.c_str(),"rb");
            file_is_open = true;
            we_are_reading = true;
            we_are_writing = false;
            return 0;
        }
        fprintf(stdout, "xFile error: open_to_read called after file was already open. Maybe you forgot to close?\n");
        return 1;

    };

    //closes the currently open file, if open
    void close()
    {
        if(file_is_open)
        {
            SDL_RWclose(internal_file);
            file_is_open = false;
        }
        else fprintf(stdout, "xFile error: close called when no file was open.\n");

    };

    //Simply writes date of the type specified to file and returns 0 if successful
    template <typename type> int write(type data)
    {
        //we need to check to see if the structure has access to an open file before we do anything
        if(file_is_open and we_are_writing)
        {
            SDL_RWwrite(internal_file, &data, sizeof(type), 1);
            return 0;
        }
        else if(!file_is_open)
        {
            fprintf(stdout, "xFile error: write called with no file open.\n");
        }
        else
        {
            fprintf(stdout, "xFile error: Can only write under an open_to_write context\n");
        }

        return 1;

    };




    //Reads data from the file and returns the specified type
    template <typename type> int read(type &data)
    {
        //we need to check to see if the structure has access to an open file before we do anything
        if(file_is_open and we_are_reading)
        {
            SDL_RWread(internal_file, &data, sizeof(type), 1);
            return 0;
        }
        else if(!file_is_open)
        {
            fprintf(stdout, "xFile error: read called with no file open.\n");
        }
        else
        {
            fprintf(stdout, "xFile error: Can only read under a open_to_read context.\n");
        }

        return 1;
    };

    //We need a specialized write function for dealing with the string type
    //SPECIAL NOTE: These do not currently work with unicode, as they only encode the string
    //as single chars, and assume that each character will be one byte width.
    int write (std::string data)
    {
        //we need to check to see if the structure has access to an open file before we do anything
        if(file_is_open and we_are_writing)
        {
            //first we find the length of the string the user passed us, and write that to the file
            size_t length = data.size();
            SDL_RWwrite(internal_file, &length, sizeof(size_t), 1);

            //then we write the actual string data to the file as a char array
            SDL_RWwrite(internal_file, data.c_str(), sizeof(char), length);

            return 0;
        }
        else if(!file_is_open)
        {
            fprintf(stdout, "xFile error: write called with no file open.\n");
        }
        else
        {
            fprintf(stdout, "xFile error: Can only write under an open_to_write context\n");
        }
        return 1;
    };

    //We need a specialized read function for dealing with the string type
    //SPECIAL NOTE: These do not currently work with unicode, as they only encode the string
    //as single chars, and assume that each character will be one byte width.
    int read (std::string &data)
    {
        //we need to check to see if the structure has access to an open file before we do anything
        if(file_is_open and we_are_reading)
        {
            //first we need to read the length of the string from the file
            //(easier than null terminating and searching one char at a time
            size_t length;
            SDL_RWread(internal_file, &length, sizeof(size_t), 1);

            //then we create a dummy char array to store all of our characters
            char char_string[length];
            SDL_RWread(internal_file, &char_string,sizeof(char),length);

            //finally we assign the dummy array to the string the user passed to the function
            data.assign(char_string);

            return 0;
        }
        else if(!file_is_open)
        {
            fprintf(stdout, "xFile error: read called with no file open.\n");
        }
        else
        {
            fprintf(stdout, "xFile error: Can only read under a open_to_read context.\n");
        }
        return 1;
    }

};



#endif // XFILE_HPP_INCLUDED
