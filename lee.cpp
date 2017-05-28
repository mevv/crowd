#include <lee.h>

std::vector<std::pair<double, double> > Lee(std::vector<double> map,
                                            int width, int height ,
                                            std::pair<double, double> start,
                                            std::pair<double, double> end)
{
    int * intMap = new int [width*height];
    cout << "map: " << endl;
    int count = 0;
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if(map[count] == 1.0)
                intMap[count] == 0;
            if(map[count] == 9.0)
                intMap[count] == 1;
            if(i == start.first && j == start.second)
                intMap[count] == 2;
            if(i == end.first && j == end.second)
                intMap[count] == 3;
            cout << intMap[count] << " ";
            count++;
        }
        cout << endl;
    }


    LeePathfinder c(intMap, width, height);
    auto intPath =  c.CalculatePath();
//    c.PrintMap();
    std::vector<std::pair<double,double> > path;
    for(auto i : *intPath)
        path.push_back(i);
    return path;
}
