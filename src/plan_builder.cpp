#include "plan_builder.h"

#include <QDebug>

PlanBuilder::PlanBuilder()
{

}

bool PlanBuilder::buildObjectsPool(const QJsonObject& plan, ObjectsPool& pool)
{
    bool result = true;

    QJsonArray obstacles = plan.value(QString("obstacles")).toArray();
    QJsonArray spawn_zones = plan.value(QString("spawn_zones")).toArray();
    QJsonArray exits = plan.value(QString("exits")).toArray();
    QJsonArray entries = plan.value(QString("entries")).toArray();
    QJsonObject size = plan.value(QString("size")).toObject();

    result &= buildObstacles(obstacles, pool);
    result &= buildExits(exits, pool);
    result &= buildEntries(entries, pool);
    result &= buildSpawnZones(spawn_zones, pool);

    return result;
}

bool PlanBuilder::buildCalculator(const QJsonObject& plan, Calculator& calculator)
{
    calculator.setSceneSize(QVector2D(plan.value("size").toObject().value("x").toDouble(),
                                      plan.value("size").toObject().value("y").toDouble()));

    return true;
}

bool PlanBuilder::buildObstacles(const QJsonArray& obstacles, ObjectsPool& pool)
{
    int id = 0;
    for(auto i : obstacles)
    {
        QVector<QVector2D> vertexes;
        QJsonObject obstacle = i.toObject();

        QVector2D position(obstacle.value("position").toObject().value("x").toDouble(),
                           obstacle.value("position").toObject().value("y").toDouble());

        QJsonArray jsonVertexes = obstacle.value("vertexes").toArray();

        vertexes.push_back(QVector2D(0, 0));

        for (auto j : jsonVertexes)
            vertexes.push_back(QVector2D(j.toObject().value("x").toDouble(),
                                      j.toObject().value("y").toDouble()));

        pool.addObstacle(Obstacle(id, position, QColor(obstacle.value("color").toObject().value("R").toInt(),
                                                       obstacle.value("color").toObject().value("G").toInt(),
                                                       obstacle.value("color").toObject().value("B").toInt(),
                                                       obstacle.value("color").toObject().value("A").toInt()), vertexes));
        id++;
    }

    return true;
}

bool PlanBuilder::buildExits(const QJsonArray& exits, ObjectsPool& pool)
{
    int id = 0;

    for(auto i : exits)
    {
        QJsonObject tmp = i.toObject();
        QJsonObject a = tmp.value(QString("begin")).toObject();
        QJsonObject b = tmp.value(QString("end")).toObject();

        pool.addExit(Exit(id, QVector2D(a.value("x").toDouble(), a.value("y").toDouble()),
                             QColor(tmp.value("color").toObject().value("R").toInt(),
                                    tmp.value("color").toObject().value("G").toInt(),
                                    tmp.value("color").toObject().value("B").toInt(),
                                    tmp.value("color").toObject().value("A").toInt()),
                             QVector2D(b.value("x").toDouble(), b.value("y").toDouble())));
        id++;
    }

    return true;
}

bool PlanBuilder::buildEntries(const QJsonArray& entries, ObjectsPool& pool)
{
    int id = 0;

    for (auto i : entries)
    {
        QJsonObject tmp = i.toObject();

        pool.addEntry(Entry(id,
                            QVector2D(tmp.value("begin").toObject().value("x").toDouble(),
                                      tmp.value("begin").toObject().value("y").toDouble()),
                            QColor(tmp.value("color").toObject().value("R").toInt(),
                                   tmp.value("color").toObject().value("G").toInt(),
                                   tmp.value("color").toObject().value("B").toInt(),
                                   tmp.value("color").toObject().value("A").toInt()),
                            QVector2D(tmp.value("end").toObject().value("x").toDouble(),
                                      tmp.value("end").toObject().value("y").toDouble())
                       ));
        id++;
    }

    return true;
}

bool PlanBuilder::buildSpawnZones(const QJsonArray& spawnZones, ObjectsPool& pool)
{
    int id = 0;

    for (auto i : spawnZones)
    {
        QJsonObject tmp = i.toObject();

        pool.addSpawnZone(SpawnZone(id,
                                    QVector2D(tmp.value("a").toObject().value("x").toDouble(),
                                              tmp.value("a").toObject().value("y").toDouble()),
                                    QColor(),
                                    QPoint(tmp.value("b").toObject().value("x").toDouble(),
                                           tmp.value("b").toObject().value("y").toDouble())
                                    ));
        id++;
    }

    return true;
}
