#ifndef AGENT_H
#define AGENT_H

#include <QVector2D>

#include "scene_object.h"

class Agent : public SceneObject
{
public:
    Agent(int id, double size, double mass, QVector2D position, QVector2D speed, QVector2D acceleration, QColor color);

    inline double getSize() const { return m_size; }

    inline QVector2D getSpeed() const { return m_speed; }

    inline double getMass() const { return m_mass; }

    void setSpeed(const QVector2D speed) { m_speed = speed; }

    QColor getColor() {return this->m_color; }

    void setPrevPos(){ m_prevPos = this->getPos(); }

    inline QVector2D getPrevPos(){ return m_prevPos; }

    bool operator==(const Agent& a) const
    {
        if(this->getID() == a.getID())
            return true;
        return false;
    }

    QVector2D getCenter() const {return QVector2D(this->getPos().x() +  this->getSize(),
                                            this->getPos().y() +  this->getSize());};

private:
    int m_size;
    double m_mass;
    QVector2D m_speed;
    QVector2D m_prevPos;
    QVector2D m_acceleration;
    QColor m_color;
};

#endif // AGENT_H
