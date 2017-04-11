#include "agent.h"

Agent::Agent(int id, double size, double mass, QVector2D position, QVector2D speed, QVector2D acceleration) :
    SceneObject(id, position),
    m_size(size),
    m_mass(mass),
    m_speed(speed),
    m_acceleration(acceleration)
{

}

