#include "objects_pool.h"
#include <QDebug>

ObjectsPool::ObjectsPool()
{

}

ObjectsPool::~ObjectsPool()
{
    m_agents.clear();
}

std::vector<Agent>::iterator ObjectsPool::removeAgent(const Agent &agent)
{
    auto it =  m_agents.erase(std::remove(m_agents.begin(), m_agents.end(), agent), m_agents.end());

    ++deleted;

    if(m_agents.size() == 0)
        emit endOfSimulation();

    return it;
}

void ObjectsPool::clear()
{
    m_agents.clear();
    m_obstacles.clear();
    m_exits.clear();
    m_entries.clear();
    m_spawnZones.clear();
}
