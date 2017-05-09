#include "calculator.h"
#include <QPainter>

Calculator::Calculator(QPoint sceneSize, std::shared_ptr<ObjectsPool> pool) :
    m_sceneSize(sceneSize),
    m_pool(pool)
{

}

void Calculator::setDirection(Agent &agent)
{
//    QPoint coord = getNearestExit(agent);
//    double distance = sqrt( pow( agent.getPos().x() - coord.x() ,2)
//                          + pow( agent.getPos().y() - coord.y() ,2) );

//    double hypotenuse = sqrt((pow(agent.getSpeed().x(),2) + pow(agent.getSpeed().y(),2))) * m_time + 2;
//    QPoint newCoords = QPoint(hypotenuse*(agent.getPos().x() - coord.x())/distance,
//                              hypotenuse*(agent.getPos().y() - coord.y())/distance);

//    agent.setPrevPos();

//    agent.setPos(QVector2D(agent.getPos().x() - newCoords.x(),
//                           agent.getPos().y() - newCoords.y()));

    agent.setPos(agent.getPos() + agent.getSpeed() * m_time);
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

QPoint Calculator::getNearestExit(const Agent &agent)
{
    auto exits = m_pool->getExits();
    QPoint coord;
    double distance = INFINITY;

    for(auto i : exits)
    {
        double tmp = sqrt( pow( agent.getPos().x() - i.getCenter().x() ,2) + pow( agent.getPos().y() - i.getCenter().y() ,2) );
        if(tmp < distance)
        {
            distance = tmp;
            coord = i.getCenter();
        }
    }
    return coord;
}

QVector2D Calculator::calcPanicForce(const Agent &agent)
{
    QPoint coord = getNearestExit(agent);
    if(agent.getPos().toPoint() == coord)
    {
        m_pool->removeAgent(agent);
        return QVector2D();
    }
    QVector2D desiredSpeed = calcNormal(agent.getPos().toPoint(), coord);

    double a = sqrt(pow(defaultSpeedModule,2)/desiredSpeed.lengthSquared());
    desiredSpeed *= a;
    QVector2D panicForce = (desiredSpeed - agent.getSpeed())/delta_T;
    return panicForce;
}

QVector2D Calculator::calcNormal(QPoint a, QPoint b)
{
    return QVector2D(b.x() -  a.x(), b.y() - a.y()).normalized();
}

QVector2D Calculator::calcCrossAgentForce(const Agent &agent)
{
    QVector2D crossAgentForce(0,0);
    auto agents = m_pool->getAgents();
    for(Agent i : agents)
    {
        if(agent == i) continue;
        double D = agent.getSize() + i.getSize() - (i.getCenter() - agent.getCenter()).length();
//        D /= 50;
        QVector2D n = calcNormal( i.getCenter().toPoint(), agent.getCenter().toPoint());

        QVector2D deltaV = i.getSpeed() - agent.getSpeed();
        QVector2D tau(n.y(), -n.x());
//        crossAgentForce += A*n*exp(D/B) + K*Heaviside(D)*D*n + K*Heaviside(D)*D*deltaV*tau*tau;
        crossAgentForce += A*n*exp(D/B) + K*Heaviside(D)*D*n + K*Heaviside(D)*D*deltaV*tau*tau;

    }

    return crossAgentForce;
}

QVector2D Calculator::calcWallForce(const Agent &agent)
{
    QVector2D wallForce(0,0);
    auto obstacles = m_pool->getObstacles();
    for(auto i : obstacles)
    {

        QVector2D nearestPoint(getMinDistanceToObstalce(agent, i));
        double D = agent.getSize() - (agent.getCenter() - nearestPoint).length();
        QVector2D n = calcNormal(nearestPoint.toPoint(), agent.getCenter().toPoint());
        QVector2D tau(n.y(), -n.x());
        wallForce += Awall*n*exp(D/Bwall) + Kwall*Heaviside(D)*D*n  - Kwall*Heaviside(D)*D*agent.getSpeed()*tau*tau;
//        qDebug() << "n: " << n << " WF: " << wallForce.normalized();
    }
//    qDebug() << "____________________";
    return wallForce;
}

QPoint Calculator::getMinDistanceToObstalce(const Agent &agent, const Obstacle &obstacle)
{
    QPoint nearestSide(0,0);
    double distanceToSide = INFINITY;
    auto apexes = obstacle.getAbsolutePoints();

    QPoint prevApex(-1,-1);

    for(auto i : apexes)
    {
        QPoint nearestPoint;
        if(prevApex == QPoint(-1,-1))
        {
            prevApex = i;
            continue;
        }
        double tmp = getDistanceToSide(prevApex, i, agent, nearestPoint);

        if( tmp < distanceToSide )
        {
            distanceToSide = tmp;
            nearestSide = nearestPoint;
        }
    }
    return nearestSide;
}

double Calculator::getDistanceToSide(const QPoint &a,const QPoint &b, const Agent &agent, QPoint &result)
{
    QVector2D v(b - a);
    QVector2D w(agent.getCenter().toPoint() - a);
    double c1 = scalarMultiplication(w, v);
    double c2 = scalarMultiplication(v, v);
    if(c1 <= 0)
    {
        result = a;
        return distanceBetweenPoints(agent.getCenter().toPoint(), a);
    }
    if(c2 <= c1)
    {
        result = b;
        return distanceBetweenPoints(agent.getCenter().toPoint(), b);
    }
    double k = c1/c2;
    result.setX(a.x() + k*v.x());
    result.setY(a.y() + k*v.y());

    //    distance( Point P, Segment P0:P1 )
    //          {
    //          v = P1 - P0
    //          w = P - P0
    //          if ( (c1 = w · v) <= 0 )
    //                return d(P, P0)
    //          if ( (c2 = v · v) <= c1 )
    //                return d(P, P1)
    //          b = c1 / c2
    //          Pb = P0 + bv
    //          return d(P, Pb)
    //          }

    return distanceBetweenPoints(agent.getCenter().toPoint(), result);
}

double Calculator::distanceBetweenPoints(const QPoint &a, const QPoint &b)
{
    return sqrt(pow(b.x() - a.x(), 2) + pow( b.y() - a.y(), 2) );
}

double Calculator::scalarMultiplication(const QVector2D &a, QVector2D &b)
{
    return a.x()*b.x() + a.y()*b.y();
}

int Calculator::Heaviside(double n)
{
    return n >= 0 ? 1 : 0;
}

void Calculator::calcForce(Agent &agent)
{
    QVector2D panicForce = calcPanicForce(agent);
    QVector2D crossAgentForce = calcCrossAgentForce(agent);
    QVector2D wallForce = calcWallForce(agent);
//    QVector2D totalForce = panicForce + crossAgentForce;
    QVector2D totalForce = panicForce + crossAgentForce + wallForce;
    QVector2D speed = agent.getSpeed() + m_time*totalForce/agent.getMass();
    agent.setSpeed(speed);
//    qDebug() << totalForce;
}

std::vector<QVector2D> Calculator::update(double delta)
{
    m_time = delta / 1000;
    std::vector<QVector2D> moveRecord;



    for (auto&& i : m_pool->getAgents())
    {
        if(isInExit(i))
        {
            m_pool->removeAgent(i);
            continue;
        }
        setDirection(i);

        moveRecord.push_back(i.getCenter());
    }

    for (auto&& i : m_pool->getAgents())
    {
        calcForce(i);
    }

    return moveRecord;
}
