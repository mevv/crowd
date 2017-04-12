#include "obstacle.h"

Obstacle::Obstacle(int id, QVector2D position, QColor color, QVector<QPoint> points) :
    SceneObject(id, position, color),
    m_points(points)
{

}

