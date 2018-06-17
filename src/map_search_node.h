#ifndef MAP_SEARCH_NODE_H
#define MAP_SEARCH_NODE_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "map_container.h"
#include "stlastar.h"

std::vector< std::pair<double, double> > Astar(std::vector<double> map, int width, int height ,std::pair<double, double> start, std::pair<double, double> end);


class MapSearchNode
{
public:
    static MapContainer map_container;
    double x;	 // the (x,y) positions of the node
    double y;

    MapSearchNode()
    {
        x = y = 0;
        //MapContainer map;
    }
    MapSearchNode( int px, int py ) { x=px; y=py; }

    float GoalDistanceEstimate( MapSearchNode &nodeGoal );
    bool IsGoal( MapSearchNode &nodeGoal );
    bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
    float GetCost( MapSearchNode &successor );
    bool IsSameState( MapSearchNode &rhs );

    void PrintNodeInfo();


};


//int main( int argc, char *argv[] )
//{
//    //1 - FREE, 9 - OBSTACLE
//    std::vector<double> map = {
//    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // 00
//    1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,1,   // 01
//    1,9,9,1,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 02
//    1,9,9,1,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 03
//    1,9,1,1,1,1,9,9,1,9,1,9,1,1,1,1,9,9,1,1,   // 04
//    1,9,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,1,   // 05
//    1,9,9,9,9,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,   // 06
//    1,9,9,9,9,9,9,9,9,1,1,1,9,9,9,9,9,9,9,1,   // 07
//    1,9,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,   // 08
//    1,9,1,9,9,9,9,9,9,9,1,1,9,9,9,9,9,9,9,1,   // 09
//    1,9,1,1,1,1,9,1,1,9,1,1,1,1,1,1,1,1,1,1,   // 10
//    1,9,9,9,9,9,1,9,1,9,1,9,9,9,9,9,1,1,1,1,   // 11
//    1,9,1,9,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 12
//    1,9,1,9,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 13
//    1,9,1,1,1,1,9,9,1,9,1,9,1,1,1,1,9,9,1,1,   // 14
//    1,9,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,1,   // 15
//    1,9,9,9,9,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,   // 16
//    1,1,9,9,9,9,9,9,9,1,1,1,9,9,9,1,9,9,9,9,   // 17
//    1,9,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,   // 18
//    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

//    std::vector< std::pair<double, double> > res = Astar(map, 20, 20, std::make_pair(0, 0), std::make_pair(10, 9));

//    return 0;
//}



#endif // MAP_SEARCH_NODE_H
