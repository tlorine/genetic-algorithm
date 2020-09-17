#include "world.h"
#include <iostream>

void vis_brain(int num, int size)
{
    if (size)
        vis_brain(num / 2, size - 1);
    if (size == BRAIN_SIZE / 2)
        std::cout << "|";
    std::cout << num % 2 << " ";
}

world::world(int count_ind)
{
    this->count_ind = count_ind;
    count_food = 0;
    count_poison = 0;
    ind_list = get_ind_list(count_ind);
    map = new t_cell *[WORLD_SIZE];
    for (int i = 0; i < WORLD_SIZE; i++)
        map[i] = new t_cell [WORLD_SIZE];
}

t_list *world::get_ind_list(int count)
{
    t_list *start;
    t_list *elem;
    int i;

    i = 0;
    start = nullptr;
    elem = nullptr;
    while (i < count)
    {
        if (elem != nullptr)
        {
            elem->next = new t_list;
            elem = elem->next;
        }
        else
            elem = new t_list;
        elem->Individual = new Individual();
        elem->x = 0;
        elem->y = 0;
        elem->next = nullptr;
        if (i == 0)
            start = elem;
        i++;
    }
    return (start);
}

void world::write_ind_list()
{
    t_list *tmp_lst;

    tmp_lst = ind_list;
    while(tmp_lst)
    {
        vis_brain(tmp_lst->Individual->get_brain(), BRAIN_SIZE - 1);
        std::cout << "\n";
        tmp_lst = tmp_lst->next;
    }
}

void world::delete_ind_list()
{
    t_list *tmp;

    tmp = ind_list;
    while (ind_list)
    {
        tmp = ind_list;
        ind_list = ind_list->next;
        delete tmp->Individual;
        tmp->Individual = nullptr;
        delete tmp;
    }
    ind_list = nullptr;
}

void world::delete_world()
{
    delete_ind_list();
    for (int i = 0; i < WORLD_SIZE; i++)
        delete map[i];
    delete map;
}

void world::clean_map()
{
    count_ind = 0;
    count_poison = 0;
    count_food = 0;
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int i_2 = 0; i_2 < WORLD_SIZE; i_2++)
        {
            map[i][i_2].cell_type = 0;
            map[i][i_2].individual = nullptr;
            map[i][i_2].visited = false;
            map[i][i_2].x_anc = -1;
            map[i][i_2].y_anc = -1;
        }
    }
}

void world::settlement()
{
    t_list *list_p;
    int x = 0, y = 0;
    list_p = ind_list;
    for(list_p = ind_list; list_p; list_p = list_p->next)
    {
        while(map[y][x].cell_type != 0)
        {
            x = std::rand() % WORLD_SIZE;
            y = std::rand() % WORLD_SIZE;
        }
        count_ind++;
        map[y][x].cell_type = IND;
        map[y][x].individual = list_p->Individual;
        list_p->x = x;
        list_p->y = y;
    }
}

void world::write_map()
{
    for(int i = 0; i < WORLD_SIZE; i++)
    {
        for(int i_2 = 0; i_2 < WORLD_SIZE; i_2++)
        {
            std::cout << map[i][i_2].cell_type;
            std::cout << (i_2 + 1 == WORLD_SIZE ? "\n" : " ");
        }
    }
}

void world::scatter(int type, int count)
{
    int x = 0, y = 0;
    if (type == FOOD)
        count_food = count;
    if (type == POIS)
        count_poison = count;
    for(int i = 0; i < count; i++)
    {
        while(map[y][x].cell_type != 0)
        {
            x = std::rand() % WORLD_SIZE;
            y = std::rand() % WORLD_SIZE;
        }
        map[y][x].cell_type = type;
    }
}

void world::build_wall(int x, int y, int lenght, int flag)
{
    int x_direction;
    int y_direction;
    int i;

    i = 0;
    x_direction = 0;
    y_direction = 0;
    if (flag == UP || flag == DOWN)
        y_direction = flag == UP ? -1 : 1;
    if (flag == LEFT || flag == RIGHT)
        x_direction = flag == LEFT ? -1 : 1;
    while (i < lenght)
    {
        if ((x >= 0 && x < WORLD_SIZE) && (y >= 0 && y < WORLD_SIZE))
            map[y][x].cell_type = WALL;
        else
            break;
        x += x_direction;
        y += y_direction;
        i++;
    }
}

t_list *world::get_roul()
{
    t_list *roul = new t_list[100];
    int sum;
    int interest;
    int i;
    int count;

    i = 0;
    sum = 0;
    interest = 0;
    count = 0;
    for (t_list *begin = ind_list; begin; begin = begin->next)
        sum += begin->Individual->get_points();
    if (!sum)
        return nullptr;
    for (t_list *begin = ind_list; begin; begin = begin->next)
    {
        interest = (begin->Individual->get_points() / sum) * 100;
        count += interest;
        while(i < count && i < 100)
        {
            roul[i].Individual = begin->Individual;
            i++;
        }
    }
    while (i < 100)
    {
        roul[i].Individual = nullptr;
        i++;
    }
    return (roul);
}

void world::selection()
{
    t_list *roul;
    t_list *new_ind_list;
    t_list *begin_ind_list;
    int rand_ancestor;

    new_ind_list = nullptr;
    begin_ind_list = nullptr;
    if(!(roul = get_roul()))
        return ;
    for (t_list *begin = ind_list; begin; begin = begin->next)
    {
        rand_ancestor = rand() % 100 + 1;
        while (roul[rand_ancestor].Individual == begin->Individual || roul[rand_ancestor].Individual == nullptr)
            rand_ancestor = rand() % 100 + 1;
        if (new_ind_list == nullptr)
        {
            new_ind_list = new t_list;
            begin_ind_list = new_ind_list;
        }
        else
        {
            begin_ind_list->next = new t_list;
            begin_ind_list = begin_ind_list->next;
        }
        begin_ind_list->Individual = new Individual(begin->Individual->get_brain(), roul[rand_ancestor].Individual->get_brain());
        begin_ind_list->next = nullptr;
    }
    delete roul;
    delete_ind_list();
    ind_list = new_ind_list;
}

world::~world()
{
}