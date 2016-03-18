#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#define DELETE_ALL -1 //used as shorthand (longhand?) for the DeleteObjectInLevel function

#include <vector>
#include "Texture.hpp"
#include "Object.hpp"
#include "Static.hpp"
#include "Entity.hpp"
#include "Entity_GrassType.hpp"
#include "Entity_SignType.hpp"
#include "Entity_CollisionType.hpp"
#include "Entity_CollisionType_White_Button.hpp"
#include "Entity_CollisionType_Black_Button.hpp"
#include "Entity_CollisionType_Grey_Button.hpp"
#include "Entity_CollisionType_Green.hpp"
#include "Entity_CollisionType_Red.hpp"
#include "Entity_CollisionType_Puzzle_Barrier.hpp"
#include "Entity_CollisionType_Death.hpp"
#include "Entity_CollisionType_Death_Red.hpp"
#include "Entity_CollisionType_Death_Green.hpp"
#include "Entity_ButtonType_White.hpp"
#include "Entity_ButtonType_Black.hpp"
#include "Entity_ButtonType_Grey.hpp"
#include "Entity_ParticleSystemType.hpp"
#include "Entity_PlayerStartType_Red.hpp"
#include "Entity_PlayerStartType_Green.hpp"
#include "Entity_MarkerType.hpp"
#include "Entity_RespawnZoneType.hpp"
#include "Entity_DeathZoneType.hpp"
#include "Entity_DeathLaserType.hpp"
#include "Entity_ExitZoneType.hpp"
#include "CollisionPoint.hpp"
#include "CollisionSeries.hpp"



using namespace std;


struct Layer {
    float parallax_modifier; // affects how much parallax the layer has
    vector <Object*> statics; // a vector which contains all statics in the layer;
    vector <Object*> entities;
};

struct Level {
	int width; //these two variables
	int height; //may or may not be necessary
	float background_r, background_g, background_b;
	Layer layers[8]; //layers to contain all statics and entities in the level (1 pre-foreground layer, 2 foreground layers (in front and behind players), and 5 background layers)
	vector <Texture> textures; //a vector to contain all textures used by statics/entities in the level
	vector <CollisionSeries> collision_series;
};

enum width_or_height{
    Width = 1,
    width = 1,
    w = 1,
    x = 1,
    Height = 2,
    height = 2,
    h = 2,
    y = 2,
};



void DeleteObjectinLevel(vector<Object*> &level_objects, int index);
bool LoadLeveltoStructure(string load_filename, Level &level_structure);
bool SaveLeveltoFile(string save_filename, Level &level_structure);
bool LoadTextureFileIntoLevel(std::string filename, Level &level_structure);
int FindStaticScalePercentage(width_or_height type,Static &input_static, Level &level_structure);


#endif // LEVEL_HPP_INCLUDED
