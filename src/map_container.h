#ifndef MAP_CONTAINER_H
#define MAP_CONTAINER_H


class MapContainer
{
public:
    int MAP_WIDTH;
    int MAP_HEIGHT;
    std::vector<double> world_map;

    int GetMap( int x, int y )
    {
        if(x < 0 ||x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        {
            return 9;
        }

        return world_map[(y*MAP_WIDTH)+x];
    }
};


#endif // MAP_CONTAINER_H
