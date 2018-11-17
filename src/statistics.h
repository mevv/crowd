#ifndef STATISTICS_H
#define STATISTICS_H

#include "agent.h"

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QMap>
#include <QDateTime>

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
    Statistics() {}

    QString getReport();

    void setInjuringForce(double force) { m_injuringForce = force; }
    void setDeadForce(double force) { m_deadForce = force; }
    void reset();

public slots:
    void simulationStartSlot();
    void agentQuitSlot(const Agent& agent);
    void gatherInfoSlot(const Agent & agent, double force);
    void finishSimulation(int time);

private:
    int m_simTime = 0; // milliseconds
    int m_agentQuitNum = 0;
    int m_iterations = 0;
    double m_injuringForce = 1000;
    double m_deadForce = 1500;

    QDateTime m_startTime;
    QVector<AgentStat> m_agentStat;
    QMap<AgentType, int> m_quitTypeRatio;
    QMap<AgentType, int> m_injuredTypeRatio;

    ResultStat makeReport();
};

#endif // STATISTICS_H
