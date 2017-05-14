#ifndef STATISTICS_H
#define STATISTICS_H

#include <agent.h>
#include <QObject>
#include <QVector>
#include <QDebug>

class Statistics: public QObject
{
    Q_OBJECT
public:
    Statistics();

public slots:
    void simulationStart(int number_of_agents);
    void agent_quit();
    int gather_info(const Agent & agent, QVector2D force, int time);
    void finishSimulation();

private:

    const int injuring_force = 1000;
    const int dead_force = 1500;

    int m_simulationTime = 0;
    int m_number_of_agents = 0;
    int m_current_time = -1;
    int m_number_of_iterations = 0;
    int m_agents_quit = 0;
    int m_injured_agents = 0;
    int m_dead_agents = 0;
    int m_children = 0;
    int m_women = 0;
    int m_men = 0;
    int m_old = 0;
    int m_other = 0;
    double m_sum_of_forces = 0;
    double m_sum_of_forces_iter = 0;
    double m_sum_of_speeds = 0;
    double m_sum_of_speeds_iter = 0;
    double m_sum_of_desired_speeds = 0;
    double m_sum_of_desired_speeds_iter = 0;

    QVector<int> m_injured_list;
    QVector<int> m_dead_list;

};

#endif // STATISTICS_H
