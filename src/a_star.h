#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include "map_search_node.h"

//1 - FREE, 9 - OBSTACLE
std::vector< std::pair<double, double> > Astar(std::vector<double> map,
                                               int width, int height ,
                                               std::pair<double, double> start,
                                               std::pair<double, double> end);

#endif
