#ifndef PLANBUILDER_H
#define PLANBUILDER_H

#include <QJsonObject>
#include <QJsonArray>

#include "objects_pool.h"

class PlanBuilder
{
public:
    PlanBuilder();

    static bool buildObjectsPool(const QJsonObject& plan, ObjectsPool& pool);

private:
    static bool buildObstacles(const QJsonArray& obstacles, ObjectsPool& pool);
    static bool buildExits(const QJsonArray& exits, ObjectsPool& pool);
    static bool buildEntries(const QJsonArray& entries, ObjectsPool& pool);
};

#endif // PLANBUILDER_H
