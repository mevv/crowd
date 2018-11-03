#ifndef A_STAR_STEP_H
#define A_STAR_STEP_H

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

#include <iostream>

namespace ASTAR {

using Cell = std::pair<size_t, size_t>;

template<typename T, typename priority_t>
struct PriorityQueue
{
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

    inline bool empty() const { return elements.empty(); }
    inline void push(T item, priority_t priority) { elements.emplace(priority, item); }
    inline T top() { return elements.top().second; }
    inline priority_t top_priority() { return elements.top().first; }

    T pop()
    {
        T best_item = elements.top().second;
        elements.pop();

        return best_item;
    }
};

struct Map
{
    explicit Map(const std::vector<double>& m, size_t h, size_t w) :
        map(m),
        height(h),
        width(w)
    {}

    double get(size_t r, size_t c) const;
    std::vector<Cell> neighbors(const Cell& cell);
    double cost(const Cell& cell);

    std::vector<double> map;
    size_t height;
    size_t width;
};

class AStar
{
public:
    AStar() : m_map(std::vector<double>(), 0, 0) {}

    explicit AStar(std::vector<double> map, size_t height, size_t width, Cell start, Cell end) :
        m_map(map, height, width),
        m_start(start),
        m_end(end)
    {}

    Map getMap() { return m_map; }

    void setStart(const Cell& s) { m_start = s; reset(); }
    void setEnd(const Cell& e) { m_end = e; }
    void setMap(const Map& m) { m_map = m; }
    void setMap(std::vector<double> map, size_t height, size_t width) { m_map = Map(map, height, width); }

    std::vector<Cell> findPath();

private:
    Map m_map;
    Cell m_start;
    Cell m_end;

    PriorityQueue<Cell, double> m_frontier;
    std::map<Cell, double> m_cost;
    std::map<Cell, Cell> m_cameFrom;

    void reset();
    bool step();

    void printMap(std::vector<Cell> path);
};

}

#endif // A_STAR_STEP_H
