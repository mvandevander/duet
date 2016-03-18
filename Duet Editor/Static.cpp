#include "Static.hpp"
#include <fstream>
#include <string>

using namespace std;

Static::Static(int x, int y, int w, int h, int rotation, int texture_index)
{
    this->type = "Static";
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->r_mult = 1;
    this->g_mult = 1;
    this->b_mult = 1;
    this->a_mult = 1;
    this->r_add = 0;
    this->g_add = 0;
    this->b_add = 0;
    this->a_add = 0;
    this->r_blend = 0;
    this->g_blend = 0;
    this->b_blend = 0;
    this->a_blend = 0;
    this->hue = 0;
    this->saturation = 1.0;
    this->lightness = 0;
    this->brightness = 1.0;
    this->contrast = 1.0;
    this->is_using_additive_tinting = false;
    this->is_using_colorize_hue = false;

    this->rotation = rotation;
    this->parallax_offset = 1.0;
    this->u = 0;
    this->v = 0;
    this->u2 = 1;
    this->v2 = 1;
    this->is_flipped_horizontally = false;
    this->texture_index = texture_index;
    this->bounds.x = this->x-(this->w/2);
    this->bounds.y = this->y-(this->h/2);
    this->bounds.w = this->w;
    this->bounds.h = this->h;
}

bool Static::serialize(fstream& file)
{
    if(!file.is_open()) return false;
    int static_version = 4;
    file.write((char*) &static_version, sizeof(static_version));
    file.write((char*) &x, sizeof(x));
    file.write((char*) &y, sizeof(y));
    file.write((char*) &w, sizeof(w));
    file.write((char*) &h, sizeof(h));

    file.write((char*) &u, sizeof(u));
    file.write((char*) &v, sizeof(v));
    file.write((char*) &u2, sizeof(u2));
    file.write((char*) &v2, sizeof(v2));

    file.write((char*) &r_mult, sizeof(r_mult));
    file.write((char*) &g_mult, sizeof(g_mult));
    file.write((char*) &b_mult, sizeof(b_mult));
    file.write((char*) &a_mult, sizeof(a_mult));

    file.write((char*) &r_add, sizeof(r_add));
    file.write((char*) &g_add, sizeof(g_add));
    file.write((char*) &b_add, sizeof(b_add));
    file.write((char*) &a_add, sizeof(a_add));

    file.write((char*) &r_blend, sizeof(r_blend));
    file.write((char*) &g_blend, sizeof(g_blend));
    file.write((char*) &b_blend, sizeof(b_blend));
    file.write((char*) &a_blend, sizeof(a_blend));

    file.write((char*) &hue, sizeof(hue));
    file.write((char*) &saturation, sizeof(saturation));
    file.write((char*) &lightness, sizeof(lightness));

    file.write((char*) &brightness, sizeof(brightness));
    file.write((char*) &contrast, sizeof(contrast));

    file.write((char*) &is_using_additive_tinting, sizeof(is_using_additive_tinting));
    file.write((char*) &is_using_colorize_hue, sizeof(is_using_colorize_hue));
    file.write((char*) &is_flipped_horizontally, sizeof(is_flipped_horizontally));


    file.write((char*) &parallax_offset, sizeof(parallax_offset));
    file.write((char*) &rotation, sizeof(rotation));
    file.write((char*) &texture_index, sizeof(texture_index));
    return true;
}


