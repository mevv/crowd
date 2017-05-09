#include "engine.h"

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

Engine::Engine()
{
    m_timer.reset(new QTimer(this));

    m_objects_pool.reset(new ObjectsPool());

    std::srand(time(0));

    QPoint sceneRealSize(500, 500);

    m_scene.reset(new Scene(sceneRealSize, m_objects_pool));
    m_calculator.reset(new Calculator(sceneRealSize ,m_objects_pool));

//    qDebug() << "Pool count:" << m_objects_pool.get()->use_count();

    connect(m_timer.get(),  &QTimer::timeout, this, [this]{ this->update(); });
    connect(m_objects_pool.get(), &ObjectsPool::endOfSimulation, this, &Engine::finishSimulation);

    m_timer->setInterval(m_timerTick);
}

void Engine::update()
{
    if (m_isMouseMove)
    {
        m_scene->moveBy(QCursor::pos() - m_mousePrevPos);
        m_mousePrevPos = QCursor::pos();
    }

    //if (m_timer->remainingTime() > 0)
    {
        m_simulationTime += m_timerTick;
        auto tmpMoveRecord = this->m_calculator->update(m_timerTick);
        m_moveRecord.push_back(tmpMoveRecord);

    }

    emit tick();
}

void Engine::draw(QPainter& painter)
{
    m_scene->draw(painter);
}

void Engine::pause()
{
    m_timer->stop();
}

void Engine::resume()
{
    m_timer->setInterval(m_timerTick);
    m_timer->start();
}

void Engine::scrollEvent(QWheelEvent * event)
{
    m_scene->setScale(event->delta() / 10000.0);
    m_timer->singleShot(0, [this]{ this->update(); });
}

void Engine::mouseClickEvent(QMouseEvent * event)
{
    m_isMouseMove = true;
    m_mousePrevPos = QCursor::pos();

    if (!m_timer->isActive())
    {
        m_timer->setInterval(0);
        m_timer->start();
    }
}

void Engine::mouseReleaseEvent(QMouseEvent * event)
{
    m_isMouseMove = false;

    if (m_timer->interval() == 0)
        m_timer->stop();
}

void Engine::setSchemeFileName(QString filename)
{
    this->m_schemeFileName = filename;
    readSchemeFromFile(this->m_schemeFileName);

}

void Engine::setSaveFileName(QString filename)
{
    this->m_saveFileName = filename;

}

void Engine::finishSimulation()
{
    this->pause();
    emit enableStatButton();
    writeRecordToFile();
}

void Engine::readSchemeFromFile(QString filename)
{
    /* DUMMY INIT*/
//    for (size_t i = 0; i < 10; i++)
//    {

//        Agent(int id, double size, double mass, QVector2D position, QVector2D speed, QVector2D acceleration, QColor color);
//        this->addAgent(Agent(i, 5, 1,
//                             QVector2D(200, 200),
//                             QVector2D(std::rand() % 50 + 1,std::rand() % 50 + 1),
//                             QVector2D(),
//                             QColor(i%255, (i*2)%255, (i*3)%255)));
        //}


//    this->addObstacle(Obstacle(1, QVector2D(10, 10),QColor(), {QPoint(100, 0), QPoint(100, 100), QPoint(0, 100)}));

//    m_objects_pool->addExit(Exit(1, QVector2D(1, 200), QColor(), QVector2D(1, 300)));
//    m_objects_pool->addExit(Exit(1, QVector2D(300, 200), QColor(), QVector2D(300, 300)));


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
    QJsonArray agents = mainData.value(QString("agents")).toArray();
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

        m_objects_pool->addObstacle(Obstacle(n, QVector2D(first_x,first_y), QColor(), apexes));
        n++;
    }

    for(auto i : exits)
    {
        QJsonObject tmp = i.toObject();
        QJsonObject a = tmp.value(QString("a")).toObject();
        QJsonObject b = tmp.value(QString("b")).toObject();

        m_objects_pool->addExit(Exit(n,
                           QVector2D(a.value(QString("x")).toInt(), a.value(QString("y")).toInt()),
                           QColor(),
                           QVector2D(b.value(QString("x")).toInt(), b.value(QString("y")).toInt())));
        n++;
    }

    for(auto i : agents)
    {
        QJsonObject tmp = i.toObject();
        QJsonObject a =  tmp.value(QString("a")).toObject();
        QJsonObject b =  tmp.value(QString("b")).toObject();

        int n = 0;
        for(int j  = a.value(QString("x")).toInt(); j < b.value(QString("x")).toInt(); j += 5)
        {
            for(int k  = a.value(QString("y")).toInt(); k < b.value(QString("y")).toInt(); k += 5)
            {
                n++;
                m_objects_pool->addAgent(Agent(n, 5, 1,
                                     QVector2D(j, k),
                                     QVector2D(std::rand() % 50 + 1,std::rand() % 50 + 1),
                                     QVector2D(),
                                     QColor(n%255, (n*2)%255, (n*3)%255)));
            }

        }

        qDebug() << n;
    }
}

void Engine::writeRecordToFile()
{

//     QJsonArray ** data = new QJsonArray*[m_moveRecord.size()];

//     int i = 0;
//     for(auto frame : m_moveRecord)
//     {
//         QVector2D * tmp = &frame[0];
//         QVector2D tmp2 [frame.size()] = *tmp;
//         data[i] = new QJsonArray(tmp2);
//         i++;
//     }




    QFile file;
    file.setFileName(m_saveFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream stream(&file);
//    stream << js_data << endl;
    file.close();



}
