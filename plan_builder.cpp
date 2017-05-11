#include "plan_builder.h"

#include <QDebug>

PlanBuilder::PlanBuilder()
{

}

bool PlanBuilder::buildObjectsPool(const QJsonObject& plan, ObjectsPool& pool)
{
    QJsonArray obstacles = plan.value(QString("obstacles")).toArray();
    QJsonArray agents = plan.value(QString("spawn_zones")).toArray();
    QJsonArray exits = plan.value(QString("exits")).toArray();
    QJsonObject size = plan.value(QString("size")).toObject();

    int n = 0;

    buildObstacles(obstacles, pool);
    buildExits(exits, pool);

}

bool PlanBuilder::buildObstacles(const QJsonArray& obstacles, ObjectsPool& pool)
{
    qDebug() << obstacles;
    int id = 0;
    for(auto i : obstacles)
    {
        QVector<QPoint> vertexes;
        QJsonObject obstacle = i.toObject();

        QVector2D position(obstacle.value("position").toObject().value("x").toDouble(),
                           obstacle.value("position").toObject().value("y").toDouble());

        QJsonArray jsonVertexes = obstacle.value("vertexes").toArray();

        vertexes.push_back(QPoint(0, 0));

        for (auto j : jsonVertexes)
            vertexes.push_back(QPoint(j.toObject().value("x").toDouble(),
                                      j.toObject().value("y").toDouble()));

        pool.addObstacle(Obstacle(id, position, QColor(), vertexes));

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
        QJsonObject a = tmp.value(QString("a")).toObject();
        QJsonObject b = tmp.value(QString("b")).toObject();

        pool.addExit(Exit(id, QVector2D(a.value("x").toInt(), a.value("y").toDouble()),
                             QColor(),
                             QVector2D(b.value("x").toInt(), b.value("y").toDouble())));
        id++;
    }
}
