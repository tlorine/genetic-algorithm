#include "world.h"
#include <stdio.h>
#include <iostream>

char *get_two_sys_num(int num)
{
    int save_num = num;
    int size = 0;
    char *str_num = nullptr;
    while (save_num)
    {
        save_num /= 2;
        size++;
    }
    if (size == 0)
        return str_num;
    str_num = new char[size + 1];
    str_num[size] = '\0';
    size--;
    for (int i = num; i; i /= 2)
    {
        str_num[size] = (i % 2) + '0';
        size--;
    }
    return (str_num);
}

bool world::inspect_enemy(Individual *ind, t_action *action)
{
    int ai = ind->get_brain() & AI;
    if (action->x + 1 < WORLD_SIZE && map[action->y][action->x + 1].cell_type == IND)
    {
        if (!ai || (ai && (map[action->y][action->x + 1].individual->get_brain() & HUNT)))
            return true;
    }
    if (action->x - 1 >= 0 && map[action->y][action->x - 1].cell_type == IND)
    {
        if (!ai || (ai && (map[action->y][action->x - 1].individual->get_brain() & HUNT)))
            return true;
    }
    if (action->y + 1 < WORLD_SIZE && map[action->y + 1][action->x].cell_type == IND)
    {
        if (!ai || (ai && (map[action->y + 1][action->x].individual->get_brain() & HUNT)))
            return true;
    }
    if (action->y - 1 >= 0 && map[action->y - 1][action->x].cell_type == IND)
    {
        if (!ai || (ai && (map[action->y - 1][action->x].individual->get_brain() & HUNT)))
            return true;
    }
    return false;
}

t_action *world::create_action(int x, int y, int type)
{
    t_action *action = new t_action;
    action->x = x;
    action->y = y;
    action->type = type;
    return action;
}

t_action *world::inspect_food(int x, int y, Individual *ind)
{
    t_action *action;

    action = nullptr;
    if ((ind->get_brain() & DOWN) && y + 1 < WORLD_SIZE && ind->is_edible(map[y + 1][x].cell_type))
    {
        action = create_action(x, y + 1, FOOD);
        if ((ind->get_brain() & SCARY) && inspect_enemy(ind, action))
        {
            delete action;
            action = nullptr;
        }
        else
            return action;
    }
    if ((ind->get_brain() & RIGHT) && x + 1 < WORLD_SIZE && ind->is_edible(map[y][x + 1].cell_type))
    {
        action = create_action(x + 1, y, FOOD);
        if ((ind->get_brain() & SCARY) && inspect_enemy(ind, action))
        {
            delete action;
            action = nullptr;
        }
        else
            return action;
    }
    if ((ind->get_brain() & LEFT) && x - 1 >= 0 && ind->is_edible(map[y][x - 1].cell_type))
    {
        action = create_action(x - 1, y, FOOD);
        if ((ind->get_brain() & SCARY) && inspect_enemy(ind, action))
        {
            delete action;
            action = nullptr;
        }
        else
            return action;
    }
    if ((ind->get_brain() & UP) && y - 1 >= 0 && ind->is_edible(map[y - 1][x].cell_type))
    {
        action = create_action(x, y - 1, FOOD);
        if ((ind->get_brain() & SCARY) && inspect_enemy(ind, action))
        {
            delete action;
            action = nullptr;
        }
        else
            return action;
    }
    return nullptr;
}

void search_push(stack *stack_bfs, stack *stack_save, t_stack *elem, t_cell **map, int y, int x, int brain)
{
    if ((map[elem->y + y][elem->x + x].cell_type == IND && !(brain & HUNT)))
        return ;
    map[elem->y + y][elem->x + x].visited = true;
    if (map[elem->y][elem->x].x_anc == -1)
    {
        map[elem->y + y][elem->x + x].x_anc = elem->x + x;
        map[elem->y + y][elem->x + x].y_anc = elem->y + y;
    }
    else
    {
        map[elem->y + y][elem->x + x].x_anc = map[elem->y][elem->x].x_anc;
        map[elem->y + y][elem->x + x].y_anc = map[elem->y][elem->x].y_anc;
    }
    stack_save->push(elem->x + x, elem->y + y);
    stack_bfs->push(elem->x + x, elem->y + y);
}

