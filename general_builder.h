#ifndef GENERALBUILDER_H
#define GENERALBUILDER_H

#include <QJsonObject>

#include <utility>

#include "calculator.h"
#include "a_star.h"


class GeneralBuilder
{
public:
    GeneralBuilder();

    static bool buildCalculator(const QJsonObject& settings, Calculator& calculator);

    static bool buildAgents(const QJsonObject& settings, ObjectsPool& pool);

    static Agent buildSingleAgent(const QJsonObject& settings, QVector2D pos, QVector2D speedDir);

    static bool buildCheckPoints(QJsonObject& settings, ObjectsPool& pool, Calculator& calculator);

    static bool buildCheckPointsForSingleAgent(QJsonObject& settings, ObjectsPool& pool, Calculator& calculator, const Agent& agent);

private:
    static double getRandomNumber(double a, double b);

    static inline double distanceBetweenPoints(const QPoint &a, const QPoint &b)
    {
        return sqrt(pow(b.x() - a.x(), 2) + pow( b.y() - a.y(), 2) );
    }

};

#endif // GENERALBUILDER_H
