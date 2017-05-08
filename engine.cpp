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

    m_objects_pool->initFromFile(this->m_schemeFileName);

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
    m_objects_pool->initFromFile(this->m_schemeFileName);

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
