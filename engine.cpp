#include "engine.h"

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>


#include "plan_builder.h"
#include "general_builder.h"
#include "json_manager.h"

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

    m_stat_thread.reset(new QThread());
    m_stat.reset(new Statistics());
    m_stat->moveToThread(m_stat_thread.get());
    m_stat_thread->start();
    connect(this, &Engine::startSimulation, m_stat.get(), &Statistics::simulationStart);
//    connect(m_calculator, &Calculator::agentStat, m_stat.get(), &Statistics::gather_info);
    connect(m_calculator.get(), &Calculator::removeAgentSignal, m_stat.get(), &Statistics::agent_quit);
}

Engine::~Engine()
{

}

void Engine::update(bool isTimeRun)
{
    if (m_isMouseMove)
    {
        m_scene->moveBy(QCursor::pos() - m_mousePrevPos);
        m_mousePrevPos = QCursor::pos();
    }

    if (isTimeRun && !m_isMouseMove )
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
    m_timer->singleShot(0, [this]{ this->update(false); });
}

void Engine::mouseClickEvent(QMouseEvent *event)
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
    this->clear();

    /* DUMMY INIT*/
    for (size_t i = 0; i < 3; i++)
    {
        m_objects_pool->addAgent(Agent(i, 0.1, 80,
                             QVector2D(1.0 + i, 0.5),
                             QVector2D(0, 0),
                             QColor(0, 0, 0),
                             5));
        qDebug() << "agent";
     }


    //m_objects_pool->addObstacle(Obstacle(1, QVector2D(100, 100),QColor(), {QPoint(-90, 300), QPoint(300, 300), QPoint(300, 100)}));

//    m_objects_pool->addExit(Exit(1, QVector2D(0, 500), QColor(), QVector2D(50, 500)));
//    m_objects_pool->addExit(Exit(1, QVector2D(400, 200), QColor(), QVector2D(400, 300)));


    /* INIT FROM FILE*/


    QJsonObject planData = JsonManager::parseJsonFile(filename);
    QJsonObject configData = JsonManager::parseJsonFile(PATH_TO_CONF);

    QJsonObject size = planData.value(QString("size")).toObject();
    m_scene->setSize(QPoint(size.value("x").toDouble(), size.value("y").toDouble()));
    m_scene->setScale(50);
    qDebug() << configData;
    PlanBuilder::buildObjectsPool(planData, *m_objects_pool);
    GeneralBuilder::buildCalculator(configData, *m_calculator);
    GeneralBuilder::buildAgents(configData, *m_objects_pool);

    int n = 10;

    // OBSTACLES BEHIND THE WALLS

    // left wall
//    m_objects_pool->addObstacle(Obstacle(m_objects_pool->getObstacles().size(),
//                                         QVector2D(0,0),
//                                         QColor(n%255, (n*2)%255, (n*3)%255),
//                                         {QPoint(0, size.value(QString("x")).toInt())}
//                                         ));

//    // top wall
//    m_objects_pool->addObstacle(Obstacle(m_objects_pool->getObstacles().size(),
//                                         QVector2D(0,0),
//                                         QColor(n%255, (n*2)%255, (n*3)%255),
//                                         {QPoint(size.value(QString("y")).toInt(), 0)}
//                                         ));
//    // right wall
//    m_objects_pool->addObstacle(Obstacle(m_objects_pool->getObstacles().size(),
//                                         QVector2D(size.value(QString("x")).toInt(),0),
//                                         QColor(n%255, (n*2)%255, (n*3)%255),
//                                         {QPoint(0, size.value(QString("y")).toInt())}
//                                         ));
//    // bottom wall
//    m_objects_pool->addObstacle(Obstacle(m_objects_pool->getObstacles().size(),
//                                         QVector2D(0, size.value(QString("y")).toInt()),
//                                         QColor(n%255, (n*2)%255, (n*3)%255),
//                                         {QPoint(size.value(QString("x")).toInt(), 0)}
//                                         ));

    m_timer->singleShot(0, [this]{ this->update(false); });
}

void Engine::finishSimulation()
{
    this->pause();
    emit enableStatButton();
    writeRecordToFile();
}

void Engine::writeRecordToFile()
{
    QFile file;
    file.setFileName("");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream stream(&file);
//    stream << js_data << endl;
    file.close();
}

void Engine::clear()
{
    m_timer->stop();
    m_objects_pool->clear();
    m_simulationTime = 0;
    m_timer->singleShot(0, [this]{ this->update(false); });
}

void Engine::startSimulationSlot()
{
    emit startSimulation(m_objects_pool->getAgents().size());
}
