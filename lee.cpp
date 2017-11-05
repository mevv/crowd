#include <lee.h>

std::vector<std::pair<double, double> > Lee(std::vector<double> map,
                                            int width, int height ,
                                            std::pair<double, double> start,
                                            std::pair<double, double> end)
{
    int * intMap = new int [max(width, height) * max(width, height)];
//    cout << "map: " << endl;
//    cout << "start:" << start.first << " " << start.second << endl;
//    cout << "end:" << end.first << " " << end.second << endl;
    int count = 0;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            //cout << map[count] << "|";
            if(map[i*width+j] == 1)
                intMap[count] = 0;
            if(map[i*width+j] == 9)
                intMap[count] = 1;
            if(j == start.first && i == start.second)
                intMap[count] = 3;
            if(j == end.first && i == end.second)
                intMap[count] = 2;
            //cout << intMap[count] << " ";
            count++;
        }
        //cout << endl;

        if (height > width)
        {
            for (int k = 0; k < height - width; k++)
            {
                intMap[count] = 1;
                count++;
            }
        }
    }

    if (height > width)
        width = height;

    if (width > height)
    {
        for (int i = count; i < width * width; i++)
            intMap[i] = 1;
        height = width;
    }

//    count = 0;
//    for(int i = 0; i < height; i++)
//    {
//        for(int j = 0; j < width; j++)
//        {
//            cout << intMap[count] << " ";
//            count++;
//        }
//        cout << endl;
//    }

    LeePathfinder c(intMap, width, height);
    auto intPath =  c.CalculatePath();
    c.PrintMap();
    std::vector<std::pair<double,double> > path;
    for(auto i : *intPath)
        path.push_back(i);

    //delete[] intMap;

    return path;
}
