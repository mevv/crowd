#include "scene.h"

#include <QDebug>

const double width = 0.2;

Scene::Scene(QVector2D size,std::shared_ptr<ObjectsPool> pool) :
    m_size(size),
    m_pool(pool)
{
    m_arrow = {QPoint(0, 0), QPoint(2, -2), QPoint(2, -1), QPoint(4, -1), QPoint(4, 1), QPoint(2, 1), QPoint(2, 2)};
}

void Scene::draw(QPainter&  painter)
{
    //filling
    painter.setBrush(Qt::white);
    painter.drawRect(m_pos.x(), m_pos.y(), m_size.x() * m_scale, m_size.y() * m_scale);

    //draw background
    painter.setBrush(Qt::gray);
    painter.drawRect(m_pos.x(), m_pos.y(), m_size.x() * m_scale, m_size.y() * m_scale);

    //draw checkpoints
    if (m_drawPath)
    {
        for (auto i : m_pool->getAgents())
        {
            for (auto j : m_pool->getCheckpoints()[i.getID()])
            {
                painter.setBrush(QBrush(j.getColor(), Qt::BrushStyle::SolidPattern));
                painter.drawEllipse(j.getPos().x() * m_scale + m_pos.x(), j.getPos().y() * m_scale + m_pos.y(), j.getRadius() * m_scale, j.getRadius() * m_scale);
            }
        }
    }

    //draw agents
    painter.setBrush(Qt::black);
    for (auto i : m_pool->getAgents())
    {
        painter.setBrush(QBrush(i.getColor(),Qt::BrushStyle::SolidPattern));
        painter.drawEllipse(i.getPos().x() * m_scale + m_pos.x(), i.getPos().y() * m_scale + m_pos.y(), i.getSize() * m_scale, i.getSize() * m_scale);
    }

    //draw obstacles
    for (auto i : m_pool->getObstacles())
    {
        QPainterPath path;

        path.moveTo(i.getPos().x() * m_scale + m_pos.x(), i.getPos().y() * m_scale + m_pos.y());

        for (auto j : i.getPoints())
            path.lineTo((j.x() + i.getPos().x()) * m_scale + m_pos.x(), (j.y() + i.getPos().y()) * m_scale + m_pos.y());

        path.closeSubpath();

        painter.setBrush(QBrush(i.getColor(), Qt::BrushStyle::SolidPattern));
        painter.drawPath(path);
    }

    QPen pen;
    //draw exits
    for (auto i : m_pool->getExits())
    {
        auto curArrow = this->scalePolygon(m_arrow, (i.getEnd() - i.getPos()).length() / 4.0 );
        curArrow = this->movePolygonTo(curArrow, i.getCenter());

        pen.setColor(i.getColor());
        pen.setWidth(width * m_scale);

        painter.setPen(pen);
        painter.drawLine(i.getPos().toPointF() * m_scale + m_pos, i.getEnd().toPointF() * m_scale  + m_pos);
    }

    //draw entries
    for (auto i : m_pool->getEntries())
    {
        pen.setColor(i.getColor());
        pen.setWidth(width * m_scale);

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


