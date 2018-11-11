#include "obstacle.h"

#include <QDebug>

Obstacle::Obstacle(int id, QVector2D position, QColor color, QVector<QVector2D> points) :
    SceneObject(id, position, color),
    m_points(points)
{

}

QVector<QVector2D> Obstacle::getAbsolutePoints() const
{
    QVector<QVector2D> data;
    //data.push_back(this->getPos());
    for(auto i : getPoints())
        data.push_back(i + this->getPos());
    return data;
}

void Obstacle::print()
{
    qDebug() << ">>>obstacle<<<";
    for (auto i : getAbsolutePoints())
        qDebug() << i.x() << i.y();
    qDebug() << ">>>obstacle<<<";
}
