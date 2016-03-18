#ifndef ENTITY_TREETYPE_HPP
#define ENTITY_TREETYPE_HPP
#include "Entity.hpp"
#include "Tree_Node.hpp"
#include "Tree_Branch.hpp"
#include <vector>
#include "point_2d.hpp"
#include "vec_2d.hpp"

#include <GL/glew.h>

using namespace std;

struct TreeInfo
{
    Tree_Branch* Closest_Branch;
    Tree_Node* Closest_Node;
    int index_of_closest_branch, index_of_second_node_in_closest_vector, index_of_closest_node;
    bool is_real;
};

class Entity_TreeType : public Entity
{
    public:
    //the coordinates of all points in a tree are handled in tree-space, meaning that the origin is the position of the first point of the tree.
        Entity_TreeType(int origin_x, int origin_y, int first_pt_w);
        virtual ~Entity_TreeType();
        vector <Tree_Branch> branches; //branches are organized by splits, so branches[0] is the trunk, branches[1-2] are the first two splits
        void update();
        void draw(GLuint displaylist,float r, float g, float b, float a);
        void add_node_in_treespace(int new_node_x, int new_node_y);
        TreeInfo find_tree_info(int tree_space_x, int tree_space_y);
        bool serialize(fstream& file);
        bool unserialize(ifstream& file);
};



#endif // ENTITY_TREETYPE_HPP
