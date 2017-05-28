#ifndef LEE_H
#define LEE_H

#include <vector>
#include "QDebug"

std::vector<std::pair<double, double> > Lee(std::vector<double> map,
                                            int width, int height ,
                                            std::pair<double, double> start,
                                            std::pair<double, double> end
                                            );
#endif // LEE_H
