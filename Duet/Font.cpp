//Again, the following code is heavily based on code written by Ryan Pridgeon, thanks go to him for fast TTF rendering.

/*All code copyright to Ryan Pridgeon, 2009
If you wish to use this code, you must include these credits
in your code, and give credit in your application/documentation
Thankyou
*/

#include "Font.hpp"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <SDL/SDL.h>
#include <string>
#include <GL/glew.h>

#include "xFile.hpp"

void SerializeFont(std::string filename, Font* font)
{
    xFile file;
    file.open_to_write(filename);
    //we save a title of the file ".font", we do it as a char array so that we can read it back in properly later
    file.write('.');
    file.write('f');
    file.write('o');
    file.write('n');
    file.write('t');


    //Then we write the version of the file format, so we can check upon load if we know how to load that version
    int font_version = 1;
    file.write(font_version);

    //now we want to write some global font info
    file.write(font->h);
    file.write(font->face_tex_w);
    file.write(font->face_tex_h);

    //write out the font texture itself (HOPE IT WORKS!!)
    int xpos, ypos;
        for(ypos=0;ypos<font->face_tex_h;ypos++){
            for(xpos=0;xpos<font->face_tex_w;xpos++){
                int buffer_position = 2*((xpos) + ((ypos)* font->face_tex_w));
                file.write(font->face_texture_data[buffer_position+1]);
            }
        }

    unsigned int num_glyphs = 128; //will need to change this if we go unicode...so will need to change this

    //now we write out all of the metadata for each glyph
    for(unsigned int i = 0; i<num_glyphs; i++)
    {
        Font::glyph_info temp_glyph = font->glyph_data[i];
        file.write(temp_glyph.width);
        file.write(temp_glyph.rows);
        file.write(temp_glyph.w);
        file.write(temp_glyph.h);
        file.write(temp_glyph.left);
        file.write(temp_glyph.top);
        file.write(temp_glyph.u1);
        file.write(temp_glyph.v1);
        file.write(temp_glyph.u2);
        file.write(temp_glyph.v2);
        file.write(temp_glyph.xadvance);
    }


    file.close();
}

struct Font* UnserializeFont(std::string filename)
{
    xFile file;

    //first we save a title of the file ".font"
    struct Font* temp = (struct Font*)malloc(sizeof(struct Font)); //just a temporary loading font which we will return to the user

    file.open_to_read(filename);

    char title[6];
    file.read(title[0]);
    file.read(title[1]);
    file.read(title[2]);
    file.read(title[3]);
    file.read(title[4]);

    title[5] = '\0';
    std::string title_string;
    title_string.assign(title);

    if(title_string!=".font")
    {
        //ABORT, it's not a font file!
        fprintf(stderr,"Font loading error...Doesn't seem to be a .font file!\n");
        file.close();
        return temp;
    }

    //check to see what version the file format is, so we can load older .font files
    int font_version;
    file.read(font_version);


    if(font_version == 1)
    {
        //we read in our global font info
        file.read(temp->h);
        file.read(temp->face_tex_w);
        file.read(temp->face_tex_h);

        temp->face_texture_data = (GLubyte*)calloc(sizeof(GLubyte)*2*(temp->face_tex_w)*(temp->face_tex_h) ,1);

        int xpos, ypos;
        for(ypos=0;ypos<temp->face_tex_h;ypos++){
            for(xpos=0;xpos<temp->face_tex_w;xpos++){
                int buffer_position = 2*((xpos) + ((ypos)* temp->face_tex_w));
                temp->face_texture_data[buffer_position] = 255;
                file.read(temp->face_texture_data[buffer_position+1]);
            }
        }
        unsigned int num_glyphs = 128; //will need to change this if we go unicode...so will need to change this

        //now we write out all of the metadata for each glyph
        for(unsigned int i = 0; i<num_glyphs; i++)
        {
            Font::glyph_info &temp_glyph = temp->glyph_data[i];
            file.read(temp_glyph.width);
            file.read(temp_glyph.rows);
            file.read(temp_glyph.w);
            file.read(temp_glyph.h);
            file.read(temp_glyph.left);
            file.read(temp_glyph.top);
            file.read(temp_glyph.u1);
            file.read(temp_glyph.v1);
            file.read(temp_glyph.u2);
            file.read(temp_glyph.v2);
            file.read(temp_glyph.xadvance);
        }
    }
    file.close();

