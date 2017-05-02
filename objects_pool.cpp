#include "objects_pool.h"
#include <QDebug>

ObjectsPool::ObjectsPool()
{

}

ObjectsPool::~ObjectsPool()
{
    qDebug() << "~ObjectsPool()";
    m_agents.clear();
}

void ObjectsPool::initFromFile(QString filename)
{
    /* DUMMY INIT*/
    for (size_t i = 0; i < 10000; i++)
        this->addAgent(Agent(i, 5, 1,
                             QVector2D(200, 200),
                             QVector2D(std::rand() % 50 + 1,std::rand() % 50 + 1),
                             QVector2D(),
                             QColor(i%255, (i*2)%255, (i*3)%255)));

    this->addObstacle(Obstacle(1, QVector2D(10, 10),QColor(), {QPoint(100, 0), QPoint(100, 100), QPoint(0, 100)}));


    this->addExit(Exit(1, QVector2D(10, 200), QColor(), QVector2D(10, 300)));

    /* INIT FROM FILE*/

    QFile file;
    QString val;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject mainData = sd.object();
    QJsonArray obstacles = mainData.value(QString("obstacles")).toArray();
    QJsonArray exits = mainData.value(QString("exits")).toArray();
    QJsonObject size = mainData.value(QString("size")).toObject();



    int n = 0;
    for(auto i : obstacles)
    {
        QJsonArray tmp = i.toArray();
        QVector<QPoint> apexes;
        for(auto j : tmp)
        {
            apexes.append(QPoint(j.toObject().value(QString("x")).toInt(), j.toObject().value(QString("y")).toInt() ));
        }

                                      // |       | //
                                      // V DUMMY V //
        this->addObstacle(Obstacle(n, QVector2D(0, 0), QColor(), apexes));
        n++;
    }

    for(auto i : exits)
    {
        qDebug() << i.value(QString("a"));
//        Exit(int id, QVector2D pos, QColor color, QVector2D end);

//        this->addExit(Exit(n, QVector2D(0, 0), QColor(), apexes));
        n++;
    }

}
