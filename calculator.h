#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <memory>

#include <QDebug>

#include "objects_pool.h"

const double defaultSpeedModule = 5;
const double delta_T = 0.5;
const double A = 2000;
const double B = 0.08;
const double K = 120000;
const double Awall = 20;
const double Bwall = 0.8;
//const double Bwall = 0.8;
const double Kwall = 24;

class Calculator: public QObject
{
    Q_OBJECT
public:

    Calculator(QPoint sceneSize, std::shared_ptr<ObjectsPool> pool);

    std::vector<QVector2D> update(double delta);

signals:
    void agentStat(const Agent &agent);
    void removeAgentSignal();


private:

    QPoint m_sceneSize;//meters
    std::shared_ptr<ObjectsPool> m_pool;
    double m_time;


    void move(Agent &agent);

    bool isInExit(const Agent &agent);

    void calcForce(Agent &agent);

    QVector2D calcPanicForce(const Agent &agent);

    QPoint getNearestExit(const Agent &agent);

    QVector2D calcNormal(QPoint a, QPoint b);

    QVector2D calcCrossAgentForce(const Agent &agent);

    QVector2D calcWallForce(const Agent &agent);

    QPoint getMinDistanceToObstalce(const Agent &agent, const Obstacle &obstacle);

    double getDistanceToSide(const QPoint &a, const QPoint &b, const Agent &agent, QPoint &result);

    double scalarMultiplication(const QVector2D &a, QVector2D &b);

    double distanceBetweenPoints(const QPoint &a, const QPoint &b);

    int Heaviside(double n);

};

#endif // CALCULATOR_H
