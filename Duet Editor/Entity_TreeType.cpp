#include "Entity_TreeType.hpp"
#include "trig_functions_degrees.hpp"
#include "GL_DrawingPrimitives.hpp"
Entity_TreeType::Entity_TreeType(int origin_x, int origin_y, int first_pt_width)
{
    x = origin_x;
    y = origin_y;
    type = 't';
    branches.push_back(Tree_Branch(0,0,first_pt_width));
    bounds.x = x-40;
    bounds.y = y-40;
    w = 80;
    h = 80;
    bounds.w = w;
    bounds.h = h;
}

Entity_TreeType::~Entity_TreeType()
{

}

void Entity_TreeType::update()
{

}

void Entity_TreeType::add_node_in_treespace(int new_node_x, int new_node_y) //assumes that input coordinates are in tree-space, with the origin being the coordinates of the first point of the tree.
{
    point_2d checking_point(new_node_x,new_node_y);
    TreeInfo Info = find_tree_info(new_node_x,new_node_y);
    Tree_Node New_Node(new_node_x,new_node_y, Info.Closest_Branch->nodes[Info.index_of_second_node_in_closest_vector].width);

    if(unsigned (Info.index_of_closest_node)==Info.Closest_Branch->nodes.size()-1){
        Info.Closest_Branch->nodes.push_back(New_Node);
    }
    else Info.Closest_Branch->nodes.insert(Info.Closest_Branch->nodes.begin()+Info.index_of_second_node_in_closest_vector,New_Node);


}


TreeInfo Entity_TreeType::find_tree_info(int tree_space_x, int tree_space_y)
{
    point_2d checking_point(tree_space_x,tree_space_y);
    int index_of_second_node_in_closest_vector = 0,index_of_closest_branch = 0,index_of_closest_node = 0;
    float shortest_distance = -1;
    for(unsigned int i = 0; i<branches.size(); i++) //need to find the closest branch, so we can attach the new node to that branch.
    {
        if(branches[i].nodes.size()==1){
            float dist_to_single_noded_branch = GetDistance(tree_space_x,tree_space_y,(int)branches[i].nodes[0].x,(int)branches[i].nodes[0].y);
            if(dist_to_single_noded_branch<shortest_distance||shortest_distance==-1){
                shortest_distance = dist_to_single_noded_branch;
                index_of_second_node_in_closest_vector = 0;
                index_of_closest_branch = i;
                index_of_closest_node = 0;
            }
        }
        else{
            for(unsigned int a = 0; a<branches[i].nodes.size()-1; a++) //also need to find the closest vector (line between nodes) on that branch, and insert the new node in-between those.
            {
                vec_2d this_segment((int)branches[i].nodes[a].x,(int)branches[i].nodes[a].y,(int)branches[i].nodes[a+1].x,(int)branches[i].nodes[a+1].y);
                vec_2d shortest_vector = find_shortest_vector_from_point_to_line(checking_point,this_segment);
                if(shortest_vector.len<shortest_distance||shortest_distance==-1)
                {
                    shortest_distance = shortest_vector.len;
                    index_of_second_node_in_closest_vector = a+1;
                    index_of_closest_branch = i;

                    if(GetDistanceSquared(tree_space_x,tree_space_y,(int)branches[i].nodes[a].x,(int)branches[i].nodes[a].y)<=GetDistanceSquared(tree_space_x,tree_space_y,(int)branches[i].nodes[a+1].x,(int)branches[i].nodes[a+1].y)){
                        index_of_closest_node = a;
                    }
                    else index_of_closest_node = a+1;
                }
            }
        }
    }
    TreeInfo Info_From_Search;
    Info_From_Search.Closest_Branch = &branches[index_of_closest_branch];
    Info_From_Search.Closest_Node = &(Info_From_Search.Closest_Branch->nodes[index_of_closest_node]);
    Info_From_Search.index_of_closest_branch = index_of_closest_branch;
    Info_From_Search.is_real = true;
    Info_From_Search.index_of_second_node_in_closest_vector = index_of_second_node_in_closest_vector;
    Info_From_Search.index_of_closest_node = index_of_closest_node;
    return Info_From_Search;
}


void Entity_TreeType::draw(GLuint displaylist,float r, float g, float b, float a)
{
    for(unsigned int i = 0; i<branches.size();i++)
    {
        branches[i].draw(displaylist,r,g,b,a);
    }
    DrawGLRectSolidFromCurrentMatrix(displaylist,-5,-5,10,10,0.0,0.0,0.0,0.8);
}


bool Entity_TreeType::serialize(fstream& file)
{
return true;
}

bool Entity_TreeType::unserialize(ifstream& file)
{
return true;
}
