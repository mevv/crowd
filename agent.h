#ifndef AGENT_H
#define AGENT_H

#include <QVector2D>

#include "scene_object.h"

class Agent : public SceneObject
{
public:
    Agent(int id, double size, double mass, QVector2D position, QVector2D speed, QVector2D acceleration);

    inline double getSize() const { return m_size; }

    inline QVector2D getSpeed() const { return m_speed; }

    void setSpeed(const QVector2D speed) { m_speed = speed; }

private:
    int m_size;
    double m_mass;
    QVector2D m_speed;
    QVector2D m_acceleration;
};

#endif // AGENT_H
