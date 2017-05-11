#include "spawn_zone.h"

SpawnZone::SpawnZone(int id, QVector2D pos, QColor color, QPoint size) :
    SceneObject(id, pos, color),
    m_size(size)
{

}

void SpawnZone::setNumAgentsToSpawn(int num, double agentSize)
{
    int maxAgentsNum = (m_size.x() / (agentSize + 2 * MIN_DISTANCE_BETWEEN_SPAWN_AGENTS)) * (m_size.x() / (agentSize + 2 * MIN_DISTANCE_BETWEEN_SPAWN_AGENTS));

    if (maxAgentsNum > num)
        m_numAgentsToSpawn = maxAgentsNum;
    else
        m_numAgentsToSpawn = num;
}
