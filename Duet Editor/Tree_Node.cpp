#include "Tree_Node.hpp"

Tree_Node::Tree_Node(double x, double y, int width)
{
    this->x = x;
    this->y = y;
    this->width = width;
    does_split_here = false;
}

void Tree_Node::move(double x, double y)
{
    this->x = x;
    this->y = y;
}

void Tree_Node::resize(int width)
{
    this->width = width;
}