    //Now we specify some parameters for our font face texture and then build the texture on the GPU
    //First we gotta bind the texture (tell GL we are working on it)
    glBindTexture(GL_TEXTURE_2D, temp->face_texture);

    //Then we set the parameters for how GL should display the texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    //Then finally, we build the texture by sending the data from our font_face buffer to the GL context
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->face_tex_w, temp->face_tex_h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, temp->face_texture_data);

    for(unsigned int i=0;i<128;i++)
    {
        //make display lists (for nicely formatted text)
        glPushMatrix();
        glNewList(temp->displaylist+i, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, temp->face_texture);
        glPushMatrix();
        glTranslatef(temp->glyph_data[i].left, (temp->h+3)-(temp->glyph_data[i].top), 0);

        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(temp->glyph_data[i].u1, temp->glyph_data[i].v1);                 glVertex2f(0, 0);
            glTexCoord2f(temp->glyph_data[i].u2, temp->glyph_data[i].v1);          glVertex2f(temp->glyph_data[i].width, 0);
            glTexCoord2f(temp->glyph_data[i].u1,temp->glyph_data[i].v2);           glVertex2f(0, temp->glyph_data[i].rows);
            glTexCoord2f(temp->glyph_data[i].u2, temp->glyph_data[i].v2);   glVertex2f(temp->glyph_data[i].width, temp->glyph_data[i].rows);

        glEnd();
        glPopMatrix();
        glTranslatef(temp->glyph_data[i].xadvance, 0, 0);
        glEndList();
        glPopMatrix();
    }


    return temp;
}


void Text(struct Font *temp, int x, int y, std::string whattosay, SDL_Color& dacolor, float alpha)
{
    glLoadIdentity();
    glTranslatef((float)x,(float)y,0.0);
    glColor4f((float)dacolor.r/255.0f,(float)dacolor.g/255.0f,(float)dacolor.b/255.0f,alpha);
    glEnable(GL_TEXTURE_2D);
    PrintFont(temp,whattosay);

}

void Text_Outlined(struct Font* temp, int x, int y, std::string whattosay, SDL_Color& text_color, SDL_Color& outline_color, float alpha)
{
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glTranslatef((float)x,(float)y,0.0);
    glListBase(temp->displaylist);
    const char* what_to_write = whattosay.c_str();
    glPushMatrix();
    glTranslatef(3.0,3.0,0.0);
    glColor4f(outline_color.r/255,outline_color.g/255,outline_color.b/255,alpha*0.75);
    glCallLists(strlen(what_to_write), GL_UNSIGNED_BYTE, what_to_write);
    glPopMatrix();
    glColor4f(text_color.r/255,text_color.g/255,text_color.b/255,alpha);
    glCallLists(strlen(what_to_write), GL_UNSIGNED_BYTE, what_to_write);
}

void Text_Outlined_Current_Matrix(struct Font* temp, int x, int y, std::string whattosay, SDL_Color text_color, SDL_Color outline_color, float alpha)
{
    glEnable(GL_TEXTURE_2D);
    glTranslatef((float)x,(float)y,0.0);
    glListBase(temp->displaylist);
    const char* what_to_write = whattosay.c_str();
    glPushMatrix();
    glTranslatef(3.0,3.0,0.0);
    glColor4f(outline_color.r/255,outline_color.g/255,outline_color.b/255,alpha*0.75);
    glCallLists(strlen(what_to_write), GL_UNSIGNED_BYTE, what_to_write);
    glPopMatrix();
    glColor4f(text_color.r/255,text_color.g/255,text_color.b/255,alpha);
    glCallLists(strlen(what_to_write), GL_UNSIGNED_BYTE, what_to_write);
}

int NextPower(int val){
    int x = 2;
    while(x<val){
        x *=2;
    }
    return x;
}

struct Font* LoadFont(std::string filepath, int height){

    struct Font* temp;
    FT_Library lib;
    FT_Face face;
    FT_Glyph glyph;
    FT_Bitmap* bitmap;
    FT_BitmapGlyph* bitglyph;
    unsigned int xpos, ypos, max_glyph_size, max_face_texture_width,
        texture_glyph_rows, max_face_texture_height,
        closest_p2_face_texture_width, closest_p2_face_texture_height,
        glyph_x_insert_count, glyph_y_insert_count;
    unsigned char i;
    int tex_offset_x = 0, tex_offset_y = 0;

