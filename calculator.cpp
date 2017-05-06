#include "calculator.h"

Calculator::Calculator(QPoint sceneSize, std::shared_ptr<ObjectsPool> pool) :
    m_sceneSize(sceneSize),
    m_pool(pool)
{

}


void Calculator::setDirection(Agent &agent)
{
    auto exits = m_pool->getExits();
    QPoint coord;
    double distance = 999999999999;

    for(auto i : exits)
    {
        double tmp = sqrt( pow( agent.getPos().x() - i.getCenter().x() ,2) + pow( agent.getPos().y() - i.getCenter().y() ,2) );
        if(tmp < distance)
        {
            distance = tmp;
            coord = i.getCenter();
        }
    }

    double hypotenuse = sqrt((pow(agent.getSpeed().x(),2) + pow(agent.getSpeed().y(),2))) * time;
    QPoint newCoords = QPoint(hypotenuse*(agent.getPos().x() - coord.x())/distance,
                              hypotenuse*(agent.getPos().y() - coord.y())/distance);

    agent.setPrevPos();

    agent.setPos(QVector2D(agent.getPos().x() - newCoords.x(),
                           agent.getPos().y() - newCoords.y()));

}

bool Calculator::isInExit(Agent agent)
{
    auto exits = m_pool->getExits();
    QVector2D a = agent.getPos();
    QVector2D b = agent.getPrevPos();

    for(auto i : exits)
    {
        QVector2D c = i.getBegin();
        QVector2D d = i.getEnd();
        double common = (b.x() - a.x())*(d.y() - c.y()) - (b.y() - a.y())*(d.x() - c.x());

        if (common == 0) continue;

        double rH = (a.y() - c.y())*(d.x() - c.x()) - (a.x() - c.x())*(d.y() - c.y());
        double sH = (a.y() - c.y())*(b.x() - a.x()) - (a.x() - c.x())*(b.y() - a.y());

        double r = rH / common;
        double s = sH / common;

        if (r >= 0 && r <= 1 && s >= 0 && s <= 1)
            return true;
    }
    return false;
}


void Calculator::update(double delta)
{
    time = delta / 1000;

    for (auto&& i : m_pool->getAgents())
    {
//        qDebug() << i.getSpeed();
//        qDebug() << i.getPos();

//        if (i.getPos().x() > m_sceneSize.x() - i.getSize() || i.getPos().x() < 0)
//            i.setSpeed(QVector2D(i.getSpeed().x() * -1, i.getSpeed().y()));

//        if (i.getPos().y() > m_sceneSize.y() - i.getSize() || i.getPos().y() < 0)
//            i.setSpeed(QVector2D(i.getSpeed().x(), i.getSpeed().y() * -1));

        setDirection(i);
        if(isInExit(i))
        {
            m_pool->removeAgent(i);
            continue;
        }

//        i.setPos(QVector2D(i.getPos().x() + time * i.getSpeed().x(), i.getPos().y() + time * i.getSpeed().y()));
    }

    auto i  = m_pool->getExits();
}
