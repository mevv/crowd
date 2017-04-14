#include "engine.h"

Engine::Engine()
{
    m_timer.reset(new QTimer(this));

    std::shared_ptr<ObjectsPool> pool(new ObjectsPool());

    std::srand(time(0));

    for (size_t i = 0; i < 10000; i++)
        pool->addAgent(Agent(i, 5, 1, QVector2D(200, 200), QVector2D(std::rand() % 50 + 1, std::rand() % 50 + 1), QVector2D(), QColor()));

    pool->addObstacle(Obstacle(1, QVector2D(10, 10),QColor(), {QPoint(100, 0), QPoint(100, 100), QPoint(0, 100)}));

    QPoint sceneRealSize(100, 100);



    m_scene.reset(new Scene(QPoint(100, 100), pool));
    m_calculator.reset(new Calculator(pool));

    qDebug() << "Pool count:" << pool.use_count();

    connect(m_timer.get(),  &QTimer::timeout, this, [this]{ this->update(); });

    m_timer->setInterval(timerTick);
    //m_timer->start();
}

void Engine::update()
{
    if (m_timer->remainingTime() > 0)
        this->m_calculator->update(m_timer->remainingTime());

    emit tick();
}

void Engine::draw(QPainter& painter)
{
    m_scene->draw(painter);
}

void Engine::pause()
{
    if (m_timer->isActive())
        m_timer->stop();
}

void Engine::resume()
{
    m_timer->setInterval(timerTick);
    m_timer->start();
}

void Engine::scrollEvent(QWheelEvent * event)
{
    m_scene->setScale(event->delta() / 10000.0);
    m_timer->singleShot(0, [this]{ this->update(); });
}
