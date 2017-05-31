#include "statistics.h"
#include "unistd.h"



Statistics::Statistics()
{

}

void Statistics::simulationStartSlot(int number_of_agents)
{
    m_startTime = QDateTime::currentDateTime();
}


void Statistics::gatherInfoSlot(const Agent & agent, double force)
{
    //qDebug() << agent.getID() << force << m_agentStat.size();

    //qDebug() << "______________";
    for (auto& i : m_agentStat)
    {
       //qDebug() << i.id << i.sumForces << i.iterations;
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
    //qDebug() << "Added";
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

void Statistics::updateAgentStat(const Agent& agent, double force)
{

}

ResultStat Statistics::makeReport()
{
    ResultStat stat;

    //qDebug() << m_agentStat.size();

    for (auto i : m_agentStat)
    {
        //qDebug() <<  "Report" << i.id << i.sumForces << i.maxForce << i.iterations;
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

    //qDebug() << m_typeRatio;
    return stat;
}

QString Statistics::getReport()
{
    QString result;

    //usleep(5000);

    ResultStat stat = makeReport();

    result += "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

    result += "Симуляція розпочата:" + m_startTime.toString(Qt::ISODate) + "\n";
    result += "Симуляція закінчена: " + QDateTime::currentDateTime().toString(Qt::ISODate) + "\n";
    result += "Середня сила: " + QString::number(stat.averageForce) + " ,Н" + "\n";
    result += "Середня швидкість: " + QString::number(stat.averageSpeed) + " ,м/с" + "\n";
    result += "Середня різниця бажаної та фактичної швидкостей: " + QString::number(stat.averageSpeedDelta) + " ,м/с" + "\n";
    result += "Кількість евакуйованих: " + QString::number(m_agentQuitNum) + "\n";
    result += "Кількість травмованих: " + QString::number(stat.injuringNum) + "\n";
    //result += "Кількість мертвих: " + QString::number(stat.deadNum) + "\n";

    if (m_agentQuitNum > 0)
    {
        result += "Співвідношення евакуйованих по группах: \n";
        result += "    Чоловіки - " + QString::number((double)m_quitTypeRatio[AgentType::Man] / m_agentQuitNum * 100.0) + " %" + "\n";
        result += "    Жінки - " + QString::number((double)m_quitTypeRatio[AgentType::Woman] / m_agentQuitNum * 100.0) + " %" + "\n";
        result += "    Діти - " + QString::number((double)m_quitTypeRatio[AgentType::Child] / m_agentQuitNum * 100.0) +  " %" + "\n";
        result += "    Старці - " + QString::number((double)m_quitTypeRatio[AgentType::Old] / m_agentQuitNum * 100.0) + " %" + "\n";
        result += "    Додаткова група - " + QString::number((double)m_quitTypeRatio[AgentType::Custom] / m_agentQuitNum * 100.0) + " %" + "\n";
    }

    if (stat.injuringNum > 0)
    {
        result += "Співвідношення травмованих по группах: \n";
        result += "    Чоловіки - " + QString::number((double)m_injuredTypeRatio[AgentType::Man] / stat.injuringNum * 100.0) + " %" + "\n";
        result += "    Жінки - " + QString::number((double)m_injuredTypeRatio[AgentType::Woman] / stat.injuringNum * 100.0) + " %" + "\n";
        result += "    Діти - " + QString::number((double)m_injuredTypeRatio[AgentType::Child] / stat.injuringNum * 100.0) + " %" + "\n";
        result += "    Старці - " + QString::number((double)m_injuredTypeRatio[AgentType::Old] / stat.injuringNum * 100.0) + " %" + "\n";
        result += "    Додаткова група - " + QString::number((double)m_injuredTypeRatio[AgentType::Custom] / stat.injuringNum * 100.0) + " %" + "\n";
    }
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
