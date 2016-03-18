#include "Level.hpp"


void DeleteObjectinLevel(vector<Object*> &level_objects, int index)
{
    if(index==DELETE_ALL){
        for(unsigned int i = 0; i<level_objects.size(); i++)
        {
            delete level_objects[i]; //the data stored at the pointer must be deleted
        }
        level_objects.clear();
    }
    else if(index!=DELETE_ALL){
        delete level_objects[index]; //the data stored at the pointer must be deleted
        level_objects.erase(level_objects.begin() + index);
    }
}



bool LoadLeveltoStructure(string load_filename, Level &level_structure)
{
    ifstream load_file(load_filename.c_str(),ios::in|ios::binary);
    if(!load_file.is_open()){ //something's wrong, and the file isn't real, so don't load it
        fprintf(stderr, "Unable to load level, does not exist: %s \n", load_filename.c_str());
        load_file.close();
        return false;
    }
    else { // we load the level file
        int save_file_version;
        load_file.read((char*) &save_file_version, sizeof(int));
        if(save_file_version==1){
            DeleteObjectinLevel(level_structure.layers[0].statics,DELETE_ALL); //clear the level_structure's statics
            DeleteObjectinLevel(level_structure.layers[2].statics,DELETE_ALL); //clear the level_structure's statics
            DeleteObjectinLevel(level_structure.layers[4].statics,DELETE_ALL); //clear the level_structure's statics
            for(int i = 0; i<=7; i++) {DeleteObjectinLevel(level_structure.layers[i].entities,DELETE_ALL);} //clear the level_structure's statics}
            for(int i = 2; i<=7; i++){ // we go through all the layers in the level
                if(i==4) i++;
                DeleteObjectinLevel(level_structure.layers[i].statics,DELETE_ALL); //clear the level_structure's statics
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    level_structure.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int a = 0; a<num_statics; a++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)) return false;
                        level_structure.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            level_structure.textures.clear(); //clear the level_structure's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    level_structure.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of level_structure's textures
                }
            }
            level_structure.collision_series.clear();
        }
        if(save_file_version==2){
            DeleteObjectinLevel(level_structure.layers[0].statics,DELETE_ALL); //clear the level_structure's statics
            DeleteObjectinLevel(level_structure.layers[1].statics,DELETE_ALL); //clear the level_structure's statics
            DeleteObjectinLevel(level_structure.layers[4].statics,DELETE_ALL); //clear the level_structure's statics
            for(int i = 0; i<=7; i++) {DeleteObjectinLevel(level_structure.layers[i].entities,DELETE_ALL);} //clear the level_structure's statics}
            for(int i = 2; i<=7; i++){ // we go through all the layers in the level
                if(i==4) i++;
                DeleteObjectinLevel(level_structure.layers[i].statics,DELETE_ALL); //clear the level_structure's statics
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    level_structure.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int a = 0; a<num_statics; a++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)) return false;
                        level_structure.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            level_structure.textures.clear(); //clear the level_structure's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    level_structure.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of level_structure's textures
                }
            }
            size_t num_collision_series;
            level_structure.collision_series.clear();
            load_file.read((char*) &num_collision_series, sizeof(num_collision_series));
            if(num_collision_series>0){
                level_structure.collision_series.reserve(num_collision_series);
                for(unsigned int i = 0; i<num_collision_series; i++)
                {
                    CollisionSeries temp_series(0,0);
                    if(!temp_series.unserialize(load_file)) return false;
                    level_structure.collision_series.push_back(temp_series);
                }
            }
        }
        if(save_file_version==3){
            DeleteObjectinLevel(level_structure.layers[0].statics,DELETE_ALL); //clear the level_structure's statics
            DeleteObjectinLevel(level_structure.layers[0].entities,DELETE_ALL); //clear the level_structure's entities
            DeleteObjectinLevel(level_structure.layers[1].statics,DELETE_ALL); //clear the level_structure's statics
            DeleteObjectinLevel(level_structure.layers[1].entities,DELETE_ALL); //clear the level_structure's entities
            DeleteObjectinLevel(level_structure.layers[4].statics,DELETE_ALL); //clear the level_structure's statics
            DeleteObjectinLevel(level_structure.layers[4].entities,DELETE_ALL); //clear the level_structure's entities
            for(int i = 2; i<=7; i++){ // we go through all the layers in the level
                if(i==4) i++;
                DeleteObjectinLevel(level_structure.layers[i].statics,DELETE_ALL); //clear the level_structure's statics
                DeleteObjectinLevel(level_structure.layers[i].entities,DELETE_ALL); //clear the level_structure's entities
                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    level_structure.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        char type;
                        load_file.read((char*) &type, sizeof(char)); //find out what type of entity it is
                        if(type=='g'){
                            Entity* temp_entity = new Entity_GrassType(0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='s'){
                            Entity* temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    level_structure.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)) return false;
                        level_structure.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            level_structure.textures.clear(); //clear the level_structure's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    level_structure.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of level_structure's textures
                }
            }
            size_t num_collision_series;
            level_structure.collision_series.clear();
            load_file.read((char*) &num_collision_series, sizeof(num_collision_series));
            if(num_collision_series>0){
                level_structure.collision_series.reserve(num_collision_series);
                for(unsigned int i = 0; i<num_collision_series; i++)
                {
                    CollisionSeries temp_series(0,0);
                    if(!temp_series.unserialize(load_file)) return false;
                    level_structure.collision_series.push_back(temp_series);
                }
            }
        }
        if(save_file_version==4){
                DeleteObjectinLevel(level_structure.layers[0].statics,DELETE_ALL); //clear the level_structure's statics
                DeleteObjectinLevel(level_structure.layers[0].entities,DELETE_ALL); //clear the level_structure's entities
                DeleteObjectinLevel(level_structure.layers[4].statics,DELETE_ALL); //clear the level_structure's statics
                DeleteObjectinLevel(level_structure.layers[4].entities,DELETE_ALL); //clear the level_structure's entities
            for(int i = 1; i<=7; i++){ // we go through all the layers in the level
                if(i==4) i++;
                DeleteObjectinLevel(level_structure.layers[i].statics,DELETE_ALL); //clear the level_structure's statics
                DeleteObjectinLevel(level_structure.layers[i].entities,DELETE_ALL); //clear the level_structure's entities

                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    level_structure.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        char type;
                        load_file.read((char*) &type, sizeof(char)); //find out what type of entity it is
                        if(type=='c'){
                            Entity* temp_entity = new Entity_CollisionType(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='g'){
                            Entity* temp_entity = new Entity_GrassType(0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='s'){
                            Entity* temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    level_structure.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)) return false;
                        level_structure.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            level_structure.textures.clear(); //clear the level_structure's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    level_structure.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of level_structure's textures
                }
            }
            size_t num_collision_series;
            level_structure.collision_series.clear();
            load_file.read((char*) &num_collision_series, sizeof(num_collision_series));
            if(num_collision_series>0){
                level_structure.collision_series.reserve(num_collision_series);
                for(unsigned int i = 0; i<num_collision_series; i++)
                {
                    CollisionSeries temp_series(0,0);
                    if(!temp_series.unserialize(load_file)) return false;
                    level_structure.collision_series.push_back(temp_series);
                }
            }
        }
            if(save_file_version==5){
            for(int i = 0; i<=7; i++){ // we go through all the layers in the level
                DeleteObjectinLevel(level_structure.layers[i].statics,DELETE_ALL); //clear the level_structure's statics
                DeleteObjectinLevel(level_structure.layers[i].entities,DELETE_ALL); //clear the level_structure's entities

                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    level_structure.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        char type;
                        load_file.read((char*) &type, sizeof(char)); //find out what type of entity it is
                        if(type=='c'){
                            Entity* temp_entity = new Entity_CollisionType(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='1'){
                            Entity* temp_entity = new Entity_CollisionType_White_Button(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='2'){
                            Entity* temp_entity = new Entity_CollisionType_Black_Button(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='3'){
                            Entity* temp_entity = new Entity_CollisionType_Grey_Button(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='4'){
                            Entity* temp_entity = new Entity_CollisionType_Red(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='5'){
                            Entity* temp_entity = new Entity_CollisionType_Green(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='6'){
                            Entity* temp_entity = new Entity_ButtonType_White(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='7'){
                            Entity* temp_entity = new Entity_ButtonType_Black(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='8'){
                            Entity* temp_entity = new Entity_ButtonType_Grey(0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='g'){
                            Entity* temp_entity = new Entity_GrassType(0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                        if(type=='s'){
                            Entity* temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                            if(!temp_entity->unserialize(load_file)) return false;
                            level_structure.layers[i].entities.push_back(temp_entity);
                        }
                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    level_structure.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)) return false;
                        level_structure.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            level_structure.textures.clear(); //clear the level_structure's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    level_structure.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of level_structure's textures
                }
            }
            size_t num_collision_series;
            level_structure.collision_series.clear();
            load_file.read((char*) &num_collision_series, sizeof(num_collision_series));
            if(num_collision_series>0){
                level_structure.collision_series.reserve(num_collision_series);
                for(unsigned int i = 0; i<num_collision_series; i++)
                {
                    CollisionSeries temp_series(0,0);
                    if(!temp_series.unserialize(load_file)) return false;
                    level_structure.collision_series.push_back(temp_series);
                }
            }
        }
        if(save_file_version==6){
            for(int i = 0; i<=7; i++){ // we go through all the layers in the level
                DeleteObjectinLevel(level_structure.layers[i].statics,DELETE_ALL); //clear the level_structure's statics
                DeleteObjectinLevel(level_structure.layers[i].entities,DELETE_ALL); //clear the level_structure's entities

                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    level_structure.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        string::size_type typename_length;
                        load_file.read((char*) &typename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                        vector <char> typename_buffer(typename_length); //create a vector as long as the read length in which we'll store the filename
                        load_file.read(&typename_buffer[0],typename_length); //read in the filename from the file and store it in the vector
                        string type;
                        type.assign(typename_buffer.begin(),typename_buffer.end()); //assign the vector to a string

                        //we create a NULL pointer to store our new entity
                        Entity* temp_entity = NULL;

                        //Now we interpret the type string so that we know what type of Entity should be loaded
                        //if it matches up to a known type, we allocate the memory for that type and assign the
                        //pointer to the new memory space

                        if(type=="CollisionType"){
                            temp_entity = new Entity_CollisionType(0,0,0,0);
                        }
                        else if(type=="CollisionType_White_Button"){
                            temp_entity = new Entity_CollisionType_White_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Black_Button"){
                            temp_entity = new Entity_CollisionType_Black_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Grey_Button"){
                            temp_entity = new Entity_CollisionType_Grey_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Red"){
                            temp_entity = new Entity_CollisionType_Red(0,0,0,0);
                        }
                        else if(type=="CollisionType_Green"){
                            temp_entity = new Entity_CollisionType_Green(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death"){
                            temp_entity = new Entity_CollisionType_Death(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death_Red"){
                            temp_entity = new Entity_CollisionType_Death_Red(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death_Green"){
                            temp_entity = new Entity_CollisionType_Death_Green(0,0,0,0);
                        }
                        else if(type=="CollisionType_Puzzle_Barrier"){
                            temp_entity = new Entity_CollisionType_Puzzle_Barrier(0,0,0,0);
                        }
                        else if(type=="ButtonType_White"){
                            temp_entity = new Entity_ButtonType_White(0,0,0,0);
                        }
                        else if(type=="ButtonType_Black"){
                            temp_entity = new Entity_ButtonType_Black(0,0,0,0);
                        }
                        else if(type=="ButtonType_Grey"){
                            temp_entity = new Entity_ButtonType_Grey(0,0,0,0);
                        }
                        else if(type=="PlayerStartType_Red"){
                            temp_entity = new Entity_PlayerStartType_Red(0,0,0,0);
                        }
                        else if(type=="PlayerStartType_Green"){
                            temp_entity = new Entity_PlayerStartType_Green(0,0,0,0);
                        }
                        else if(type=="MarkerType"){
                            temp_entity = new Entity_MarkerType(0,0,0,0);
                        }
                        else if(type=="RespawnZoneType"){
                            temp_entity = new Entity_RespawnZoneType(0,0,0,0);
                        }
                        else if(type=="DeathZoneType"){
                            temp_entity = new Entity_DeathZoneType(0,0,0,0);
                        }
                        else if(type=="DeathLaserType"){
                            temp_entity = new Entity_DeathLaserType(0,0,0,0);
                        }
                        else if(type=="ExitZoneType"){
                            temp_entity = new Entity_ExitZoneType(0,0,0,0);
                        }
                        else if(type=="GrassType"){
                            temp_entity = new Entity_GrassType(0,0,0,0,0);
                        }
                        else if(type=="SignType"){
                            temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                        }
                        else{ //we don't know about this entity type, so we can't handle loading the file
                            fprintf(stderr, "Unexpected Entity Type: %s \n", type.c_str()); //print what we can to the error log
                            load_file.close(); //clean up
                            return false;

                        }

                        //Now that we know the type of the entity, we can just load it
                            if(!temp_entity->unserialize(load_file)){
                                //if the unserialize function returns false, loading has failed somehow
                                load_file.close(); //don't forget to clean up
                                return false;
                            }
                            level_structure.layers[i].entities.push_back(temp_entity);

                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    level_structure.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)){
                            //something really wierd must've happened
                            load_file.close();
                             return false;
                        }
                        level_structure.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            level_structure.textures.clear(); //clear the level_structure's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    temp_filename.replace(temp_filename.end()-3,temp_filename.end(),"tga");
                    level_structure.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of level_structure's textures
                }
            }
        }
        if(save_file_version==7){
            //first we want to read in the background color
            load_file.read((char*) &level_structure.background_r, sizeof(level_structure.background_r));
            load_file.read((char*) &level_structure.background_g, sizeof(level_structure.background_r));
            load_file.read((char*) &level_structure.background_b, sizeof(level_structure.background_r));

            for(int i = 0; i<=7; i++){ // we go through all the layers in the level
                DeleteObjectinLevel(level_structure.layers[i].statics,DELETE_ALL); //clear the level_structure's statics
                DeleteObjectinLevel(level_structure.layers[i].entities,DELETE_ALL); //clear the level_structure's entities

                size_t num_entities;
                load_file.read((char*) &num_entities, sizeof(num_entities)); //read in the number of entities in the layer from the file
                if(num_entities>0){ //if there are any entities in the layer
                    level_structure.layers[i].entities.reserve(num_entities); //add empty vector space for the entities to be loaded
                    for(unsigned int a = 0; a<num_entities; a++) //go through all the entities in the file
                    {
                        string::size_type typename_length;
                        load_file.read((char*) &typename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                        vector <char> typename_buffer(typename_length); //create a vector as long as the read length in which we'll store the filename
                        load_file.read(&typename_buffer[0],typename_length); //read in the filename from the file and store it in the vector
                        string type;
                        type.assign(typename_buffer.begin(),typename_buffer.end()); //assign the vector to a string

                        //we create a NULL pointer to store our new entity
                        Entity* temp_entity = NULL;

                        //Now we interpret the type string so that we know what type of Entity should be loaded
                        //if it matches up to a known type, we allocate the memory for that type and assign the
                        //pointer to the new memory space

                        if(type=="CollisionType"){
                            temp_entity = new Entity_CollisionType(0,0,0,0);
                        }
                        else if(type=="CollisionType_White_Button"){
                            temp_entity = new Entity_CollisionType_White_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Black_Button"){
                            temp_entity = new Entity_CollisionType_Black_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Grey_Button"){
                            temp_entity = new Entity_CollisionType_Grey_Button(0,0,0,0);
                        }
                        else if(type=="CollisionType_Red"){
                            temp_entity = new Entity_CollisionType_Red(0,0,0,0);
                        }
                        else if(type=="CollisionType_Green"){
                            temp_entity = new Entity_CollisionType_Green(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death"){
                            temp_entity = new Entity_CollisionType_Death(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death_Red"){
                            temp_entity = new Entity_CollisionType_Death_Red(0,0,0,0);
                        }
                        else if(type=="CollisionType_Death_Green"){
                            temp_entity = new Entity_CollisionType_Death_Green(0,0,0,0);
                        }
                        else if(type=="CollisionType_Puzzle_Barrier"){
                            temp_entity = new Entity_CollisionType_Puzzle_Barrier(0,0,0,0);
                        }
                        else if(type=="ButtonType_White"){
                            temp_entity = new Entity_ButtonType_White(0,0,0,0);
                        }
                        else if(type=="ButtonType_Black"){
                            temp_entity = new Entity_ButtonType_Black(0,0,0,0);
                        }
                        else if(type=="ButtonType_Grey"){
                            temp_entity = new Entity_ButtonType_Grey(0,0,0,0);
                        }
                        else if(type=="PlayerStartType_Red"){
                            temp_entity = new Entity_PlayerStartType_Red(0,0,0,0);
                        }
                        else if(type=="PlayerStartType_Green"){
                            temp_entity = new Entity_PlayerStartType_Green(0,0,0,0);
                        }
                        else if(type=="MarkerType"){
                            temp_entity = new Entity_MarkerType(0,0,0,0);
                        }
                        else if(type=="RespawnZoneType"){
                            temp_entity = new Entity_RespawnZoneType(0,0,0,0);
                        }
                        else if(type=="DeathZoneType"){
                            temp_entity = new Entity_DeathZoneType(0,0,0,0);
                        }
                        else if(type=="DeathLaserType"){
                            temp_entity = new Entity_DeathLaserType(0,0,0,0);
                        }
                        else if(type=="ExitZoneType"){
                            temp_entity = new Entity_ExitZoneType(0,0,0,0);
                        }
                        else if(type=="GrassType"){
                            temp_entity = new Entity_GrassType(0,0,0,0,0);
                        }
                        else if(type=="SignType"){
                            temp_entity = new Entity_SignType(0,0,0,0,0,0,0);
                        }
                        else{ //we don't know about this entity type, so we can't handle loading the file
                            fprintf(stderr, "Unexpected Entity Type: %s \n", type.c_str()); //print what we can to the error log
                            load_file.close(); //clean up
                            return false;

                        }

                        //Now that we know the type of the entity, we can just load it
                            if(!temp_entity->unserialize(load_file)){
                                //if the unserialize function returns false, loading has failed somehow
                                load_file.close(); //don't forget to clean up
                                return false;
                            }
                            level_structure.layers[i].entities.push_back(temp_entity);

                    }
                }
                size_t num_statics;
                load_file.read((char*) &num_statics, sizeof(num_statics)); //read in the number of statics in the layer from the file
                if(num_statics>0){ //if there are any statics in the layer
                    level_structure.layers[i].statics.reserve(num_statics); //add empty vector space for the statics to be loaded
                    for(unsigned int b = 0; b<num_statics; b++) //go through all the statics in the file
                    {
                        Static* temp_static = new Static(0,0,0,0,0,0);
                        if(!temp_static->unserialize(load_file)){
                            //something really wierd must've happened
                            load_file.close();
                             return false;
                        }
                        level_structure.layers[i].statics.push_back(temp_static);
                    }
                }
            }
            size_t num_textures;
            level_structure.textures.clear(); //clear the level_structure's textures
            load_file.read((char*) &num_textures, sizeof(num_textures)); //read in the number of textures in the file
            if(num_textures>0){ //if there's more than one texture
                for(unsigned int i = 0; i<num_textures; i++) //go through all the textures in the file
                {
                    string::size_type filename_length;
                    load_file.read((char*) &filename_length, sizeof(string::size_type)); //read in the length of the filename for the texture
                    vector <char> filename_buffer(filename_length); //create a vector as long as the read length in which we'll store the filename
                    load_file.read(&filename_buffer[0],filename_length); //read in the filename from the file and store it in the vector
                    string temp_filename;
                    temp_filename.assign(filename_buffer.begin(),filename_buffer.end()); //assign the vector to a string
                    temp_filename.replace(temp_filename.end()-3,temp_filename.end(),"tga");
                    level_structure.textures.push_back(LoadTexture(temp_filename)); //load the filename specified and push the texture ont the end of level_structure's textures
                }
            }
        }
        load_file.close();
        return true;
    }
}

bool SaveLeveltoFile(string save_filename, Level &level_structure)
{
    fstream save_file(save_filename.c_str(),ios::out|ios::binary);
    if(!save_file.is_open()){//error loading file;
        fprintf(stderr, "Unable to save level, maybe directory is bad?: %s \n", save_filename.c_str());
        save_file.close();
        return false;
    }
    else{
        int save_file_version = 7;
        save_file.write((char*) &save_file_version, sizeof(int));
        //we save the background color
        save_file.write((char*) &level_structure.background_r, sizeof level_structure.background_r);
        save_file.write((char*) &level_structure.background_g, sizeof level_structure.background_g);
        save_file.write((char*) &level_structure.background_b, sizeof level_structure.background_b);
        for(int i = 0; i<=7; i++) // go through all the layers
        {
            size_t num_entities = level_structure.layers[i].entities.size(); //store the number of entities in a temporary variable
            save_file.write((char*) &num_entities, sizeof num_entities); //write the number of entities in the layer to the file
            for(unsigned int a = 0; a<num_entities; a++) //go through all the statics in the layer
            {
                if(!level_structure.layers[i].entities[a]->serialize(save_file)){
                    fprintf(stderr, "File isn't showing up as open in entity.serialize: %s \nThe offending entity is [%s] \n", save_filename.c_str(), (char*)a);
                    return false;
                }
            }
            size_t num_statics = level_structure.layers[i].statics.size(); //store the number of statics in a temporary variable
            save_file.write((char*) &num_statics, sizeof num_statics); //write the number of statics in the layer to the file
            for(unsigned int a = 0; a<num_statics; a++) //go through all the statics in the layer
            {
                if(!level_structure.layers[i].statics[a]->serialize(save_file)){
                    fprintf(stderr, "File isn't showing up as open in static.serialize: %s \nThe offending static is [%s] \n", save_filename.c_str(), (char*)a);
                    return false;
                }
            }
        }
        size_t num_textures = level_structure.textures.size(); //store the number of loaded textures in a temporary variable
        save_file.write((char*) &num_textures, sizeof num_textures); //write the number of textures to the file
        for(unsigned int i = 0; i <num_textures; i++) //go through all the textures
        {
            //For now, we want to replace the file extension of the texture with ".tga"
            std::string temp_filename = level_structure.textures[i].filename;
            temp_filename.replace(temp_filename.end()-3,temp_filename.end(),"tga");
            //rather than write the textures themselves, which would bloat up the saved file by storing the textures inside,
            //I will only write the filename of the textures, and just load them back up when I load a level

            string::size_type filename_length = temp_filename.size();
            save_file.write((char*) &filename_length, sizeof filename_length);
            save_file.write(temp_filename.data(), filename_length);
        }
        size_t num_collision_series = level_structure.collision_series.size();
        save_file.write((char*) &num_collision_series, sizeof num_collision_series);
        for(unsigned int i = 0; i <num_collision_series; i++)
        {
            level_structure.collision_series[i].serialize(save_file);
        }
        save_file.close();
        return true;

    }
}

bool LoadTextureFileIntoLevel(std::string filename, Level &level_structure)
{
    for(unsigned int i = 0; i<level_structure.textures.size(); i++){ //check to see if the texture is already loaded
        if(level_structure.textures[i].filename == filename){
            fprintf(stderr, "Unable to load texture: %s  already loaded. \n", filename.c_str());
            return false;
        }
    }
    ifstream load_file(filename.c_str(),ios::in);
    if(load_file.is_open()){ //check to see that the file is real
        load_file.close();
        level_structure.textures.push_back(LoadTexture(filename));
        return true;
    }
    else{ // the file is not real, so we don't load it
        fprintf(stderr, "Unable to load texture: %s does not exist or is not image \n", filename.c_str());
        load_file.close();
        return false;
    }
}


int FindStaticScalePercentage(width_or_height type,Static &input_static, Level &level_structure)
{
        if(type==Width) return (int)(((float)input_static.w/(float)level_structure.textures[input_static.texture_index].w)*100);
	else if(type==Height) return (int)(((float)input_static.h/(float)level_structure.textures[input_static.texture_index].h)*100);
	else return 0;
}
