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
