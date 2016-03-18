#ifndef UI_TREEEDITOR_HPP
#define UI_TREEEDITOR_HPP

#include "BaseUIElement.hpp"
#include "Entity_TreeType.hpp"

class UI_TreeEditor : public BaseUIElement
{
    public:
        UI_TreeEditor(int x, int y, int w, int h, Entity_TreeType* Tree_To_Edit);
        int tree_view_origin_x, tree_view_origin_y;
        int mx, my;
        int active_branch_index, active_node_index, closest_vector_node1, closest_vector_node2;
        vec_2d closest_vector;
        bool is_adding_node;
        int new_node_x, new_node_y;
        void update(SDL_Event event);
        virtual ~UI_TreeEditor();
        Entity_TreeType * Editing_Tree;
        TreeInfo Tree_Debug_Info;
};
#endif // UI_TREEEDITOR_HPP
