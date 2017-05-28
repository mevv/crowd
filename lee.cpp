#include <lee.h>

std::vector<std::pair<double, double> > Lee(std::vector<double> map,
                                            int width, int height ,
                                            std::pair<double, double> start,
                                            std::pair<double, double> end
                                            )
{
    std::vector<std::pair<double,double> > path;
    int W      = width;         // ширина рабочего поля
    int H      = height;         // высота рабочего поля
    const int WALL   = 9;         // непроходимая ячейка
    const int BLANK  = 1;         // свободная непомеченная ячейка

    int ax = start.first;
    int ay = start.second;
    int bx = end.first;
    int by = end.second;

    int px[W * H], py[W * H];      // координаты ячеек, входящих в путь
    int len;                       // длина пути
    int grid[H][W];                // рабочее поле

    for(int i = 0; i < width; i++)
        for(int j = 0; j < height; j++)
            grid[i][j] = map[i+j];

    // Перед вызовом lee() массив grid заполнен значениями WALL и BLANK

    int dx[4] = {1, 0, -1, 0};   // смещения, соответствующие соседям ячейки
    int dy[4] = {0, 1, 0, -1};   // справа, снизу, слева и сверху
    int d, x, y, k;
    bool stop;

    if (grid[ay][ax] == WALL || grid[by][bx] == WALL) {
        qDebug() << "Search terminated. Did not find goal state\n";
        return path;
    }

    // распространение волны
    d = 0;
    grid[ay][ax] = 0;            // стартовая ячейка помечена 0
    do {
      stop = true;               // предполагаем, что все свободные клетки уже помечены
      for ( y = 0; y < H; ++y )
        for ( x = 0; x < W; ++x )
          if ( grid[y][x] == d )                         // ячейка (x, y) помечена числом d
          {
            for ( k = 0; k < 4; ++k )                    // проходим по всем непомеченным соседям
            {
               int iy=y + dy[k], ix = x + dx[k];
               if ( iy >= 0 && iy < H && ix >= 0 && ix < W &&
                    grid[iy][ix] == BLANK )
               {
                  stop = false;              // найдены непомеченные клетки
                  grid[iy][ix] = d + 1;      // распространяем волну
               }
            }
          }
      d++;
    } while ( !stop && grid[by][bx] == BLANK );

    if (grid[by][bx] == BLANK) {
        qDebug() << "Search terminated. Did not find goal state\n";
        return path;
    }

    // восстановление пути
    len = grid[by][bx];            // длина кратчайшего пути из (ax, ay) в (bx, by)
    x = bx;
    y = by;
    d = len;
    while ( d > 0 )
    {
      px[d] = x;
      py[d] = y;                   // записываем ячейку (x, y) в путь
      path[d].first = x;
      path[d].second = y;
      d--;
      for (k = 0; k < 4; ++k)
      {
         int iy=y + dy[k], ix = x + dx[k];
         if ( iy >= 0 && iy < H && ix >= 0 && ix < W &&
              grid[iy][ix] == d)
        {
            x = x + dx[k];
            y = y + dy[k];           // переходим в ячейку, которая на 1 ближе к старту
            break;
        }
      }
    }
    px[0] = ax;
    py[0] = ay;                    // теперь px[0..len] и py[0..len] - координаты ячеек пути
    path[0].first = ax;
    path[0].second = ay;
    return path;
}
