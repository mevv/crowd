#include "calculator.h"

#include <QTime>

#include <algorithm>

#include "json_manager.h"
#include "general_builder.h"
#include "is_in_polygon.h"

Calculator::Calculator(QVector2D sceneSize, std::shared_ptr<ObjectsPool> pool) :
    m_sceneSize(sceneSize),
    m_pool(pool)
{
    QTime t = QTime::currentTime();
    qsrand((uint)t.msec());
}

void Calculator::move(Agent &agent)
{
    agent.setPos(agent.getPos() + agent.getSpeed() * m_time);
}

bool Calculator::isInExit(const Agent &agent)
{
    auto exits = m_pool->getExits();

    for(auto i : exits)
    {
//        QVector2D c = i.getBegin();
//        QVector2D d = i.getEnd();
//        double common = (b.x() - a.x())*(d.y() - c.y()) - (b.y() - a.y())*(d.x() - c.x());

//        if (common == 0)
//            continue;

//        double rH = (a.y() - c.y())*(d.x() - c.x()) - (a.x() - c.x())*(d.y() - c.y());
//        double sH = (a.y() - c.y())*(b.x() - a.x()) - (a.x() - c.x())*(b.y() - a.y());

//        double r = rH / common;
//        double s = sH / common;

//        if (r >= 0 && r <= 1 && s >= 0 && s <= 1)
//            return true;
        QVector2D tmp;
        if (getDistanceToSide(QVector2D(i.getBegin()), QVector2D(i.getEnd()), agent, tmp) < DISTANCE_TO_EXIT)
            return true;
    }

    return false;
}

double Calculator::getRandomNumber(double a, double b)
{
    if (a == b)
        return a;

    double hack = 1000;

    a *= hack;
    b *= hack;

    return (qrand() % ((int)b - (int)a) + (int)a) / hack;
}

QVector2D getCenterOfLine(QVector2D begin, QVector2D end)
{
    return QVector2D((begin.x() + end.x()) / 2.0, (begin.y() + end.y()) / 2.0);
}

QVector2D Calculator::getNearestExit(const Agent &agent)
{
    auto exits = m_pool->getExits();
    QVector2D coord(0,0);
    double distance = INFINITY;

    for(auto i : exits)
    {
        QVector2D point(0, 0);

        double tmp = getDistanceToSide(i.getBegin(), i.getEnd(), agent, point);

        //small hack:)
        point = getCenterOfLine(point, getCenterOfLine(i.getBegin(), i.getEnd()));

        if(tmp < distance)
        {
            distance = tmp;
            coord = point;
        }
    }

    return coord;
}

QVector2D Calculator::calcPanicForce(const Agent &agent)
{
    QVector2D coord = getNearestExit(agent);

    if(agent.getCenter() == coord)
        return QVector2D(0, 0);

    QVector2D desiredSpeed(0, 0);

    desiredSpeed = calcNormal(agent.getCenter(), coord);

    if (m_usePathFinding)
    {
        if (m_pool->getCheckpoints().find(agent.getID()) != m_pool->getCheckpoints().end())
        {
            if (m_pool->getCheckpoints()[agent.getID()].size() > 0)
            {
                if (distanceBetweenPoints(agent.getCenter(), m_pool->getCheckpoints()[agent.getID()][0].getPos()) < m_pool->getCheckpoints()[agent.getID()][0].getRadius())
                {
                    m_pool->getCheckpoints()[agent.getID()].erase(m_pool->getCheckpoints()[agent.getID()].begin());
                }
                else
                {
                    desiredSpeed = calcNormal(agent.getCenter(), m_pool->getCheckpoints()[agent.getID()][0].getPos());
                }
            }
            else
            {
                m_pool->getCheckpoints().erase(agent.getID());
            }
        }

    }

//    double a = sqrt(pow(agent.getWishSpeed(),2)/desiredSpeed.lengthSquared());
//    desiredSpeed *= a;

    desiredSpeed *= agent.getWishSpeed();

    QVector2D panicForce = (desiredSpeed - agent.getSpeed()) / m_param.deltaT * agent.getMass();

    return panicForce;
}

