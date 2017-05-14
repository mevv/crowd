#include "statistics.h"

Statistics::Statistics()
{

}

void Statistics::simulationStart(int number_of_agents)
{
    m_number_of_agents = number_of_agents;
}


int Statistics::gather_info(const Agent & agent, QVector2D force, int time)
{
    if(m_current_time != time)
    {
        m_current_time = time;
        m_number_of_iterations++;
        m_sum_of_forces += m_sum_of_forces_iter/(m_number_of_agents - m_agents_quit);
        m_sum_of_speeds += m_sum_of_speeds_iter/(m_number_of_agents - m_agents_quit);

        m_sum_of_forces_iter = 0;
        m_sum_of_speeds_iter = 0;
        m_sum_of_desired_speeds_iter = 0;
    }

    m_sum_of_forces_iter += force.length();
    m_sum_of_speeds_iter += agent.getSpeed().length();
    //    idk where to take it
    //        m_sum_of_desired_speeds_iter =

    if(force.length() > dead_force)
    {
        if(!m_dead_list.contains(agent.getID()))
        {
            m_dead_agents++;
            m_dead_list.push_back(agent.getID());
            return 1;
        }
    }

    if(force.length() > injuring_force)
    {
        if(!m_injured_list.contains(agent.getID()))
        {
            m_injured_agents++;
            m_injured_list.push_back(agent.getID());
        }
    }
    return 0;
}

void Statistics::agent_quit()
{
    m_agents_quit += 1;
}

void Statistics::finishSimulation()
{
    double totalForce = m_sum_of_forces/m_number_of_iterations;
    double totalSpeed = m_sum_of_speeds/m_number_of_iterations;
}
