#include "UI_TreeEditor.hpp"

UI_TreeEditor::UI_TreeEditor(int x, int y ,int w ,int h, Entity_TreeType* Tree_To_Edit)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    tree_view_origin_x = x+(w/2);
    tree_view_origin_y = y+h-10;
    type = "TreeEditor";
    Editing_Tree = Tree_To_Edit;
    mouse_is_hovered_over = false;
    mx = 0;
    my = 0;
    is_adding_node = false;
    Tree_Debug_Info.is_real = false;
}

void UI_TreeEditor::update(SDL_Event event)
{
    if(event.type == SDL_MOUSEMOTION)
    {
        mx = event.motion.x;
        my = event.motion.y;
        if(is_mouse_over(mx,my))
        {
            mouse_is_hovered_over = true;
        }
        else mouse_is_hovered_over = false;
    }

    if(mouse_is_hovered_over)
    {
        Tree_Debug_Info = Editing_Tree->find_tree_info(mx-tree_view_origin_x,my-tree_view_origin_y);
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
        {
            is_adding_node = true;
        }
        if(is_adding_node)
        {
            new_node_x = mx;
            new_node_y = my;
        }
        if(event.type ==SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT&&is_adding_node)
        {
            is_adding_node = false;
            Editing_Tree->add_node_in_treespace(new_node_x-tree_view_origin_x,new_node_y-tree_view_origin_y);
        }
    }

}

UI_TreeEditor::~UI_TreeEditor()
{
    //dtor
}
