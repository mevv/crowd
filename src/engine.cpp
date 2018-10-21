#include "engine.h"

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QMetaType>


#include "plan_builder.h"
#include "general_builder.h"
#include "json_manager.h"
#include "agent.h"

//Q_DECLARE_METATYPE(Agent)

Engine::Engine()
{
    m_timer.reset(new QTimer(this));

    m_objects_pool.reset(new ObjectsPool());

    std::srand(time(0));

    QVector2D sceneRealSize(0, 0);

    m_scene.reset(new Scene(sceneRealSize, m_objects_pool));
    m_calculator.reset(new Calculator(sceneRealSize ,m_objects_pool));

    connect(m_timer.get(),  &QTimer::timeout, this, [this]{ this->update(); });
    connect(m_objects_pool.get(), &ObjectsPool::endOfSimulation, this, &Engine::finishSimulation);

    m_timer->setInterval(m_simulationDelay);

    m_stat_thread.reset(new QThread());
    m_stat.reset(new Statistics());
    m_stat->moveToThread(m_stat_thread.get());
    m_stat_thread->start();

    connect(this, &Engine::startSimulation, m_stat.get(), &Statistics::simulationStartSlot);
    connect(m_calculator.get(), &Calculator::removeAgentSignal, m_stat.get(), &Statistics::agentQuitSlot);

    qRegisterMetaType<Agent>("Agent");
    connect(m_calculator.get(), &Calculator::sendStatSignal, m_stat.get(), &Statistics::gatherInfoSlot);
}

Engine::~Engine()
{
    m_stat_thread->quit();
    m_stat_thread->wait();
}

void Engine::update(bool isTimeRun)
{
    if (m_isMouseMove)
    {
        m_scene->moveBy(QCursor::pos() - m_mousePrevPos);
        m_mousePrevPos = QCursor::pos();
    }

    if (isTimeRun && !m_isMouseMove)
    {
        m_simulationTime += m_simulationStep;
        //QTime start = QTime::currentTime();
        this->m_calculator->update(m_simulationStep);
        //qDebug() << "calc update elapsed:" << start.elapsed();
    }

    emit updateAgentInRoomSignal(m_objects_pool->getAgents().size());

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
    m_stat->reset();
    m_timer->setInterval(m_simulationDelay);
    emit startSimulation(0);
    m_timer->start();
}

void Engine::scrollEvent(QWheelEvent * event)
{
    double scale = (event->delta() > 0) ? event->delta() * SCALE_SENSIVITY : 1.0 / abs(event->delta()) / SCALE_SENSIVITY;

    if (scale > 1.2)
        scale = 1.005;
    if (scale < 0.8)
        scale = 0.995;

    m_scene->scale(scale);

    m_timer->singleShot(0, [this]{ this->update(false); });

    emit changeScaleSignal(scale);
}

void Engine::mouseClickEvent(QMouseEvent*)
{
    m_isMouseMove = true;
    m_mousePrevPos = QCursor::pos();
    m_simRunOnMouseClick = m_timer->isActive();

    if (!m_simRunOnMouseClick)
    {
        m_timer->setInterval(0);
        m_timer->start();
    }
}

void Engine::mouseReleaseEvent(QMouseEvent*)
{
    m_isMouseMove = false;

    if (!m_simRunOnMouseClick)
        m_timer->stop();
}

void Engine::loadPlan(QString filename)
{
    this->clear();
    m_lastPlanFilePath = filename;

    QJsonObject planData = JsonManager::parseJsonFile(filename);
    QJsonObject configData = JsonManager::parseJsonFile(JsonManager::getConfPath());

    QJsonObject size = planData.value(QString("size")).toObject();
    m_scene->setSize(QVector2D(size.value("x").toDouble(), size.value("y").toDouble()));
    m_scene->setScale(50);

    qDebug() << configData;

    PlanBuilder::buildObjectsPool(planData, *m_objects_pool);
    PlanBuilder::buildCalculator(planData, *m_calculator);
    GeneralBuilder::buildCalculator(configData, *m_calculator);
    GeneralBuilder::buildAgents(configData, *m_objects_pool, m_calculator->getPanicLevel());

    if (m_calculator->isUsePathFinding())
        GeneralBuilder::buildCheckPoints(configData, *m_objects_pool, *m_calculator, m_calculator->getPathAlgorithmIndex());

    m_timer->singleShot(0, [this]{ this->update(false); });
}

void Engine::finishSimulation()
{
    this->pause();

    if (m_calculator->isCollectStat())
        emit sendStatReportSignal(m_stat->getReport());
}

void Engine::clear()
{
    m_timer->stop();
    m_objects_pool->clear();
    m_stat->reset();
    m_simulationTime = 0;
    m_timer->singleShot(0, [this]{ this->update(false); });
}

void Engine::reset()
{
    this->clear();

    if (m_lastPlanFilePath.size() > 0)
        loadPlan(m_lastPlanFilePath);
}

void Engine::startSimulationSlot()
{
    emit startSimulation(m_objects_pool->getAgents().size());
}
