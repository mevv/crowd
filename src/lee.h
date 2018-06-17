#ifndef LEE_H
#define LEE_H

#include "leePathfinder.h"
#include <vector>

std::vector<std::pair<double, double> > Lee(std::vector<double> map,
                                            int width, int height ,
                                            std::pair<double, double> start,
                                            std::pair<double, double> end);
#endif // LEE_H
