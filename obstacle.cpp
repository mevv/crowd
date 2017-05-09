#include "obstacle.h"

Obstacle::Obstacle(int id, QVector2D position, QColor color, QVector<QPoint> points) :
    SceneObject(id, position, color),
    m_points(points)
{

}

QVector<QPoint> Obstacle::getAbsolutePoints() const
{
    QVector<QPoint> data;
    data.push_back(this->getPos().toPoint());
    for(auto i : getPoints())
        data.push_back(i + this->getPos().toPoint());
    return data;
}
