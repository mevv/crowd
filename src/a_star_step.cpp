#include "a_star_step.h"

#include <QDebug>

#include <cmath>
#include <limits>
#include <map>

using namespace ASTAR;
using Cell = std::pair<size_t, size_t>;

inline double heuristic(const Cell& a, const Cell& b)
{
    return std::max(a.first, b.first) - std::min(a.first, b.first) + std::max(a.second, b.second) - std::min(a.second, b.second);
}

std::vector<Cell> reconstructPath(std::map<Cell, Cell> came_from, Cell start, Cell goal)
{
  std::vector<Cell> path;
  Cell current = goal;

  if (came_from.empty())
      return path;

  while (current != start)
  {
    path.push_back(current);
    if (current == came_from[current])
        break;
    current = came_from[current];
  }

  path.push_back(start);

  std::reverse(path.begin(), path.end());

  return path;
}

double Map::get(size_t r, size_t c) const
{
    if (r < height && c < width)
        return map[r * width + c];

    return std::numeric_limits<double>::infinity();
}

std::vector<Cell> Map::neighbors(const Cell& cell)
{
    std::vector<Cell> res;

    res.emplace_back(cell.first + 1, cell.second);
    res.emplace_back(cell.first, cell.second + 1);
    res.emplace_back(cell.first - 1, cell.second);
    res.emplace_back(cell.first, cell.second - 1);

    res.emplace_back(cell.first + 1, cell.second + 1);
    res.emplace_back(cell.first - 1, cell.second + 1);
    res.emplace_back(cell.first + 1, cell.second - 1);
    res.emplace_back(cell.first - 1, cell.second - 1);

    return res;
}

double Map::cost(const Cell& cell)
{
    return get(cell.first, cell.second);
}

std::vector<Cell> AStar::findPath()
{
    reset();

    while (step()) {}

    //printMap(cost, reconstructPath(cameFrom, m_start, m_end));

    return reconstructPath(m_cameFrom, m_start, m_end);
}

void AStar::reset()
{
    m_frontier.push(m_start, 0);
    m_cameFrom[m_start] = m_start;
    m_cost[m_start] = 0;
}

bool AStar::step()
{
    if (m_frontier.empty())
        return false;

    if (m_frontier.top() == m_end)
        return false;

    if (std::isinf(m_frontier.top_priority()))
        return false;

    Cell current = m_frontier.pop();

    for (Cell next : m_map.neighbors(current))
    {
        double new_cost = m_cost[current] + m_map.cost(next);
        if (m_cost.find(next) == m_cost.end() || new_cost < m_cost[next])
        {
            m_cost[next] = new_cost;
            double priority = new_cost + heuristic(next, m_end);
            m_cameFrom[next] = current;
            m_frontier.push(next, priority);
        }
    }

    return true;
}

void AStar::printMap(std::vector<Cell> path)
{
    std::cout << "(" << m_start.first << ", " << m_start.second << ")" << std::endl;
    std::cout << "(" << m_end.first << ", " << m_end.second << ")" << std::endl;

    for (size_t i = 0; i < m_map.height; i++)
    {
        for (size_t j = 0; j < m_map.width; j++)
            if (std::find(path.begin(), path.end(), Cell(i, j)) != path.end())
              std::cout << "* ";
            else
                std::cout << m_map.get(i, j) << " ";
            //std::cout << m_cost[Cell(i, j)] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