t_action *world::search_food(int x, int y, Individual *ind)
{
    int brain = ind->get_brain();
    stack *stack_bfs = new stack;
    stack *stack_save = new stack;
    t_action *action = nullptr;
    t_stack *elem = nullptr;

    stack_bfs->push(x, y);
    while (elem = stack_bfs->get_elem())
    {
        if (map[elem->y][elem->x].cell_type == FOOD || (map[elem->y][elem->x].cell_type == IND && (brain & HUNT)) || (map[elem->y][elem->x].cell_type == POIS && !(brain & POISON_EAT)))
        {
            if (elem->y != y && elem->x != x)
                break ;
        }
        if ((brain & UP) && elem->y - 1 >= 0 && !map[elem->y - 1][elem->x].visited)
        {
            if (map[elem->y - 1][elem->x].cell_type != WALL || (brain & JUMP))
                search_push(stack_bfs, stack_save, elem, map, -1, 0, brain);
        }
        if ((brain & DOWN) && elem->y + 1 < WORLD_SIZE && !map[elem->y + 1][elem->x].visited)
        {
            if (map[elem->y + 1][elem->x].cell_type != WALL || (brain & JUMP))
                search_push(stack_bfs, stack_save, elem, map, 1, 0, brain);
        }
        if ((brain & LEFT) && elem->x - 1 >= 0  && !map[elem->y][elem->x - 1].visited)
        {
            if (map[elem->y][elem->x - 1].cell_type != WALL || (brain & JUMP))
                search_push(stack_bfs, stack_save, elem, map, 0, -1, brain);
        }
        if ((brain & RIGHT) && elem->y + 1 < WORLD_SIZE && !map[elem->y][elem->x + 1].visited)
        {
            if (map[elem->y][elem->x + 1].cell_type != WALL || (brain & JUMP))
                search_push(stack_bfs, stack_save, elem, map, 0, 1, brain);
        }
        stack_bfs->delete_elem();
    }
    if (elem != nullptr)
        action = create_action(map[elem->y][elem->x].x_anc, map[elem->y][elem->x].y_anc, FOOD);
    else
        return nullptr;
    
    while (stack_bfs->get_elem())
        stack_bfs->delete_elem();
    delete stack_bfs;
    while (elem = stack_save->get_elem())
    {
        map[elem->y][elem->x].x_anc = -1;
        map[elem->y][elem->x].y_anc = -1;
        map[elem->y][elem->x].visited = false;
        stack_save->delete_elem();
    }
    delete stack_save;
    return (action);
}

t_action *world::look_around(int x, int y, Individual *ind)
{
    t_action *action;
    action = inspect_food(x, y, ind);
    if (action)
        return action;
    else if (ind->get_brain() & VISION)
    {
        action = search_food(x, y, ind);
        if (action && !inspect_enemy(ind, action))
            return action;
        if (action)
            delete action;
    }
    return nullptr;

}

void world::live_ind(t_list *ind)
{
    t_action *action;
    action = look_around(ind->x, ind->y, ind->Individual);
    if(action != nullptr)
    {
        char *tmp = get_two_sys_num(ind->Individual->get_brain());
        printf("action: x %d, y :%d | brain %s\n", action->x, action->y, tmp);
        if (tmp)
            delete tmp;
        if (map[action->y][action->x].cell_type == FOOD || map[action->y][action->x].cell_type == IND)
        {
            if (map[action->y][action->x].cell_type == IND)
                map[action->y][action->x].individual->die();
            ind->Individual->eat();
        }
        else if (map[action->y][action->x].cell_type == POIS)
            ind->Individual->die();
        map[ind->y][ind->x].cell_type = 0;
        map[ind->y][ind->x].individual = nullptr;
        map[action->y][action->x].cell_type = IND;
        map[action->y][action->x].individual = ind->Individual;
        ind->x = action->x;
        ind->y = action->y;
        ind->Individual->complete_step();
    }
    if(ind->Individual->get_health() <= 0)
    {
        map[ind->y][ind->x].cell_type = 0;
        map[ind->y][ind->x].individual = nullptr;
    }
}

void world::live(int count_cycles)
{
    for(int i = 0; i < count_cycles; i++)
    {
        for (t_list *begin = ind_list; begin; begin = begin->next)
        {
            if (begin->Individual->get_health() > 0)
                live_ind(begin);
        }
    }
}