    temp = (struct Font*)malloc(sizeof(struct Font));
    temp->h = height; //we store the loaded font height in the Font structure

    //In order to get the proper font size, we need to change the height to be smaller though...dunno why (padding?)
    height-=2;
    unsigned int u_height = height;
    //we need to generate the empty texture to store our glyphs
    //and the empty display lists to render them
    glGenTextures(1,&temp->face_texture);
    temp->displaylist = glGenLists(128);

    //initialize freetype and load our font
    FT_Init_FreeType(&lib);
    FT_New_Face(lib, filepath.c_str(), 0, &face);
    FT_Set_Char_Size(face, u_height << 6, u_height << 6, 96, 96);

    //since we want to use one texture for the whole font set, we need the
    //minumum size texture which can contain all the required glyphs

    //WE TRIED SOME OTHER WAYS OF DOING THIS, PROBABLY A FASTER METHOD
    //BUT CURRENTLY WE HAVE SETTLE UPON BRUTE FORCE!

            //first, we find the maximum size of a single glyph by querying Freetype
            //then we bitshift the max_advance_width returned by FreeType to get
            //the amount in whole pixels. (it returns 1/64 pixels)
            //max_glyph_size = face->max_advance_width >> 6;

            //max_glyph_size = temp->h+8;

    //we can just naively find the maximum glyph size for now, it's hacky and slow
    //but once the font is saved as a .font we don't have to do it, so whatever.

    max_glyph_size = 0;
    for(i=0;i<128;i++)
    {
        // We load up the current glyph and render it
        FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_RENDER);
        FT_Get_Glyph(face->glyph, &glyph);

        //Just creating some handy-dandy variables which are easier
        //to work with then nested pointers
        bitglyph = (FT_BitmapGlyph*)&glyph;
        bitmap = &((FT_BitmapGlyph)glyph)->bitmap;

