#include "CollisionSeries.hpp"
#include "CollisionPoint.hpp"
#include "GL_DrawingPrimitives.hpp"
#include "trig_functions_degrees.hpp"

CollisionSeries::CollisionSeries(int first_point_x, int first_point_y)
{
    points.push_back(CollisionPoint(first_point_x,first_point_y));
}

void CollisionSeries::move(int new_x, int new_y)
{
    int relative_x_movement = new_x - points[0].x;
    int relative_y_movement = new_y - points[0].y;
    for(unsigned int i = 0; i<points.size();i++)
    {
        CollisionPoint &current_point = points[i];
        current_point.move(current_point.x+relative_x_movement,current_point.y+relative_y_movement);
    }
}

void CollisionSeries::draw(GLuint displaylist, int cam_x, int cam_y, int cam_width, int cam_height, float r, float g, float b, float a)
{
    for(unsigned int i = 0; i<points.size();i++)
    {
        CollisionPoint &temp = points[i];
        SDL_Rect screen;
        screen.x = cam_x;
        screen.y = cam_y;
        screen.w = cam_width;
        screen.h = cam_height;
        if(IsPointInRectangle(temp.x,temp.y,screen)){
        point_2d adjusted_point(temp.x-cam_x,temp.y-cam_y);
        DrawGLRectSolid(displaylist,(adjusted_point.x-4),(adjusted_point.y-4),8,8,r,g,b,a);
        if(i!=points.size()-1)DrawGLLine(adjusted_point.x,adjusted_point.y,(points[i+1].x)-cam_x,(points[i+1].y)-cam_y,r,g,b,a);
        }
    }
}

void CollisionSeries::addPoint(int new_point_x, int new_point_y)
{
    if(points.size()==1) points.push_back(CollisionPoint(new_point_x,new_point_y));
    int shortest_distance_squared = 0;
    int shortest_index = 0;
    for(unsigned int i = 0; i<points.size(); i++)
    {
        CollisionPoint &temp = points[i];
        int dist_x_from_pt = abs(temp.x-new_point_x);
        int dist_y_from_pt = abs(temp.y-new_point_y);
        int overall_dist_from_pt_squared = ((dist_x_from_pt*dist_x_from_pt)+(dist_y_from_pt*dist_y_from_pt));
        if(overall_dist_from_pt_squared<shortest_distance_squared||shortest_distance_squared==0)
        {
            shortest_distance_squared = overall_dist_from_pt_squared;
            shortest_index  = i;
        }
    }
    /*int first_point_index = shortest_index;
    int dist_from_next_pt;
    int dist_from_prev_pt;
    CollisionPoint &temp = points[first_point_index];
    int dist_x_from_pt = abs(temp.x-new_point_x);
    int dist_y_from_pt = abs(temp.y-new_point_y);
    dist_from_next_pt = ((dist_x_from_pt*dist_x_from_pt)+(dist_y_from_pt*dist_y_from_pt));*/


    if(shortest_distance_squared!=0) //if the distance is zero, it's because the point is already there
    {
        if((unsigned)shortest_index == points.size()-1) points.push_back(CollisionPoint(new_point_x,new_point_y));
        else points.insert(points.begin()+shortest_index,CollisionPoint(new_point_x,new_point_y));
    }
}

CollisionPoint* CollisionSeries::return_closest_point(int pt_x, int pt_y)
{
    int shortest_distance_squared = -1;
    int shortest_index = -1;
    for(unsigned int i = 0; i<points.size(); i++)
    {
        CollisionPoint &temp = points[i];
        int dist_x_from_pt = abs(temp.x-pt_x);
        int dist_y_from_pt = abs(temp.y-pt_y);
        int overall_dist_from_pt_squared = ((dist_x_from_pt*dist_x_from_pt)+(dist_y_from_pt*dist_y_from_pt));
        if(overall_dist_from_pt_squared<shortest_distance_squared||shortest_distance_squared==-1)
        {
            shortest_distance_squared = overall_dist_from_pt_squared;
            shortest_index  = i;
        }
    }
    if(shortest_index == -1) shortest_index = 0;
    return &points[shortest_index];
}

