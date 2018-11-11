#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <memory>
#include <QDebug>

//#include "a_star.h"
#include "objects_pool.h"
#include "a_star_step.h"

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

const double DISTANCE_TO_EXIT = 0.5;
const double DITANCE_OF_INTERACTION = 2.0;

class Calculator: public QObject
{
    Q_OBJECT
public:

    Calculator(QVector2D sceneSize, std::shared_ptr<ObjectsPool> pool);

    void update(double delta);

    void setMathParams(const MathParams& param) { m_param = param; }
    void setCollectStat(bool collectStat = true) { m_iscollectStat = collectStat; }
    void setSceneSize(QVector2D size) { m_sceneSize = size; }
    void setGridStep(double gridStep) { m_gridStep = gridStep; }
    void setCheckpointRadius(double radius) { m_checkpointRadius = radius; }
    void setUsePathFinding(bool use = true) { m_usePathFinding = use; }
    void setRepathCoef(double coef) { m_repathCoef = coef; }

    bool isCollectStat() const { return m_iscollectStat; }
    bool isUsePathFinding() const { return m_usePathFinding; }

    inline MathParams getMathParams() { return m_param; }

    QVector<double> buildAStarMatrix(int & height, int & width);

    QVector2D getNearestExit(const Agent &agent);

    double getGridStep() const { return m_gridStep; }
    double getPanicLevel() const { return m_panicLevel; }

    int getPathAlgorithmIndex() { return pathAlgorithmIndex; }

public slots:
    void pathAlgorithmChangedSlot(int index);
    void changePanicLevelSlot(double panicLevel);
    void buildPathfinfingMatrixSlot();

signals:
    void sendStatSignal(const Agent & agent, double force);
    void removeAgentSignal(const Agent& agent);
    void enterAgentSignal();
    
private:

    double m_gridStep = 1;
    double m_checkpointRadius = 1;
    double m_repathCoef = 2;

    QVector2D m_sceneSize; // meters

    std::shared_ptr<ObjectsPool> m_pool;
    QMap<int, ASTAR::AStar> m_pathfinding;

    bool m_iscollectStat = true;
    bool m_usePathFinding = true;

    double m_time;
    double m_physicalForcesAgentSum = 0;
    double m_panicLevel = 0;

    int m_iterations = 0;
    int m_newId = 1000000;

    MathParams m_param;

    int pathAlgorithmIndex = 0;

    int isInObstacle(double x, double y);

    void move(Agent &agent);
    void entryProcess();
    void calcForce(Agent &agent);

    bool isInExit(const Agent &agent);

    QVector2D calcPanicForce(const Agent &agent);
    QVector2D calcNormal(QVector2D a, QVector2D b);
    QVector2D calcTau(QVector2D n, QVector2D speed);
    QVector2D calcCrossAgentForce(const Agent &agent);
    QVector2D calcWallForce(const Agent &agent);
    QVector2D getNearestPointOfObstacle(const Agent &agent, const Obstacle &obstacle);
    QVector2D getPointOnLine(QVector2D a, QVector2D b);

    std::vector<Checkpoint> getPath(const Agent &agent);

    double getDistanceToSide(const QVector2D &a, const QVector2D &b, const Agent &agent, QVector2D &result);
    double scalarMultiplication(const QVector2D &a, const QVector2D &b);
    double distanceBetweenPoints(const QVector2D &a, const QVector2D &b);
    double getRandomNumber(double a, double b);

    int Heaviside(double n);

    void updateWishSpeeds();
};

#endif // CALCULATOR_H
