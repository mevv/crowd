#include "engine.h"

#include <QWidget>

Engine::Engine()
{
    m_timer.reset(new QTimer(this));

    std::shared_ptr<ObjectsPool> pool(new ObjectsPool());

    std::srand(time(0));

    for (size_t i = 0; i < 10000; i++)
        pool->addAgent(Agent(i, 5, 1,
                             QVector2D(200, 200),
                             QVector2D(std::rand() % 50 + 1,std::rand() % 50 + 1),
                             QVector2D(),
                             QColor(i%255, (i*2)%255, (i*3)%255)));

    pool->addObstacle(Obstacle(1, QVector2D(10, 10),QColor(), {QPoint(100, 0), QPoint(100, 100), QPoint(0, 100)}));

    QPoint sceneRealSize(500, 500);

    m_scene.reset(new Scene(sceneRealSize, pool));
    m_calculator.reset(new Calculator(sceneRealSize ,pool));

    qDebug() << "Pool count:" << pool.use_count();

    connect(m_timer.get(),  &QTimer::timeout, this, [this]{ this->update(); });

    m_timer->setInterval(timerTick);
    //m_timer->start();
}

void Engine::update()
{
    if (m_isMouseMove)
    {
        m_scene->moveBy(QCursor::pos() - m_mousePrevPos);
        m_mousePrevPos = QCursor::pos();
    }

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
