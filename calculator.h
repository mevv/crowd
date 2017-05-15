#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <memory>

#include <QDebug>

#include "objects_pool.h"

//const double defaultSpeedModule = 5;
//const double delta_T = 0.5;
//const double A = 2000;
//const double B = 0.08;
//const double K = 120000;
//const double Awall = 20;
//const double Bwall = 0.8;
//const double Kwall = 24;

struct MathParams
{
    double deltaT = 0.5;
    double A = 2000;
    double B = 0.08;
    double K = 120000;
    double Awall = 20;
    double Bwall = 0.8;
    double Kwall = 24;
};

class Calculator: public QObject
{
    Q_OBJECT
public:

    Calculator(QPoint sceneSize, std::shared_ptr<ObjectsPool> pool);

    std::vector<QVector2D> update(double delta);

    void setMathParams(const MathParams& param) { m_param = param; }

    inline MathParams getMathParams() { return m_param; }

signals:
    void agentStat(const Agent &agent);
    void removeAgentSignal();

    
private:

    QPoint m_sceneSize;//meters

    std::shared_ptr<ObjectsPool> m_pool;

    double m_time;

    MathParams m_param;


    void move(Agent &agent);

    bool isInExit(const Agent &agent);

    void calcForce(Agent &agent);

    QVector2D calcPanicForce(const Agent &agent);

    QVector2D getNearestExit(const Agent &agent);

    QVector2D calcNormal(QVector2D a, QVector2D b);

    QVector2D calcCrossAgentForce(const Agent &agent);

    QVector2D calcWallForce(const Agent &agent);

    QVector2D getMinDistanceToObstalce(const Agent &agent, const Obstacle &obstacle);

    double getDistanceToSide(const QVector2D &a, const QVector2D &b, const Agent &agent, QVector2D &result);

    double scalarMultiplication(const QVector2D &a, QVector2D &b);

    double distanceBetweenPoints(const QVector2D &a, const QVector2D &b);

    int Heaviside(double n);

};

#endif // CALCULATOR_H
