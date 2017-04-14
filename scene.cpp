#include "scene.h"

#include <QDebug>


Scene::Scene(QPoint size,std::shared_ptr<ObjectsPool> pool) :
    m_size(size),
    m_pool(pool)
{
    qDebug() << "Scene count" << m_pool.use_count();
}

void Scene::draw(QPainter&  painter)
{
    //draw agents
    auto brush = new QBrush();
    brush->setStyle(Qt::SolidPattern);
    for (auto i : m_pool->getAgents())
    {
        brush->setColor(i.getColor());
//        painter.setBackground(*brush);
        painter.setBrush(*brush);
        painter.drawEllipse(i.getPos().x() * m_scale, i.getPos().y() * m_scale, i.getSize() * m_scale, i.getSize() * m_scale);
    }

    //draw obstacles
    for (auto i : m_pool->getObstacles())
    {
        QPainterPath path;

        path.moveTo(i.getPos().x() * m_scale, i.getPos().y() * m_scale);

        for (auto j : i.getPoints())
            path.lineTo((j.x() + i.getPos().x()) * m_scale, (j.y() + i.getPos().y()) * m_scale);

        path.closeSubpath();

        painter.drawPath(path);
    }
}
