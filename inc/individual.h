#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "individual_config.h"

class Individual
{
private:
    int health;
    int brain;
    int consumability;
    int points;
    int create_new_brain(int x_brain, int y_brain);
public:
    Individual(int x_brain=0, int y_brain=0);
    ~Individual();

    int get_brain();
    int get_points();
    int get_health();
    void complete_step();
    void eat();
    void die();
    bool is_edible(int type);
};

#endif