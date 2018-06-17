#ifndef STATISTICS_H
#define STATISTICS_H

#include <src/agent.h>
#include <QObject>
#include <QVector>
#include <QDebug>
#include <QMap>

#include <QDateTime>

const int INJURING_FORCE = 1000;
const int DEAD_FORCE = 1500;

struct AgentStat
{
    int id = 0;
    AgentType type;

    double maxForce = 0;
    double sumForces = 0;

    double sumSpeeds = 0;
    double sumWishedSpeeds = 0;

    int iterations = 1;
};

struct ResultStat
{
    int injuringNum = 0;
    int deadNum = 0;
    double averageForce = 0;
    double averageSpeed = 0;
    double averageSpeedDelta = 0;
};

class Statistics: public QObject
{
    Q_OBJECT
public:
    Statistics();

    QString getReport();

    void reset();

public slots:
    void simulationStartSlot(int number_of_agents);
    void agentQuitSlot(const Agent& agent);
    void gatherInfoSlot(const Agent & agent, double force);
    void finishSimulation();

private:
    double m_currentTime = 0;

    int m_agentQuitNum = 0;
    int m_iterations = 0;

    QDateTime m_startTime;

    QVector<AgentStat> m_agentStat;
    QMap<AgentType, int> m_quitTypeRatio;
    QMap<AgentType, int> m_injuredTypeRatio;

    void updateAgentStat(const Agent& agent, double force);

    ResultStat makeReport();
};

#endif // STATISTICS_H
