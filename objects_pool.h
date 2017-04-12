#ifndef OBJECTSPOOL_H
#define OBJECTSPOOL_H

#include <vector>

#include "agent.h"
#include "obstacle.h"

class ObjectsPool
{
public:
    ObjectsPool();

    ~ObjectsPool();

    inline std::vector<Agent>& getAgents() { return m_agents; }

    inline std::vector<Obstacle>& getObstacles() { return m_obstacles; }

    void addAgent(const Agent& agent) { m_agents.push_back(agent); }

    void addObstacle(const Obstacle& obstacle) { m_obstacles.push_back(obstacle); }

private:
    std::vector<Agent> m_agents;
    std::vector<Obstacle> m_obstacles;
};

#endif // OBJECTSPOOL_H
