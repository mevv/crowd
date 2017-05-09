#include "objects_pool.h"
#include <QDebug>

ObjectsPool::ObjectsPool()
{

}

ObjectsPool::~ObjectsPool()
{
    qDebug() << "~ObjectsPool()";
    m_agents.clear();
}

void ObjectsPool::removeAgent(Agent &agent)
{
   auto iter =  m_agents.erase(std::remove(m_agents.begin(), m_agents.end(), agent), m_agents.end());
    if(m_agents.size() + deleted != 2000)
    {
        qDebug() << "DELETED: " << ++deleted;
    }
    if(m_agents.size() == 0)
        emit endOfSimulation();
//    return iter;
    //TODO
}