bool Static::unserialize(ifstream& file)
{
    if(!file.is_open()) return false;
    int static_version;
    file.read((char*) &static_version,sizeof(int));
    if(static_version==1)
    {
        file.read((char*) &x,sizeof(int));
        file.read((char*) &y,sizeof(int));
        file.read((char*) &w,sizeof(int));
        file.read((char*) &h,sizeof(int));
        file.read((char*) &rotation,sizeof(int));
        file.read((char*) &texture_index,sizeof(int));
        bounds.x = x-(w/2);
        bounds.y = y-(h/2);
        bounds.w = w;
        bounds.h = h;
        return true;
    }
    else if(static_version==2)
    {
        file.read((char*) &x,sizeof(int));
        file.read((char*) &y,sizeof(int));
        file.read((char*) &w,sizeof(int));
        file.read((char*) &h,sizeof(int));
        file.read((char*) &u, sizeof(u));
        file.read((char*) &v, sizeof(v));
        file.read((char*) &u2, sizeof(u2));
        file.read((char*) &v2, sizeof(v2));
        file.read((char*) &is_flipped_horizontally, sizeof(is_flipped_horizontally));
        file.read((char*) &parallax_offset, sizeof(parallax_offset));
        file.read((char*) &rotation,sizeof(int));
        file.read((char*) &texture_index,sizeof(int));
        bounds.x = x-(w/2);
        bounds.y = y-(h/2);
        bounds.w = w;
        bounds.h = h;
        return true;
    }
    else if(static_version==3)
    {
        file.read((char*) &x,sizeof(int));
        file.read((char*) &y,sizeof(int));
        file.read((char*) &w,sizeof(int));
        file.read((char*) &h,sizeof(int));

        file.read((char*) &u, sizeof(u));
        file.read((char*) &v, sizeof(v));
        file.read((char*) &u2, sizeof(u2));
        file.read((char*) &v2, sizeof(v2));

        file.read((char*) &r_mult, sizeof(r_mult));
        file.read((char*) &g_mult, sizeof(g_mult));
        file.read((char*) &b_mult, sizeof(b_mult));
        file.read((char*) &a_mult, sizeof(a_mult));

        file.read((char*) &r_add, sizeof(r_add));
        file.read((char*) &g_add, sizeof(g_add));
        file.read((char*) &b_add, sizeof(b_add));
        file.read((char*) &a_add, sizeof(a_add));

        file.read((char*) &r_blend, sizeof(r_blend));
        file.read((char*) &g_blend, sizeof(g_blend));
        file.read((char*) &b_blend, sizeof(b_blend));
        file.read((char*) &a_blend, sizeof(a_blend));

        file.read((char*) &hue, sizeof(hue));
        file.read((char*) &saturation, sizeof(saturation));
        file.read((char*) &lightness, sizeof(lightness));

        file.read((char*) &brightness, sizeof(brightness));
        file.read((char*) &contrast, sizeof(contrast));

        file.read((char*) &is_using_additive_tinting, sizeof(is_using_additive_tinting));
        file.read((char*) &is_flipped_horizontally, sizeof(is_flipped_horizontally));

        file.read((char*) &parallax_offset, sizeof(parallax_offset));
        file.read((char*) &rotation,sizeof(int));
        file.read((char*) &texture_index,sizeof(int));

        bounds.x = x-(w/2);
        bounds.y = y-(h/2);
        bounds.w = w;
        bounds.h = h;
        return true;
    }
    else if(static_version==4)
    {
        file.read((char*) &x,sizeof(int));
        file.read((char*) &y,sizeof(int));
        file.read((char*) &w,sizeof(int));
        file.read((char*) &h,sizeof(int));

        file.read((char*) &u, sizeof(u));
        file.read((char*) &v, sizeof(v));
        file.read((char*) &u2, sizeof(u2));
        file.read((char*) &v2, sizeof(v2));

        file.read((char*) &r_mult, sizeof(r_mult));
        file.read((char*) &g_mult, sizeof(g_mult));
        file.read((char*) &b_mult, sizeof(b_mult));
        file.read((char*) &a_mult, sizeof(a_mult));

        file.read((char*) &r_add, sizeof(r_add));
        file.read((char*) &g_add, sizeof(g_add));
        file.read((char*) &b_add, sizeof(b_add));
        file.read((char*) &a_add, sizeof(a_add));

        file.read((char*) &r_blend, sizeof(r_blend));
        file.read((char*) &g_blend, sizeof(g_blend));
        file.read((char*) &b_blend, sizeof(b_blend));
        file.read((char*) &a_blend, sizeof(a_blend));

        file.read((char*) &hue, sizeof(hue));
        file.read((char*) &saturation, sizeof(saturation));
        file.read((char*) &lightness, sizeof(lightness));

        file.read((char*) &brightness, sizeof(brightness));
        file.read((char*) &contrast, sizeof(contrast));

        file.read((char*) &is_using_additive_tinting, sizeof(is_using_additive_tinting));
        file.read((char*) &is_using_colorize_hue, sizeof(is_using_colorize_hue));
        file.read((char*) &is_flipped_horizontally, sizeof(is_flipped_horizontally));

        file.read((char*) &parallax_offset, sizeof(parallax_offset));
        file.read((char*) &rotation,sizeof(int));
        file.read((char*) &texture_index,sizeof(int));

        bounds.x = x-(w/2);
        bounds.y = y-(h/2);
        bounds.w = w;
        bounds.h = h;
        return true;
    }
    else return false;
}

