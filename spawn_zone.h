#ifndef SPAWNZONE_H
#define SPAWNZONE_H

#include "scene_object.h"

const double MIN_DISTANCE_BETWEEN_SPAWN_AGENTS = 0;

class SpawnZone : public SceneObject
{
public:
    SpawnZone(int id, QVector2D pos, QColor color, QPoint size);

    inline QPoint getSize() const { return m_size; }


    void setNumAgentsToSpawn(int num, double agentSize);

private:
    QPoint m_size;
    int m_numAgentsToSpawn = 0;
};

#endif // SPAWNZONE_H
