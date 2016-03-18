#ifndef TREE_BRANCH_HPP
#define TREE_BRANCH_HPP
#include <vector>
#include "Tree_Node.hpp"

#include <GL/glew.h>

using namespace std;

class Tree_Branch
{
    public:
        Tree_Branch(double first_pt_x, double first_pt_y, int first_pt_width);
        void draw(GLuint displaylist,float r, float g, float b, float a);
        vector <Tree_Node> nodes;
};

#endif // TREE_BRANCH_HPP
