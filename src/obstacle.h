#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QVector>
#include <QPoint>

#include "scene_object.h"

class Obstacle : public SceneObject
{
public:
    Obstacle(int id, QVector2D position, QColor color, QVector<QVector2D> points);

    QVector<QVector2D> getPoints() const { return m_points; }
    QVector<QVector2D> getAbsolutePoints() const;

    void print();
private:
    QVector<QVector2D> m_points;//up left point (0, 0)
};

#endif // OBSTACLE_H
