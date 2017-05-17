#include "statistics.h"

Statistics::Statistics()
{

}

void Statistics::simulationStartSlot(int number_of_agents)
{

}


void Statistics::gatherInfoSlot(const Agent & agent, QVector2D force, int iter)
{
    for (auto& i : m_agentStat)
    {
        if (i.id == agent.getID())
        {
            i.sumForces += force.length();
            i.curForce += force.length();

            if (iter > m_iterations)
            {
                i.iterations++;

                if (i.curForce > i.maxForce)
                    i.maxForce = i.curForce;

                i.curForce = 0;
            }

            m_iterations++;

            return ;
        }
    }

    AgentStat stat;

    stat.id = agent.getID();
    stat.type = agent.getType();
    stat.iterations = 1;
    stat.curForce = force.length();
    stat.sumForces = force.length();

    m_agentStat.push_back(stat);

    m_iterations++;
}

void Statistics::agentQuitSlot()
{
    m_agentQuitNum++;
}

void Statistics::finishSimulation()
{

}

void Statistics::updateAgentStat(const Agent& agent, double force)
{

}

ResultStat Statistics::makeReport()
{
    ResultStat stat;

    qDebug() << m_agentStat.size();

    for (auto i : m_agentStat)
    {
        qDebug() << i.id << i.sumForces << i.maxForce << i.iterations;
        stat.averageForce += (i.sumForces / i.iterations);

        if (i.maxForce > INJURING_FORCE)
            stat.injuringNum++;

        if (i.maxForce > DEAD_FORCE)
            stat.deadNum++;

        m_typeRatio[i.type]++;
    }

    return stat;
}

QString Statistics::getReport()
{
    QString result;

    ResultStat stat = makeReport();

    result += "Час симуляції: " + QString::number(m_currentTime) + "\n";
    result += "Середня сила: " + QString::number(stat.averageForce) + "\n";
    result += "Кількість евакуйованих: " + QString::number(m_agentQuitNum) + "\n";
    result += "Кількість травмованих: " + QString::number(stat.injuringNum) + "\n";
    result += "Кількість мертвих: " + QString::number(stat.deadNum) + "\n";
    if (m_agentQuitNum > 0)
    {
    result += "Співвідношення евакуйованих по группах: \n";
    result += "    Чоловіки - " + QString::number(m_typeRatio[AgentType::Man] / m_agentQuitNum * 100.0) + "\n";
    result += "    Жінки - " + QString::number(m_typeRatio[AgentType::Woman] / m_agentQuitNum * 100.0) + "\n";
    result += "    Діти - " + QString::number(m_typeRatio[AgentType::Child] / m_agentQuitNum * 100.0) + "\n";
    result += "    Старці - " + QString::number(m_typeRatio[AgentType::Old] / m_agentQuitNum * 100.0) + "\n";
    result += "    Додаткова група - " + QString::number(m_typeRatio[AgentType::Custom] / m_agentQuitNum * 100.0) + "\n";
    }

    return result;
}

void Statistics::reset()
{
    m_currentTime = 0;
    m_agentQuitNum = 0;
    m_agentStat.clear();
    m_typeRatio.clear();
}
