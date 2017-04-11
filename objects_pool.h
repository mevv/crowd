#ifndef OBJECTSPOOL_H
#define OBJECTSPOOL_H

#include <vector>

#include "agent.h"

class ObjectsPool
{
public:
    ObjectsPool();

    ~ObjectsPool();

    inline std::vector<Agent>& getAgents() { return m_agents; }

    void addAgent(const Agent& agent) { m_agents.push_back(agent); }

private:
    std::vector<Agent> m_agents;
};

#endif // OBJECTSPOOL_H
