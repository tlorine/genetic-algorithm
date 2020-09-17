#include "individual.h"
#include "world.h"
#include <iostream>

int Individual::create_new_brain(int x_brain, int y_brain)
{
    int i;
    int brain;
    int brain_size;
    int brain_pointer;

    brain_pointer = 1;
    brain = 0;
    i = 0;
    while (i < BRAIN_SIZE / 2)
    {
        if (x_brain)
            brain |= (x_brain & brain_pointer) ? brain_pointer : 0;
        else if (rand() % 2 == 1)
           brain |= brain_pointer;
        brain_pointer = brain_pointer * 2;
        i++;
    }
    while (i < BRAIN_SIZE)
    {
        if (y_brain)
            brain |= (y_brain & brain_pointer) ? brain_pointer : 0;
        else if (rand() % 2 == 1)
            brain |= brain_pointer;
        brain_pointer = brain_pointer * 2;
        i++;
    }
    return (brain);
}

Individual::Individual(int x_brain, int y_brain)
{
    brain = create_new_brain(x_brain, y_brain);
    health = HEALTH;
    points = 0;
}

Individual::~Individual()
{
}

void Individual::complete_step()
{
    health = health - consumability;
    points += 1;
}

int Individual::get_points()
{
    return (points);
}

int Individual::get_brain()
{
    std::cout << "get_brain" << std::endl;
    return (brain);
}

int Individual::get_health()
{
    return health;
}

bool Individual::is_edible(int type)
{
    std::cout << "is_edible" << std::endl;
    if (type == FOOD || (type == POIS && !(brain & POISON_EAT)) || (type == IND && (brain & HUNT)))
        return true;
    else
        return false;
}

void Individual::eat()
{
    health += EAT_ENERGY;
}

void Individual::die()
{
    std::cout << "SAM DIE\n";
    health = 0;
}