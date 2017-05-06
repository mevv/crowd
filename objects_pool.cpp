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
    for (size_t i = 0; i < 10; i++)

//        Agent(int id, double size, double mass, QVector2D position, QVector2D speed, QVector2D acceleration, QColor color);
        this->addAgent(Agent(i, 5, 1,
                             QVector2D(200, 200),
                             QVector2D(std::rand() % 50 + 1,std::rand() % 50 + 1),
                             QVector2D(),
                             QColor(i%255, (i*2)%255, (i*3)%255)));

//    this->addObstacle(Obstacle(1, QVector2D(10, 10),QColor(), {QPoint(100, 0), QPoint(100, 100), QPoint(0, 100)}));


    this->addExit(Exit(1, QVector2D(0, 200), QColor(), QVector2D(0, 300)));

    this->addExit(Exit(1, QVector2D(300, 200), QColor(), QVector2D(300, 300)));


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

        bool isFirst = true;
        int first_x = 0;
        int first_y = 0;

        for(auto j : tmp)
        {
            if(isFirst)
            {
                first_x = j.toObject().value(QString("x")).toInt();
                first_y = j.toObject().value(QString("y")).toInt();
                isFirst = false;
            }
            else
                apexes.append(QPoint(j.toObject().value(QString("x")).toInt() - first_x,
                                     j.toObject().value(QString("y")).toInt() - first_y ));
        }

        this->addObstacle(Obstacle(n, QVector2D(first_x,first_y), QColor(), apexes));
        n++;
    }

    for(auto i : exits)
    {
        QJsonObject tmp = i.toObject();
        QJsonObject a = tmp.value(QString("a")).toObject();
        QJsonObject b = tmp.value(QString("b")).toObject();

        this->addExit(Exit(n,
                           QVector2D(a.value(QString("x")).toInt(), a.value(QString("y")).toInt()),
                           QColor(),
                           QVector2D(b.value(QString("x")).toInt(), b.value(QString("y")).toInt())));
        n++;
    }
}

void ObjectsPool::removeAgent(Agent &agent)
{
    m_agents.erase(std::remove(m_agents.begin(), m_agents.end(), agent), m_agents.end());
    qDebug() << ++deleted;
}
