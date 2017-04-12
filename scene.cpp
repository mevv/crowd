#include "scene.h"

#include <cmath>

#include <QDebug>


Scene::Scene(std::shared_ptr<ObjectsPool> pool) :
    m_pool(pool)
{
    qDebug() << "Scene count" << m_pool.use_count();
}

void Scene::draw(QPainter&  painter)
{
    for (auto i : m_pool->getAgents())
        painter.drawEllipse(round(i.getPos().x() * m_scale), round(i.getPos().y() * m_scale), i.getSize() * m_scale, i.getSize() * m_scale);

    for (auto i : m_pool->getObstacles())
    {
        QPainterPath path;

        path.moveTo(i.getPos().x(), i.getPos().y());

        for (auto j : i.getPoints())
            path.lineTo(j.x() + i.getPos().x(), j.y() + i.getPos().y());

        path.closeSubpath();

        painter.drawPath(path);
    }
}
