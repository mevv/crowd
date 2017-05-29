#include <lee.h>

std::vector<std::pair<double, double> > Lee(std::vector<double> map,
                                            int width, int height ,
                                            std::pair<double, double> start,
                                            std::pair<double, double> end)
{
    int * intMap = new int [width*height];
    cout << "map: " << endl;
    cout << start.first << " " << start.second << endl;
    int count = 0;
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            //cout << i << "|" << j << "|" << map[count] << "|";
            if((int)map[count] == 1)
                intMap[count] = 0;
            if(map[count] == 9)
                intMap[count] = 1;
            if(j == start.first && i == start.second)
                intMap[count] = 3;
            if(j == end.first && i == end.second)
                intMap[count] = 2;
            //cout << intMap[count] << " ";
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

    delete[] intMap;

    return path;
}
