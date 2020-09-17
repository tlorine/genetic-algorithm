#include "world.h"
#include <stdio.h>
#include <iostream>

bool world::inspect_enemy(Individual *ind, t_action *action)
{
    int ai = ind->get_brain() & AI;
    std::cout << "inspect_enemy ";
    printf("y:%d x:%d\n", action->y, action->x);
    if (action->x + 1 < WORLD_SIZE && map[action->y][action->x + 1].cell_type == IND)
    {
        if (!ai || (ai && (map[action->y][action->x + 1].individual->get_brain() & HUNT)))
            return true;
    }
    std::cout << "inspect_enemy 2";
    if (action->x - 1 >= 0 && map[action->y][action->x - 1].cell_type == IND)
    {
        if (!ai || (ai && (map[action->y][action->x - 1].individual->get_brain() & HUNT)))
        {
            std::cout << ": TRUE :";
            return true;
        }
    }
    std::cout << "inspect_enemy 3";
    if (action->y + 1 < WORLD_SIZE && map[action->y + 1][action->x].cell_type == IND)
    {
        if (!ai || (ai && (map[action->y + 1][action->x].individual->get_brain() & HUNT)))
            return true;
    }
    std::cout << "inspect_enemy 4";
    if (action->y - 1 >= 0 && map[action->y - 1][action->x].cell_type == IND)
    {
        if (!ai || (ai && (map[action->y - 1][action->x].individual->get_brain() & HUNT)))
            return true;
    }
    std::cout << "inspect_enemy: end" << std::endl;
    return false;
}

t_action *world::create_action(int x, int y, int type)
{
    std::cout << "create_action" << std::endl;
    t_action *action = new t_action {x, y, type};
    return action;
}

t_action *world::inspect_food(int x, int y, Individual *ind)
{
    t_action *action;

    std::cout << "inscpect_food" << std::endl;
    action = nullptr;
    if ((ind->get_brain() & DOWN) && y + 1 < WORLD_SIZE && ind->is_edible(map[y + 1][x].cell_type))
    {
        std::cout << "inscpect_food: 1-if" << std::endl;
        action = create_action(x, y + 1, FOOD);
        if ((ind->get_brain() & SCARY) && inspect_enemy(ind, action))
        {
            std::cout << "inspect_food: delete action" << std::endl;
            delete action;
            action = nullptr;
        }
        else
            return action;
    }
    if ((ind->get_brain() & RIGHT) && x + 1 < WORLD_SIZE && ind->is_edible(map[y][x + 1].cell_type))
    {
        std::cout << "inscpect_food: 2-if" << std::endl;
        action = create_action(x + 1, y, FOOD);
        if ((ind->get_brain() & SCARY) && inspect_enemy(ind, action))
        {
            std::cout << "inspect_food: delete action 2" << std::endl;
            delete action;
            action = nullptr;
        }
        else
            return action;
    }
    if ((ind->get_brain() & LEFT) && x - 1 >= 0 && ind->is_edible(map[y][x - 1].cell_type))
    {
        std::cout << "inscpect_food: 3-if" << std::endl;
        action = create_action(x - 1, y, FOOD);
        if ((ind->get_brain() & SCARY) && inspect_enemy(ind, action))
        {
            std::cout << "inspect_food: delete action 3" << std::endl;
            delete action;
            action = nullptr;
        }
        else
            return action;
    }
    if ((ind->get_brain() & UP) && y - 1 >= 0 && ind->is_edible(map[y - 1][x].cell_type))
    {
        std::cout << "inscpect_food: 4-if" << std::endl;
        action = create_action(x, y - 1, FOOD);
        if ((ind->get_brain() & SCARY) && inspect_enemy(ind, action))
        {
            std::cout << "inspect_food: delete action 4" << std::endl;
            delete action;
            action = nullptr;
        }
        else
            return action;
    }
    std::cout << "inscpect_food: end" << std::endl;
    return nullptr;
}

