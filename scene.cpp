#include "scene.h"

#include <QDebug>


Scene::Scene(QPoint size,std::shared_ptr<ObjectsPool> pool) :
    m_size(size),
    m_pool(pool)
{
    m_arrow = {QPoint(0, 0), QPoint(2, -2), QPoint(2, -1), QPoint(4, -1), QPoint(4, 1), QPoint(2, 1), QPoint(2, 2)};
}

void Scene::draw(QPainter&  painter)
{
    //much shorter, but so fucking slow!
    //    QTransform transform;
    //    transform.scale(m_scale, m_scale);
    //    painter.setTransform(transform);

    //    painter.scale(m_scale, m_scale);

    //draw background
    painter.setBrush(Qt::gray);
    painter.drawRect(m_pos.x(), m_pos.y(), m_size.x() * m_scale, m_size.y() * m_scale);

    //draw agents
    painter.setBrush(Qt::black);
    for (auto i : m_pool->getAgents())
    {
        //qDebug() << i.getID() << i.getPos() << i.getPos().x() * m_scale + m_pos.x() << i.getPos().y() * m_scale + m_pos.y() << i.getSize() * m_scale;
        //brush->setColor(i.getColor());
        //painter.setBrush(*brush);
        painter.drawEllipse(i.getPos().x() * m_scale + m_pos.x(), i.getPos().y() * m_scale + m_pos.y(), i.getSize() * m_scale, i.getSize() * m_scale);
    }

    //draw obstacles
    for (auto i : m_pool->getObstacles())
    {
        QPainterPath path;

        path.moveTo(i.getPos().x() * m_scale + m_pos.x(), i.getPos().y() * m_scale + m_pos.y());

        for (auto j : i.getPoints())
        {
            //qDebug() << j;
            path.lineTo((j.x() + i.getPos().x()) * m_scale + m_pos.x(), (j.y() + i.getPos().y()) * m_scale + m_pos.y());
        }

        path.closeSubpath();

        painter.drawPath(path);
    }

    //draw exits
    for (auto i : m_pool->getExits())
    {
        //qDebug() << (i.getEnd() - i.getPos()).length();
        auto curArrow = this->scalePolygon(m_arrow, (i.getEnd() - i.getPos()).length() / 4.0 );
        curArrow = this->movePolygonTo(curArrow, i.getCenter());


//        QPainterPath path;

//        path.moveTo(curArrow[0].x() * m_scale + m_pos.x(), curArrow[0].y() * m_scale + m_pos.y());

//        for (auto j : curArrow)
//        {
//            path.lineTo(j.x() * m_scale + m_pos.x(), j.y() * m_scale + m_pos.y());
//        }

//        path.closeSubpath();

//        painter.drawPath(path);

        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(3);

        painter.setPen(pen);

        painter.drawLine(i.getPos().toPointF() * m_scale + m_pos, i.getEnd().toPointF() * m_scale  + m_pos);
    }
}

//get out this methods from here, need another entity for it
std::vector<QPoint> Scene::movePolygonTo(const std::vector<QPoint>& polygon, QPoint place)
{
    std::vector<QPoint> result;

    for (auto i : polygon)
        result.push_back(QPoint(i.x() + place.x(), i.y() + place.y()));

    return result;
}

std::vector<QPoint> Scene::scalePolygon(const std::vector<QPoint>& polygon, double scale)
{
    std::vector<QPoint> result;

    for (auto i : polygon)
        result.push_back(QPoint(i.x() * scale, i.y() * scale));

    return result;
}


