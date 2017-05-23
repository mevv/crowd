#include "checkpoint.h"

Checkpoint::Checkpoint(int id, QVector2D pos, QColor color, double radius) :
    SceneObject(id, pos, QColor()),
    m_radius(radius)
{

}

