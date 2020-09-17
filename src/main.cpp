#include "individual.h"
#include "world.h"
#include <iostream>
#include <time.h>

using namespace std;

int main()
{
    srand(time(NULL));
    world wrld = world(500);
    wrld.clean_map();
    wrld.build_wall(0, 5, 50, RIGHT);
    wrld.build_wall(99, 8, 50, LEFT);
    wrld.settlement();
    wrld.scatter(FOOD, 100);
    wrld.scatter(POIS, 30);
    wrld.live(1);
    wrld.delete_world();
}