        //Check to see if the current glyph is bigger than our max recorded
        if(bitmap->width>max_glyph_size) max_glyph_size = bitmap->width;
        if(bitmap->rows>max_glyph_size) max_glyph_size = bitmap->rows;
        FT_Done_Glyph(glyph);
    }




    //now we multiply the maximum glyph size by the amount of glyphs we want in a row of our texture
    //and calculate a few things about the required texture based on that
    max_face_texture_width = max_glyph_size*16;

    texture_glyph_rows = 8; //how many rows of glyphs do we want in our texture?
    max_face_texture_height = max_glyph_size*texture_glyph_rows;

    //Of course, we want to have a texture with dimensions that are a power of 2
    //even if it's bigger than we need, for speed and compatabilty
    closest_p2_face_texture_width = NextPower(max_face_texture_width);
    closest_p2_face_texture_height = NextPower(max_face_texture_height);

    //we store the texture size in the Font structure
    temp->face_tex_w = closest_p2_face_texture_width;
    temp->face_tex_h = closest_p2_face_texture_height;

    //we create a buffer big enough to store our font texture data
    GLubyte* font_face_buffer;
    font_face_buffer = (GLubyte*)calloc(sizeof(GLubyte)*2*(temp->face_tex_w)*(temp->face_tex_h) ,1);

    temp->face_texture_data = (GLubyte*)calloc(sizeof(GLubyte)*2*(temp->face_tex_w)*(temp->face_tex_h) ,1);

    glyph_x_insert_count = 0;
    glyph_y_insert_count = 0;

    //Now we are going to loop through all of the characters that we want to use and store them in
    //a buffer which we will build into a texture shortly.
    for(i=0;i<128;i++)
    {

        if(glyph_x_insert_count==16)
        {
            glyph_x_insert_count = 0;
            glyph_y_insert_count+=1;
            tex_offset_y = glyph_y_insert_count*max_glyph_size;
        }
        tex_offset_x = glyph_x_insert_count*max_glyph_size;
        glyph_x_insert_count+=1;
        // We load up the current glyph and render it
        FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_RENDER);
        FT_Get_Glyph(face->glyph, &glyph);

        //Just creating some handy-dandy variables which are easier
        //to work with then nested pointers
        bitglyph = (FT_BitmapGlyph*)&glyph;
        bitmap = &((FT_BitmapGlyph)glyph)->bitmap;

        //We copy a bunch of data from the glyph to our Font structure
        temp->glyph_data[i].width = bitmap->width;
        temp->glyph_data[i].rows = bitmap->rows;
        temp->glyph_data[i].left = (*bitglyph)->left;
        temp->glyph_data[i].top = (*bitglyph)->top;

        //we need to find the u and v coordinates for the glyph in the texture
        temp->glyph_data[i].u1 = (float)(tex_offset_x)/(float)temp->face_tex_w;
        temp->glyph_data[i].v1 = (float)(tex_offset_y)/(float)temp->face_tex_h;
        temp->glyph_data[i].u2 = (float)(tex_offset_x+temp->glyph_data[i].width)/(float)temp->face_tex_w;
        temp->glyph_data[i].v2 = (float)(tex_offset_y+temp->glyph_data[i].rows)/(float)temp->face_tex_h;

        //We also want to store the amount we move forward after finishing drawing this character
        //so we can use it later, when we actually draw it
        temp->glyph_data[i].xadvance = face->glyph->advance.x >> 6;

        for(ypos=0;ypos<bitmap->rows;ypos++){
            for(xpos=0;xpos<bitmap->width;xpos++){
                int buffer_position = 2*((xpos+tex_offset_x) + ((ypos+tex_offset_y)* temp->face_tex_w));
                font_face_buffer[buffer_position] = 255;
                font_face_buffer[buffer_position+1] = bitmap->buffer[xpos+ypos*bitmap->width];
            }
        }
         FT_Done_Glyph(glyph);

    }

    //Now we specify some parameters for our font face texture and then build the texture on the GPU
    //First we gotta bind the texture (tell GL we are working on it)
    glBindTexture(GL_TEXTURE_2D, temp->face_texture);

    //Then we set the parameters for how GL should display the texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    temp->face_texture_data = font_face_buffer;

    //Then finally, we build the texture by sending the data from our font_face buffer to the GL context
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->face_tex_w, temp->face_tex_h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, temp->face_texture_data);

    //Normally, here we would free the temporary buffer to free memory, but we want to save that data so we can save a .font file from it.
    //don't forget to delete our temporary buffer so we don't leak that memory
    //free(font_face_buffer);

    for(i=0;i<128;i++)
    {
        //make display lists (for nicely formatted text)
        glPushMatrix();
        glNewList(temp->displaylist+i, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, temp->face_texture);
        glPushMatrix();
        glTranslatef(temp->glyph_data[i].left, (temp->h+3)-(temp->glyph_data[i].top), 0);

        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(temp->glyph_data[i].u1, temp->glyph_data[i].v1);                 glVertex2f(0, 0);
            glTexCoord2f(temp->glyph_data[i].u2, temp->glyph_data[i].v1);          glVertex2f(temp->glyph_data[i].width, 0);
            glTexCoord2f(temp->glyph_data[i].u1,temp->glyph_data[i].v2);           glVertex2f(0, temp->glyph_data[i].rows);
            glTexCoord2f(temp->glyph_data[i].u2, temp->glyph_data[i].v2);   glVertex2f(temp->glyph_data[i].width, temp->glyph_data[i].rows);

        glEnd();
        glPopMatrix();
        glTranslatef(temp->glyph_data[i].xadvance, 0, 0);
        glEndList();
        glPopMatrix();
    }


    FT_Done_Face(face);
    FT_Done_FreeType(lib);

    return temp;
}

int PrintFont(struct Font* temp, std::string text){
    glListBase(temp->displaylist);
    const char* what_to_write = text.c_str();
    glCallLists(strlen(what_to_write), GL_UNSIGNED_BYTE, what_to_write);
    return 0;
}

void SizeText(struct Font* temp, std::string text, int *w, int*h){
    //glListBase(temp->displaylist);
    const char* what_to_write = text.c_str();
    int total_width = 0, total_height = 0;

    unsigned int string_length = strlen(what_to_write);
    for(unsigned int i = 0;i<string_length;i++)
    {
        int this_char_xadvance = temp->glyph_data[int(what_to_write[i])].xadvance;
        int this_char_rows = temp->glyph_data[int(what_to_write[i])].rows;
        total_width += this_char_xadvance;
        if(this_char_rows>total_height) total_height = this_char_rows;

    }

    *w = total_width;
    *h = total_height;

}




int DeleteFont(struct Font* temp){
    glDeleteTextures(1, &temp->face_texture);
    glDeleteLists(temp->displaylist, 128);
    free(temp);
    return 0;

}
