#ifndef STATISTICS_H
#define STATISTICS_H

#include <agent.h>
#include <QObject>
#include <QVector>
#include <QDebug>
#include <QMap>

const int INJURING_FORCE = 1000;
const int DEAD_FORCE = 1500;

struct AgentStat
{
    int id = 0;
    AgentType type;
    double curForce = 0;
    double maxForce = 0;
    double sumForces = 0;
    int iterations = 1;
};

struct ResultStat
{
    int injuringNum = 0;
    int deadNum = 0;
    double averageForce = 0;
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
    void agentQuitSlot();
    void gatherInfoSlot(const Agent & agent, QVector2D force, int iter);
    void finishSimulation();

private:
    double m_currentTime = 1;

    int m_agentQuitNum = 1;
    int m_iterations = 0;


    QVector<AgentStat> m_agentStat;
    QMap<AgentType, int> m_typeRatio;

    void updateAgentStat(const Agent& agent, double force);

    ResultStat makeReport();
};

#endif // STATISTICS_H
