#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>

#include "scene.h"
#include "calculator.h"
#include "objects_pool.h"

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine();

    bool isStarted() const { return m_timer->isActive(); }

    void update();

    void draw(QPainter& painter);

    void pause();

    void resume();

    void scrollEvent(QWheelEvent * event);

signals:
    void tick();


private:
    int timerTick = 10;//milliseconds

    std::unique_ptr<QTimer> m_timer;
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<Calculator> m_calculator;
};

#endif // ENGINE_H
