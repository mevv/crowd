#include "engine.h"

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

#include "plan_builder.h"

Engine::Engine()
{
    m_timer.reset(new QTimer(this));

    m_objects_pool.reset(new ObjectsPool());

    std::srand(time(0));

    //readSchemeFromFile(m_schemeFileName);

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

void Engine::loadPlan(QString filename)
{
    /* DUMMY INIT*/
    for (size_t i = 0; i < 10; i++)
    {
        m_objects_pool->addAgent(Agent(i, 5, 1,
                             QVector2D(10+i*20, 50),
                             QVector2D(0, 0),
                             QVector2D(),
                             QColor(i%255, (i*2)%255, (i*3)%255)));
     }


    //m_objects_pool->addObstacle(Obstacle(1, QVector2D(100, 100),QColor(), {QPoint(-90, 300), QPoint(300, 300), QPoint(300, 100)}));

//    m_objects_pool->addExit(Exit(1, QVector2D(0, 500), QColor(), QVector2D(50, 500)));
//    m_objects_pool->addExit(Exit(1, QVector2D(400, 200), QColor(), QVector2D(400, 300)));


    /* INIT FROM FILE*/

    QFile file;
    QString val;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject mainData = sd.object();

    QJsonObject size = mainData.value(QString("size")).toObject();

    PlanBuilder::buildObjectsPool(mainData, *m_objects_pool);

    int n = 10;

    // OBSTACLES BEHIND THE WALLS

    // left wall
    m_objects_pool->addObstacle(Obstacle(m_objects_pool->getObstacles().size(),
                                         QVector2D(0,0),
                                         QColor(n%255, (n*2)%255, (n*3)%255),
                                         {QPoint(0, size.value(QString("x")).toInt())}
                                         ));

    // top wall
    m_objects_pool->addObstacle(Obstacle(m_objects_pool->getObstacles().size(),
                                         QVector2D(0,0),
                                         QColor(n%255, (n*2)%255, (n*3)%255),
                                         {QPoint(size.value(QString("y")).toInt(), 0)}
                                         ));
    // right wall
    m_objects_pool->addObstacle(Obstacle(m_objects_pool->getObstacles().size(),
                                         QVector2D(size.value(QString("x")).toInt(),0),
                                         QColor(n%255, (n*2)%255, (n*3)%255),
                                         {QPoint(0, size.value(QString("y")).toInt())}
                                         ));
    // bottom wall
    m_objects_pool->addObstacle(Obstacle(m_objects_pool->getObstacles().size(),
                                         QVector2D(0, size.value(QString("y")).toInt()),
                                         QColor(n%255, (n*2)%255, (n*3)%255),
                                         {QPoint(size.value(QString("x")).toInt(), 0)}
                                         ));

}

void Engine::finishSimulation()
{
    this->pause();
    emit enableStatButton();
    writeRecordToFile();
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
    file.setFileName("");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream stream(&file);
//    stream << js_data << endl;
    file.close();



}
