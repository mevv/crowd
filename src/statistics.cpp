#include "statistics.h"
#include "unistd.h"



Statistics::Statistics()
{

}

void Statistics::simulationStartSlot()
{
    m_startTime = QDateTime::currentDateTime();
}


void Statistics::gatherInfoSlot(const Agent & agent, double force)
{
    for (auto& i : m_agentStat)
    {
        if (i.id == agent.getID())
        {
            i.sumForces += force;

            i.sumSpeeds += agent.getSpeed().length();
            i.sumWishedSpeeds += agent.getWishSpeed();

            i.iterations++;

            if (force > i.maxForce)
                i.maxForce = force;

            m_iterations++;

            return ;
        }
    }

    AgentStat stat;

    stat.id = agent.getID();
    stat.type = agent.getType();
    stat.iterations = 1;
    stat.sumForces = force;
    stat.sumSpeeds = agent.getSpeed().length();
    stat.sumWishedSpeeds = agent.getWishSpeed();

    m_agentStat.push_back(stat);

    m_iterations++;
}

void Statistics::agentQuitSlot(const Agent& agent)
{
    m_quitTypeRatio[agent.getType()]++;
    m_agentQuitNum++;
}

void Statistics::finishSimulation()
{

}

ResultStat Statistics::makeReport()
{
    ResultStat stat;

    for (auto i : m_agentStat)
    {
        stat.averageForce += (i.sumForces / i.iterations);
        stat.averageSpeed += (i.sumSpeeds / i.iterations);
        stat.averageSpeedDelta += (i.sumWishedSpeeds - i.sumSpeeds) / i.iterations;

        if (i.maxForce > INJURING_FORCE)
        {
            stat.injuringNum++;

            m_injuredTypeRatio[i.type]++;
        }

        if (i.maxForce > DEAD_FORCE)
            stat.deadNum++;
    }

    stat.averageForce /= m_agentStat.size();
    stat.averageSpeed /= m_agentStat.size();
    stat.averageSpeedDelta /= m_agentStat.size();

    return stat;
}

QString Statistics::getReport()
{
    QString result;
    ResultStat stat = makeReport();

    // result += QString::number(m_agentQuitNum) + " " + QString::number(stat.injuringNum) + " " + QString::number(stat.deadNum)

//    result += "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

//    result += tr("Simulation started:") + m_startTime.toString(Qt::ISODate) + "\n";
//    result += tr("Simulation finished: ") + QDateTime::currentDateTime().toString(Qt::ISODate) + "\n";
//    result += tr("Average force: ") + QString::number(stat.averageForce) + tr(", N") + "\n";
//    result += tr("Average velocity: ") + QString::number(stat.averageSpeed) + tr(", m/s") + "\n";
//    result += tr("Average wished and actual velocity delta: ") + QString::number(stat.averageSpeedDelta) + tr(", m/s") + "\n";
    result += tr("Number of evacuated people: ") + QString::number(m_agentQuitNum) + "\n";
    result += tr("Number of injured people: ") + QString::number(stat.injuringNum) + "\n";
    result += tr("Number of dead people: ") + QString::number(stat.deadNum) + "\n";

//    if (m_agentQuitNum > 0)
//    {
//        result += tr("Evacuated people ratio:\n");
//        result += tr("    Male - ") + QString::number((double)m_quitTypeRatio[AgentType::Man] / m_agentQuitNum * 100.0) + " %" + "\n";
//        result += tr("    Female - ") + QString::number((double)m_quitTypeRatio[AgentType::Woman] / m_agentQuitNum * 100.0) + " %" + "\n";
//        result += tr("    Child - ") + QString::number((double)m_quitTypeRatio[AgentType::Child] / m_agentQuitNum * 100.0) +  " %" + "\n";
//        result += tr("    Old - ") + QString::number((double)m_quitTypeRatio[AgentType::Old] / m_agentQuitNum * 100.0) + " %" + "\n";
//        result += tr("    Additional - ") + QString::number((double)m_quitTypeRatio[AgentType::Custom] / m_agentQuitNum * 100.0) + " %" + "\n";
//    }

//    if (stat.injuringNum > 0)
//    {
//        result += "Injured people ratio:\n";
//        result += "    Male - " + QString::number((double)m_injuredTypeRatio[AgentType::Man] / stat.injuringNum * 100.0) + " %" + "\n";
//        result += "    Female - " + QString::number((double)m_injuredTypeRatio[AgentType::Woman] / stat.injuringNum * 100.0) + " %" + "\n";
//        result += "    Child - " + QString::number((double)m_injuredTypeRatio[AgentType::Child] / stat.injuringNum * 100.0) + " %" + "\n";
//        result += "    Old - " + QString::number((double)m_injuredTypeRatio[AgentType::Old] / stat.injuringNum * 100.0) + " %" + "\n";
//        result += "    Additional - " + QString::number((double)m_injuredTypeRatio[AgentType::Custom] / stat.injuringNum * 100.0) + " %" + "\n";
//    }
    return result;
}

void Statistics::reset()
{
    m_currentTime = 0;
    m_agentQuitNum = 0;
    m_agentStat.clear();
    m_quitTypeRatio.clear();
    m_injuredTypeRatio.clear();
}
