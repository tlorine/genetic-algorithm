#ifndef WORLD_H
# define WORlD_H

# include "individual.h"
# include "stack.h"

# define WORLD_SIZE 100

/*
** TYPES
*/

# define IND 1
# define FOOD 2
# define WALL 3
# define POIS 4

typedef struct  s_cell
{
    Individual  *individual;
    int         cell_type;
    bool        visited;
    int         x_anc;
    int         y_anc;
}               t_cell;

typedef struct  s_action
{
    int         x;
    int         y;
    int         type;
}               t_action;

typedef struct  s_list_ind
{
    Individual *Individual;
    struct s_list_ind *next;
    int x;
    int y;
}               t_list;

class world
{
private:
    int count_ind;
    int count_food;
    int count_poison;
    t_list *ind_list;
    t_cell **map;

    t_list      *get_ind_list(int count);
    t_list      *get_roul();
    t_action    *create_action(int x, int y, int type);
    /*
    ** LIVE_LOGICK
    */
    t_action    *search_food(int x, int y, Individual *ind);
    t_action    *inspect_food(int x, int y, Individual *ind);
    bool        inspect_enemy(Individual *ind, t_action *action);
    t_action    *look_around(int x, int y, Individual *ind);
    void        live_ind(t_list *ind);

    void        delete_ind_list();
public:
    world(int count_ind);
    ~world();
    void write_ind_list();
    void delete_world();
    void live(int count_cycles); // -
    void scatter(int type, int count); // раскидываем объекты по типам
    void settlement(); // заселяем своими особями
    void selection();
    void write_map();
    void clean_map();
    void build_wall(int x, int y, int lenght, int flag); // -
};


#endif