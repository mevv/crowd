#include "engine.h"

#include <QWidget>

Engine::Engine()
{
    m_timer.reset(new QTimer(this));

    m_objects_pool.reset(new ObjectsPool());

    m_objects_pool->initFromFile(this->schemeFileName);

    std::srand(time(0));

    QPoint sceneRealSize(500, 500);

    m_scene.reset(new Scene(sceneRealSize, m_objects_pool));
    m_calculator.reset(new Calculator(sceneRealSize ,m_objects_pool));

//    qDebug() << "Pool count:" << m_objects_pool.get()->use_count();

    connect(m_timer.get(),  &QTimer::timeout, this, [this]{ this->update(); });

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
        this->m_calculator->update(m_timerTick);

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
    this->schemeFileName = filename;
    m_objects_pool->initFromFile(this->schemeFileName);

}

