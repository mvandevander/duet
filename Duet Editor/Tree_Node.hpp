#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP


class Tree_Node
{
    public:
        Tree_Node(double x, double y, int width);
        void move(double x, double y);
        void resize(int width);
        double x, y; //in relation to origin of tree, not in world coordinates
        bool does_split_here;
        int width;
};

#endif // TREE_NODE_HPP
