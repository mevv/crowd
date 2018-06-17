#include "a_star.h"


std::vector< std::pair<double, double> > Astar(std::vector<double> map, int width, int height ,std::pair<double, double> start, std::pair<double, double> end)
{
    AStarSearch<MapSearchNode> astarsearch;
    unsigned int SearchCount = 0;
    const unsigned int NumSearches = 1;

    std::vector< std::pair<double, double> > result;

    MapSearchNode::map_container.MAP_WIDTH = width;
    MapSearchNode::map_container.MAP_HEIGHT = height;
    MapSearchNode::map_container.world_map = map;

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
//            cout << "Search found goal state\n";

            MapSearchNode *node = astarsearch.GetSolutionStart();

            int steps = 0;

//            node->PrintNodeInfo();
            for( ;; )
            {
                node = astarsearch.GetSolutionNext();



                if( !node )
                {
                    break;
                }

                result.push_back(std::make_pair(node->x, node->y));

//                node->PrintNodeInfo();
                steps ++;
            };

//            cout << "Solution steps " << steps << endl;

            // Once you're done with the solution you can free the nodes up
            astarsearch.FreeSolutionNodes();
        }
        else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED )
        {
            cout << "Search terminated. Did not find goal state\n";

        }

        // Display the number of loops the search went through
//        cout << "SearchSteps : " << SearchSteps << "\n";

        SearchCount ++;

        astarsearch.EnsureMemoryFreed();
    }

    return result;
}
