#ifndef GENERALBUILDER_H
#define GENERALBUILDER_H

#include <QJsonObject>

#include "calculator.h"

class GeneralBuilder
{
public:
    GeneralBuilder();

    static bool buildCalculator(const QJsonObject& settings, Calculator& calculator);

    static bool buildAgents(const QJsonObject& settings, ObjectsPool& pool);

private:
    static int getRandomNumber(int a, int b);

    static inline double distanceBetweenPoints(const QPoint &a, const QPoint &b)
    {
        return sqrt(pow(b.x() - a.x(), 2) + pow( b.y() - a.y(), 2) );
    }

};

#endif // GENERALBUILDER_H
