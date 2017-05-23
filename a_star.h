#ifndef A_STAR_H
#define A_STAR_H

#include "stlastar.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>

#include <QDebug>


// Definitions
class MapSearchNode
{
public:


    static int MAP_WIDTH;
    static int MAP_HEIGHT;
    static std::vector<double> world_map;

    // map helper functions
    static int GetMap( int x, int y )
    {
        if(x < 0 ||x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        {
            return 9;
        }

        return world_map[(y*MAP_WIDTH)+x];
    }


    double x;	 // the (x,y) positions of the node
    double y;

    MapSearchNode() { x = y = 0; }
    MapSearchNode( int px, int py ) { x=px; y=py; }

    float GoalDistanceEstimate( MapSearchNode &nodeGoal )
    {
        return fabsf(x - nodeGoal.x) + fabsf(y - nodeGoal.y);
    }

    bool IsGoal( MapSearchNode &nodeGoal )
    {

        if( (x == nodeGoal.x) &&
            (y == nodeGoal.y) )
        {
            return true;
        }

        return false;
    }

    bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node )
    {

        int parent_x = -1;
        int parent_y = -1;

        if( parent_node )
        {
            parent_x = parent_node->x;
            parent_y = parent_node->y;
        }


        MapSearchNode NewNode;

        // push each possible move except allowing the search to go backwards

        if( (MapSearchNode::GetMap( x-1, y ) < 9)
            && !((parent_x == x-1) && (parent_y == y))
          )
        {
            NewNode = MapSearchNode( x-1, y );
            astarsearch->AddSuccessor( NewNode );
        }

        if( (MapSearchNode::GetMap( x, y-1 ) < 9)
            && !((parent_x == x) && (parent_y == y-1))
          )
        {
            NewNode = MapSearchNode( x, y-1 );
            astarsearch->AddSuccessor( NewNode );
        }

        if( (MapSearchNode::GetMap( x+1, y ) < 9)
            && !((parent_x == x+1) && (parent_y == y))
          )
        {
            NewNode = MapSearchNode( x+1, y );
            astarsearch->AddSuccessor( NewNode );
        }


        if( (MapSearchNode::GetMap( x, y+1 ) < 9)
            && !((parent_x == x) && (parent_y == y+1))
            )
        {
            NewNode = MapSearchNode( x, y+1 );
            astarsearch->AddSuccessor( NewNode );
        }

        return true;
    }


    float GetCost( MapSearchNode &successor )
    {
        return (float) MapSearchNode::GetMap( x, y );
    }

    bool IsSameState( MapSearchNode &rhs )
    {
        if( (x == rhs.x) &&
            (y == rhs.y) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void PrintNodeInfo()
    {
        qDebug() << "Node position : (" << x << ", " << y << ")";
    }

    //1 - FREE, 9 - OBSTACLE
    static std::vector< std::pair<double, double> > Astar(std::vector<double> map, int width, int height ,std::pair<double, double> start, std::pair<double, double> end)
    {
        AStarSearch<MapSearchNode> astarsearch;
        unsigned int SearchCount = 0;
        const unsigned int NumSearches = 1;

        std::vector< std::pair<double, double> > result;

        MapSearchNode::MAP_WIDTH = width;
        MapSearchNode::MAP_HEIGHT = height;
        MapSearchNode::world_map = map;

        while(SearchCount < NumSearches)
        {
            // Create a start state
            MapSearchNode nodeStart;
            nodeStart.x = start.first;
            nodeStart.y = start.second;

            // Define the goal state
            MapSearchNode nodeEnd;
            nodeEnd.x = end.first;
            nodeEnd.y = end.second;

            // Set Start and goal states

            astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

            unsigned int SearchState;
            unsigned int SearchSteps = 0;

            do
            {
                SearchState = astarsearch.SearchStep();
                SearchSteps++;
            }
            while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );

            if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED )
            {
                cout << "Search found goal state\n";

                MapSearchNode *node = astarsearch.GetSolutionStart();

                int steps = 0;

                node->PrintNodeInfo();
                for( ;; )
                {
                    node = astarsearch.GetSolutionNext();



                    if( !node )
                    {
                        break;
                    }

                    result.push_back(std::make_pair(node->x, node->y));

                    node->PrintNodeInfo();
                    steps ++;
                };

                cout << "Solution steps " << steps << endl;

                // Once you're done with the solution you can free the nodes up
                astarsearch.FreeSolutionNodes();
            }
            else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED )
            {
                cout << "Search terminated. Did not find goal state\n";

            }

            // Display the number of loops the search went through
            cout << "SearchSteps : " << SearchSteps << "\n";

            SearchCount ++;

            astarsearch.EnsureMemoryFreed();
        }

        return result;
    }

};


/*
int main( int argc, char *argv[] )
{
    //1 - FREE, 9 - OBSTACLE
    std::vector<double> map = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // 00
    1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,1,   // 01
    1,9,9,1,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 02
    1,9,9,1,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 03
    1,9,1,1,1,1,9,9,1,9,1,9,1,1,1,1,9,9,1,1,   // 04
    1,9,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,1,   // 05
    1,9,9,9,9,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,   // 06
    1,9,9,9,9,9,9,9,9,1,1,1,9,9,9,9,9,9,9,1,   // 07
    1,9,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,   // 08
    1,9,1,9,9,9,9,9,9,9,1,1,9,9,9,9,9,9,9,1,   // 09
    1,9,1,1,1,1,9,1,1,9,1,1,1,1,1,1,1,1,1,1,   // 10
    1,9,9,9,9,9,1,9,1,9,1,9,9,9,9,9,1,1,1,1,   // 11
    1,9,1,9,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 12
    1,9,1,9,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 13
    1,9,1,1,1,1,9,9,1,9,1,9,1,1,1,1,9,9,1,1,   // 14
    1,9,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,1,   // 15
    1,9,9,9,9,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,   // 16
    1,1,9,9,9,9,9,9,9,1,1,1,9,9,9,1,9,9,9,9,   // 17
    1,9,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,   // 18
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    std::vector< std::pair<double, double> > res = Astar(map, 20, 20, std::make_pair(0, 0), std::make_pair(10, 9));

    return 0;
}
*/

#endif
