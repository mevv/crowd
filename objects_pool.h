#ifndef OBJECTSPOOL_H
#define OBJECTSPOOL_H

#include <vector>

#include "agent.h"
#include "obstacle.h"
#include "exit.h"

class ObjectsPool
{
public:
    ObjectsPool();

    ~ObjectsPool();

    inline std::vector<Agent>& getAgents() { return m_agents; }

    inline std::vector<Obstacle>& getObstacles() { return m_obstacles; }

    inline std::vector<Exit>& getExits() { return m_exits; }

    void addAgent(const Agent& agent) { m_agents.push_back(agent); }

    void addObstacle(const Obstacle& obstacle) { m_obstacles.push_back(obstacle); }

    void addExit(const Exit& exit) { m_exits.push_back(exit); }

private:
    std::vector<Agent> m_agents;
    std::vector<Obstacle> m_obstacles;
    std::vector<Exit> m_exits;
};

#endif // OBJECTSPOOL_H