QVector2D Calculator::calcNormal(QVector2D a, QVector2D b)
{
    return QVector2D(b.x() -  a.x(), b.y() - a.y()).normalized();
}

QVector2D Calculator::calcTau(QVector2D n, QVector2D speed)
{
    QVector2D tauContrClockWise(n.y(), -n.x());
    QVector2D tauClockWise(-n.y(), n.x());

    if (scalarMultiplication(tauClockWise, speed.normalized()) > 0)
        return tauClockWise;
    else
        return tauContrClockWise;
}

QVector2D Calculator::calcCrossAgentForce(const Agent &agent)
{
    QVector2D crossAgentForce(0,0);

    auto agents = m_pool->getAgents();
    for(Agent i : agents)
    {
        if(agent == i)
            continue;

        if (distanceBetweenPoints(agent.getCenter(), i.getCenter()) < DITANCE_OF_INTERACTION)
        {
            double D = agent.getSize() + i.getSize() - (i.getCenter() - agent.getCenter()).length();

            QVector2D n = calcNormal( i.getCenter(), agent.getCenter());
            QVector2D deltaV = i.getSpeed() - agent.getSpeed();
            QVector2D tau = calcTau(n, agent.getSpeed());//normal rotated on 90 degrees

            QVector2D repulsionForce = m_param.K*Heaviside(D)*D*n;
            QVector2D frictionForce = m_param.K*Heaviside(D)*D*deltaV*tau*tau;

            m_physicalForcesAgentSum += repulsionForce.length();
            m_physicalForcesAgentSum += frictionForce.length();

            crossAgentForce += m_param.A*n*exp(D/m_param.B) + repulsionForce + frictionForce;
        }
    }

    return crossAgentForce;
}

QVector2D Calculator::calcWallForce(const Agent &agent)
{
    QVector2D wallForce(0,0);
    auto obstacles = m_pool->getObstacles();

    //QVector2D nearestPoint(-1000, -1000);
    //double minDistance = 1000000;

    for(auto i : obstacles)
    {
        QVector2D nearestPoint(getNearestPointOfObstacle(agent, i));
//        double curDistance = distanceBetweenPoints(agent.getCenter(), nearestPoint);

//        if (curDistance < DITANCE_OF_INTERACTION)
//            continue;

//        if (curDistance < minDistance)
//        {
//            nearestPoint = curObtacleNearestPoint;
//            minDistance = curDistance;
//        }
//    }
        double D = agent.getSize() - (agent.getCenter() - nearestPoint).length();

        QVector2D n = calcNormal(nearestPoint, agent.getCenter());
        QVector2D tau = calcTau(n, agent.getSpeed());

        QVector2D repulsionForce = m_param.Kwall*Heaviside(D)*D*n;
        QVector2D frictionForce = m_param.Kwall*Heaviside(D)*D*agent.getSpeed()*tau*tau;

        m_physicalForcesAgentSum += repulsionForce.length();
        m_physicalForcesAgentSum += frictionForce.length();

        wallForce += m_param.Awall*n*exp(D/m_param.Bwall) + repulsionForce - frictionForce;
    }
    return wallForce;
}

QVector2D Calculator::getNearestPointOfObstacle(const Agent &agent, const Obstacle &obstacle)
{
    //return QVector2D(100, agent.getCenter().y());
    QVector2D result(0,0);
    double distanceToSide = INFINITY;
    auto apexes = obstacle.getAbsolutePoints();

    apexes.push_back(apexes[0]);

    QVector2D prevApex(-1,-1);

    for(auto i : apexes)
    {
        QVector2D nearestPoint;
        if(prevApex == QVector2D(-1,-1))
        {
            prevApex = i;
            continue;
        }

        double tmp = getDistanceToSide(prevApex, i, agent, nearestPoint);

        prevApex = i;

        if(tmp < distanceToSide)
        {
            distanceToSide = tmp;
            result = nearestPoint;
        }
    }

    return result;
}