void search_push(stack *stack_bfs, stack *stack_save, t_stack *elem, t_cell **map, int y, int x, int brain)
{
    std::cout << "search_push" << std::endl;
    printf("y:%d x:%d c+t:%d\n", elem->y, elem->x, map[elem->y][elem->x].cell_type);
    if ((map[elem->y + y][elem->x + x].cell_type == IND && !(brain & HUNT)))
    {
        std::cout << "search_push RETURN" << std::endl;
        return ;
    }
    std::cout << "IF 1" << std::endl;
    map[elem->y + y][elem->x + x].visited = true;
    if (map[elem->y][elem->x].x_anc == -1)
    {
        std::cout << "IF 2" << std::endl;
        map[elem->y + y][elem->x + x].x_anc = elem->x + x;
        map[elem->y + y][elem->x + x].y_anc = elem->y + y;
    }
    else
    {
        std::cout << "ELSE 2" << std::endl;
        std::cout << map[elem->y][elem->x].x_anc << " :SF:" << map[elem->y][elem->x].y_anc << std::endl;
        map[elem->y + y][elem->x + x].x_anc = map[elem->y][elem->x].x_anc;
        map[elem->y + y][elem->x + x].y_anc = map[elem->y][elem->x].y_anc;
    }
    std::cout << "KAL" << std::endl;
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

    std::cout << "search_food" << std::endl;
    stack_bfs->push(x, y);
    while (elem = stack_bfs->get_elem())
    {
        std::cout << "search_food: main_cicle start" << std::endl;
        if (map[elem->y][elem->x].cell_type == FOOD || (map[elem->y][elem->x].cell_type == IND && (brain & HUNT)) || (map[elem->y][elem->x].cell_type == POIS && !(brain & POISON_EAT)))
        {
            if (elem->y != y && elem->x != x)
                break ;
        }
        if ((brain & UP) && elem->y - 1 >= 0 && !map[elem->y - 1][elem->x].visited)
        {
            std::cout << "search_food: if 1" << std::endl;
            if (map[elem->y - 1][elem->x].cell_type != WALL || (brain & JUMP))
                search_push(stack_bfs, stack_save, elem, map, -1, 0, brain);
        }
        if ((brain & DOWN) && elem->y + 1 < WORLD_SIZE && !map[elem->y + 1][elem->x].visited)
        {
            std::cout << "search_food: if 2" << std::endl;
            if (map[elem->y + 1][elem->x].cell_type != WALL || (brain & JUMP))
                search_push(stack_bfs, stack_save, elem, map, 1, 0, brain);
        }
        if ((brain & LEFT) && elem->x - 1 >= 0  && !map[elem->y][elem->x - 1].visited)
        {
            std::cout << "search_food: if 3" << std::endl;
            if (map[elem->y][elem->x - 1].cell_type != WALL || (brain & JUMP))
                search_push(stack_bfs, stack_save, elem, map, 0, -1, brain);
        }
        if ((brain & RIGHT) && elem->y + 1 < WORLD_SIZE && !map[elem->y][elem->x + 1].visited)
        {
            std::cout << "search_food: if 4" << std::endl;
            if (map[elem->y][elem->x + 1].cell_type != WALL || (brain & JUMP))
                search_push(stack_bfs, stack_save, elem, map, 0, 1, brain);
        }
        stack_bfs->delete_elem();
        std::cout << "search_food: main_cicle end" << std::endl;
    }
    if (elem != nullptr)
    {
        std::cout << map[elem->y][elem->x].y_anc << " :SF:" << map[elem->y][elem->x].x_anc << std::endl;
        action = create_action(map[elem->y][elem->x].x_anc, map[elem->y][elem->x].y_anc, FOOD);
    }
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
    std::cout << "search_food: end function" << std::endl;
    return (action);
}

t_action *world::look_around(int x, int y, Individual *ind)
{
    std::cout << "look_around" << std::endl;
    t_action *action;
    action = nullptr;

    if (action = inspect_food(x, y, ind))
        return action;
    else if (ind->get_brain() & VISION)
    {
        action = search_food(x, y, ind);
        std::cout << "LOOK AROUND" << std::endl;
        if (action && !inspect_enemy(ind, action))
        {
            std::cout << "look_around: return act" << std::endl;
            return action;
        }
        std::cout << "LOOK AROUND 2" << std::endl;
        if (action)
            delete action;
        std::cout << "LOOK AROUND 3" << std::endl;
    }
    return nullptr;

}

void world::live_ind(t_list *ind)
{
    t_action *action;
    std::cout << "live_ind" << std::endl;
    action = look_around(ind->x, ind->y, ind->Individual);
    if(action != nullptr)
    {
        std::cout << action->y << " " << action->x << std::endl;
        if (map[action->y][action->x].cell_type == FOOD || map[action->y][action->x].cell_type == IND)
        {
            std::cout << "live_ind if 1" << std::endl;
            if (map[action->y][action->x].cell_type == IND)
            {
                std::cout << "DIE" << std::endl;
                map[action->y][action->x].individual->die();
            }
            std::cout << "live_ind if 1 EAT" << std::endl;
            ind->Individual->eat();
        }
        else if (map[action->y][action->x].cell_type == POIS)
            ind->Individual->die();
        std::cout << "live_ind if 1/ 2" << std::endl;
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