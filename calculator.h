#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <memory>
#include <QDebug>

//#include "a_star.h"
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

    Calculator(QVector2D sceneSize, std::shared_ptr<ObjectsPool> pool);

    std::vector<QVector2D> update(double delta);

    void setMathParams(const MathParams& param) { m_param = param; }

    void setCollectStat(bool collectStat = true) { m_iscollectStat = collectStat; }

    void setSceneSize(QVector2D size) { m_sceneSize = size; }

    bool isCollectStat() const { return m_iscollectStat; }

    inline MathParams getMathParams() { return m_param; }


    QVector<double> buildAStarMatrix(int & height, int & width);

    QVector2D getNearestExit(const Agent &agent);

signals:
    void sendStatSignal(const Agent & agent, double force);
    void removeAgentSignal(const Agent& agent);

    
private:

    double gridStep = 1;

    QVector2D m_sceneSize;//meters

    std::shared_ptr<ObjectsPool> m_pool;

    double m_time;
    double m_physicalForcesAgentSum = 0;

    int m_iterations = 0;
    int m_newId = 1000000;

    MathParams m_param;



    int isInObstacle(double x, double y);

    bool m_iscollectStat = true;

    void move(Agent &agent);

    void entryProcess();

    bool isInExit(const Agent &agent);

    void calcForce(Agent &agent);

    QVector2D calcPanicForce(const Agent &agent);



    QVector2D calcNormal(QVector2D a, QVector2D b);

    QVector2D calcTau(QVector2D n, QVector2D speed);

    QVector2D calcCrossAgentForce(const Agent &agent);

    QVector2D calcWallForce(const Agent &agent);

    QVector2D getMinDistanceToObstalce(const Agent &agent, const Obstacle &obstacle);

    double getDistanceToSide(const QVector2D &a, const QVector2D &b, const Agent &agent, QVector2D &result);

    QVector2D getPointOnLine(QVector2D a, QVector2D b);

    double scalarMultiplication(const QVector2D &a, const QVector2D &b);

    double distanceBetweenPoints(const QVector2D &a, const QVector2D &b);

    double getRandomNumber(double a, double b);

    int Heaviside(double n);

};

#endif // CALCULATOR_H
