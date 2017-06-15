#ifndef GENERALBUILDER_H
#define GENERALBUILDER_H

#include <QJsonObject>

#include <utility>

#include "calculator.h"
#include "a_star.h"
#include "lee.h"

const double DISTANCE_BETWEEN_AGENTS = 3.0;

class GeneralBuilder
{
public:
    GeneralBuilder();

    static bool buildCalculator(const QJsonObject& settings, Calculator& calculator);

    static bool buildAgents(const QJsonObject& settings, ObjectsPool& pool, double panicLevel);

    static Agent buildSingleAgent(const QJsonObject& settings, QVector2D pos, QVector2D speedDir, double panicLevel);

    static bool buildCheckPoints(QJsonObject& settings,
                                 ObjectsPool& pool,
                                 Calculator& calculator,
                                 const int pathAlgorithmIndex);

    static bool buildCheckPointsForSingleAgent(QJsonObject& settings,
                                               ObjectsPool& pool,
                                               Calculator& calculator,
                                               const Agent& agent,
                                               const int pathAlgorithmIndex);

private:
    static double getRandomNumber(double a, double b);

    static inline double distanceBetweenPoints(const QPoint &a, const QPoint &b)
    {
        return sqrt(pow(b.x() - a.x(), 2) + pow( b.y() - a.y(), 2) );
    }

};

#endif // GENERALBUILDER_H