int CollisionSeries::return_closest_point_index(int pt_x, int pt_y)
{
    int shortest_distance_squared = -1;
    int shortest_index = -1;
    for(unsigned int i = 0; i<points.size(); i++)
    {
        CollisionPoint &temp = points[i];
        int dist_x_from_pt = abs(temp.x-pt_x);
        int dist_y_from_pt = abs(temp.y-pt_y);
        int overall_dist_from_pt_squared = ((dist_x_from_pt*dist_x_from_pt)+(dist_y_from_pt*dist_y_from_pt));
        if(overall_dist_from_pt_squared<shortest_distance_squared||shortest_distance_squared==-1)
        {
            shortest_distance_squared = overall_dist_from_pt_squared;
            shortest_index  = i;
        }
    }
    if(shortest_index == -1) shortest_index = 0;
    return shortest_index;
}

vec_2d CollisionSeries::return_closest_segment(int pt_x, int pt_y)
{
    point_2d checking_point(pt_x,pt_y);
    return return_closest_segment(checking_point);
}

vec_2d CollisionSeries::return_closest_segment(point_2d checking_point)
{
    vec_2d closest_vector;
    float shortest_distance = -1;
    for(unsigned int i = 0; i<points.size()-1; i++)
    {
        vec_2d this_segment(points[i].x,points[i].y,points[i+1].x,points[i+1].y);
        vec_2d shortest_vector = find_shortest_vector_from_point_to_line(checking_point,this_segment);
        if(shortest_vector.len<shortest_distance||shortest_distance==-1)
        {
            shortest_distance = shortest_vector.len;
            closest_vector = this_segment;
        }
    }
    return closest_vector;
}

vector<vec_2d> CollisionSeries::return_all_segments_directly_below(point_2d checking_point)
{
    vector<vec_2d> vectors_below_checking_point;
    for(unsigned int i = 0; i< points.size(); i++)
    {
        if(points[i].y>=checking_point.y){
            if(i!=(points.size()-1)&&points[i].x<checking_point.x&&points[i+1].x>checking_point.x)
            {
                vec_2d from_this_point_to_next(points[i].x,points[i].y,points[i+1].x,points[i+1].y);
                vectors_below_checking_point.push_back(from_this_point_to_next);
            }
            else if(i!=0&&points[i].x>checking_point.x&&points[i-1].x<checking_point.x)
            {
                vec_2d from_previous_point_to_this(points[i-1].x,points[i-1].y,points[i].x,points[i].y);
                vectors_below_checking_point.push_back(from_previous_point_to_this);
            }
        }
    }
    return vectors_below_checking_point;
}

vec_2d CollisionSeries::return_segment_directly_below(point_2d checking_point)
{
    vector<vec_2d> vectors_below_checking_point = return_all_segments_directly_below(checking_point);
    int closest_below_vector_y = -1;
    int closest_below_vector_index = -1;
    if(vectors_below_checking_point.size()==0) return vec_2d(0,0,0,0);
    for(unsigned int i = 0; i < vectors_below_checking_point.size(); i++)
    {
        int this_below_vector_y = vectors_below_checking_point[i].find_y_at(checking_point.x);
                if((this_below_vector_y>=checking_point.y&&this_below_vector_y<closest_below_vector_y)||closest_below_vector_y==-1)
                {
                    closest_below_vector_y = this_below_vector_y;
                    closest_below_vector_index = i;
                }
    }
    return vectors_below_checking_point[closest_below_vector_index];
}


bool CollisionSeries::serialize(fstream& file)
{
    if(!file.is_open()) return false;
    int collision_series_version = 1;
    file.write((char*) &collision_series_version, sizeof(collision_series_version));
    size_t num_points = points.size(); //store the number of loaded textures in a temporary variable
    file.write((char*) &num_points, sizeof num_points); //write the number of textures to the file
    for(unsigned int i = 0; i<num_points;i++)
    {
        if(!points[i].serialize(file)) return false;
    }
    return true;
}

bool CollisionSeries::unserialize(ifstream& file)
{
    if(!file.is_open()) return false;
    int collision_series_version;
    file.read((char*) &collision_series_version,sizeof(int));
    if(collision_series_version==1)
    {
        points.clear(); //to eliminate the first point which must be created when the constructor for CollisionSeries is called
        size_t num_points;
        file.read((char*) &num_points, sizeof(num_points)); //read in the number of statics in the layer from the file
        if(num_points>0){ //if there are any statics in the layer
                points.reserve(num_points); //add empty vector space for the statics to be loaded
                for(unsigned int i = 0; i<num_points; i++) //go through all the statics in the file
                {
                    CollisionPoint temp_point(0,0);
                    if(!temp_point.unserialize(file)) return false;
                    points.push_back(temp_point);
                }
            }
    return true;
    }
    else return false;
}


