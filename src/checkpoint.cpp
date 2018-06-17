#include "checkpoint.h"

Checkpoint::Checkpoint(int id, QVector2D pos, QColor color, double radius) :
    SceneObject(id, pos, color),
    m_radius(radius)
{

}