//original algorithm pseudocode
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
double Calculator::getDistanceToSide(const QVector2D &a,const QVector2D &b, const Agent &agent, QVector2D &result)
{
    QVector2D v(b - a);
    QVector2D w(agent.getCenter() - a);

    double c1 = scalarMultiplication(w, v);
    double c2 = scalarMultiplication(v, v);

    if(c1 <= 0)
    {
        result = a;
        return distanceBetweenPoints(agent.getCenter(), a);
    }

    if(c2 <= c1)
    {
        result = b;
        return distanceBetweenPoints(agent.getCenter(), b);
    }

    double k = c1/c2;

    result.setX(a.x() + k*v.x());
    result.setY(a.y() + k*v.y());

    return distanceBetweenPoints(agent.getCenter(), result);
}

double Calculator::distanceBetweenPoints(const QVector2D &a, const QVector2D &b)
{
    return sqrt(pow(b.x() - a.x(), 2) + pow( b.y() - a.y(), 2) );
}

double Calculator::scalarMultiplication(const QVector2D &a, const QVector2D &b)
{
    return a.x()*b.x() + a.y()*b.y();
}

int Calculator::Heaviside(double n)
{
    return n >= 0 ? 1 : 0;
}

void Calculator::calcForce(Agent &agent)
{
    QTime start = QTime::currentTime();
    QVector2D panicForce = calcPanicForce(agent);
    //qDebug() << "panic force:" << start.elapsed();
    start = QTime::currentTime();
    QVector2D crossAgentForce = calcCrossAgentForce(agent);
    //qDebug() << "cross agent force:" << start.elapsed();
    start = QTime::currentTime();
    QVector2D wallForce = calcWallForce(agent);
    //qDebug() << "wall force:" << start.elapsed();
    QVector2D totalForce = panicForce + crossAgentForce + wallForce;

    if (m_iscollectStat)
        emit sendStatSignal(agent, m_physicalForcesAgentSum);

    m_physicalForcesAgentSum = 0;

    QVector2D speed = agent.getSpeed() + m_time * totalForce / agent.getMass();

    agent.setSpeed(speed);
}

QVector2D Calculator::getPointOnLine(QVector2D a, QVector2D b)
{
    if (b.x() == a.x())
        return QVector2D(a.x(), getRandomNumber(std::min(a.y(), b.y()), std::max(a.y(), b.y())));
    else
    {
        double k = (b.y() - a.y()) / (b.x() - a.x());
        double c = a.y() - k * a.x();
        double x = getRandomNumber(std::min(a.x(), b.x()), std::max(a.x(), b.x()));

        return QVector2D(x, k * x + c);
    }
}

void Calculator::entryProcess()
{
    for (auto& i : m_pool->getEntries())
    {
        double curPeriod = getRandomNumber(i.getPeriodRange().first, i.getPeriodRange().second);

        QJsonObject configData = JsonManager::parseJsonFile(JsonManager::getConfPath());

        if (i.getTimeFromLastGenerate() >= curPeriod)
        {
            QVector2D entry(i.getEnd() - i.getPos());
            entry.normalize();
            QVector2D entryDir(entry.y(), -entry.x());

            Agent newAgent = GeneralBuilder::buildSingleAgent(configData, getPointOnLine(i.getPos(), i.getEnd()), entryDir, m_panicLevel);

            m_pool->addAgent(newAgent);

            emit enterAgentSignal();

            GeneralBuilder::buildCheckPointsForSingleAgent(configData, *m_pool, *this, newAgent, pathAlgorithmIndex);

            i.resetTimeFromLastGenerate();
        }
        else
        {
            i.setTimeFromLastGenerate(i.getTimeFromLastGenerate() + m_time);
        }
    }
}

