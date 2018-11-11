#ifndef OBJECTSPOOL_H
#define OBJECTSPOOL_H

#include <vector>
#include <map>

#include "agent.h"
#include "obstacle.h"
#include "exit.h"
#include "entry.h"
#include "spawn_zone.h"
#include "checkpoint.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>

class ObjectsPool: public QObject
{
    Q_OBJECT
public:
    ObjectsPool();

    ~ObjectsPool();

    inline std::vector<Agent>& getAgents() { return m_agents; }

    inline std::vector<Obstacle>& getObstacles() { return m_obstacles; }

    inline std::vector<Exit>& getExits() { return m_exits; }

    inline std::vector<Entry>& getEntries() { return m_entries; }

    inline std::vector<SpawnZone>& getSpawnZones() { return m_spawnZones; }

    inline std::map<int,std::vector<Checkpoint>>& getCheckpoints() { return m_checkpoints; }


    void addAgent(Agent agent) { m_agents.push_back(agent); }

    void addObstacle(const Obstacle& obstacle) { m_obstacles.push_back(obstacle); }

    void addExit(const Exit& exit) { m_exits.push_back(exit); }

    void addEntry(const Entry& entry) { m_entries.push_back(entry); }

    void addSpawnZone(const SpawnZone& spawnZone) { m_spawnZones.push_back(spawnZone); }

    //void addCheckpoint(const Checkpoint& checkpoint) { m_checkpoints.push_back(checkpoint); }

    void clear();

    std::vector<Agent>::iterator removeAgent(const Agent &agent);

signals:
    void endOfSimulation();

private:
    std::vector<Agent> m_agents;
    std::vector<Obstacle> m_obstacles;
    std::vector<Exit> m_exits;
    std::vector<Entry> m_entries;
    std::vector<SpawnZone> m_spawnZones;

    std::map<int, std::vector<Checkpoint>> m_checkpoints;

    int deleted = 0;
};

#endif // OBJECTSPOOL_H
