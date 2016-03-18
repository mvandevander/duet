#include "Tree_Branch.hpp"
#include "GL_DrawingPrimitives.hpp"

Tree_Branch::Tree_Branch(double first_pt_x, double first_pt_y, int first_pt_width)
{
    nodes.push_back(Tree_Node(first_pt_x,first_pt_y,first_pt_width));
}


void Tree_Branch::draw(GLuint displaylist,float r, float g, float b, float a)
{

    for(unsigned int i = 0; i<nodes.size();i++)
    {
        Tree_Node &temp = nodes[i];
        DrawGLRectSolidFromCurrentMatrix(displaylist,(int)temp.x-4,(int)temp.y-4,8,8,r,g,b,a);
        if(i!=nodes.size()-1)DrawGLLineFromCurrentMatrix((int)temp.x,(int)temp.y,(int)nodes[i+1].x,(int)nodes[i+1].y,r/2,g/2,b/2,a);
    }
}