std::vector<QVector2D> Calculator::update(double delta)
{
    m_time = delta / 1000;
    std::vector<QVector2D> moveRecord;

    m_iterations++;

    entryProcess();


    for (auto i = m_pool->getAgents().begin(); i != m_pool->getAgents().end();)
    {
        calcForce(*i);
        move(*i);

        //moveRecord.push_back(i->getCenter());

        if (isInExit(*i))
        {
            if (m_iscollectStat)
                emit removeAgentSignal(*i);

            i =  m_pool->removeAgent(*i);
        }
        else
            i++;
    }

    return moveRecord;
}

QVector<double> Calculator::buildAStarMatrix(int & height, int & width)
{
    QVector<double> res;

    height  = (int)m_sceneSize.y() / m_gridStep;
    width = (int)m_sceneSize.x() / m_gridStep;

    // TODO: maybe here ERROR - change width and height
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            res.push_back(((isInObstacle(m_gridStep * j + m_gridStep / 2.0, m_gridStep * i + m_gridStep / 2.0)) ? 9.0 : 1.0));
        }
    }
    return res;
}

int Calculator::isInObstacle(double x, double y)
{
    for(auto i : m_pool->getObstacles())
    {
        std::vector<Point> polygon;

        for (auto j : i.getAbsolutePoints())
        {
            Point point;
            point.x = j.x();
            point.y = j.y();
            polygon.push_back(point);
        }

        Point point;
        point.x = x;
        point.y = y;

        if (InPolygon(polygon, point))
            return true;
    }

    return false;
}

void Calculator::pathAlgorithmChangedSlot(int index)
{
    pathAlgorithmIndex = index;
}

void Calculator::changePanicLevelSlot(double panicLevel)
{
    m_panicLevel = panicLevel;
    updateWishSpeeds();
}

void Calculator::updateWishSpeeds()
{
    QJsonObject configData = JsonManager::parseJsonFile(JsonManager::getConfPath());

    QJsonObject childWishSpeed = configData.value("agent").toObject().value("children").toObject().value("wish_speed").toObject();
    QJsonObject manWishSpeed = configData.value("agent").toObject().value("children").toObject().value("wish_speed").toObject();
    QJsonObject womanWishSpeed = configData.value("agent").toObject().value("children").toObject().value("wish_speed").toObject();
    QJsonObject oldWishSpeed = configData.value("agent").toObject().value("children").toObject().value("wish_speed").toObject();
    QJsonObject customWishSpeed = configData.value("agent").toObject().value("children").toObject().value("wish_speed").toObject();

    for (auto& i : m_pool->getAgents())
    {
        if (i.getType() == AgentType::Child)
            i.setWishSpeed(childWishSpeed.value("min").toDouble() +
                          (childWishSpeed.value("max").toDouble() -
                           childWishSpeed.value("min").toDouble()) * m_panicLevel);
        else if (i.getType() == AgentType::Man)
            i.setWishSpeed(manWishSpeed.value("min").toDouble() +
                          (manWishSpeed.value("max").toDouble() -
                           manWishSpeed.value("min").toDouble()) * m_panicLevel);
        else if (i.getType() == AgentType::Woman)
            i.setWishSpeed(womanWishSpeed.value("min").toDouble() +
                          (womanWishSpeed.value("max").toDouble() -
                           womanWishSpeed.value("min").toDouble()) * m_panicLevel);
        else if (i.getType() == AgentType::Old)
            i.setWishSpeed(oldWishSpeed.value("min").toDouble() +
                          (oldWishSpeed.value("max").toDouble() -
                           oldWishSpeed.value("min").toDouble()) * m_panicLevel);
        else if (i.getType() == AgentType::Custom)
            i.setWishSpeed(customWishSpeed.value("min").toDouble() +
                          (customWishSpeed.value("max").toDouble() -
                           customWishSpeed.value("min").toDouble()) * m_panicLevel);
    }
}

