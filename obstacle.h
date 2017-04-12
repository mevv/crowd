#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QVector>
#include <QPoint>

#include "scene_object.h"

class Obstacle : public SceneObject
{
public:
    Obstacle(int id, QVector2D position, QColor color, QVector<QPoint> points);

    inline QVector<QPoint> getPoints() const { return m_points; }

private:
    QVector<QPoint> m_points;//up left point (0, 0)
};

#endif // OBSTACLE_H
