#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "scene_object.h"

class Checkpoint : public SceneObject
{
public:
    Checkpoint(int id, QVector2D pos, QColor color, double radius);

    double getRadius() const { return m_radius; }

private:
    double m_radius;
};

#endif // CHECKPOINT_H
