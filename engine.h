#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>
#include <QThread>

#include "scene.h"
#include "calculator.h"
#include "objects_pool.h"
#include "statistics.h"

#include <chrono>

const double SCALE_SENSIVITY = 0.01;

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine();

    ~Engine();

    bool isStarted() const { return m_timer->isActive(); }

    int getSimulationTime() const { return m_simulationTime; }

    int getTimerTick() const { return m_timerTick; }

    void update(bool isTimeRun = true);

    void draw(QPainter& painter);

    void pause();

    void resume();

    void scrollEvent(QWheelEvent * event);

    void mouseClickEvent(QMouseEvent * event);

    void mouseReleaseEvent(QMouseEvent * event);

signals:
    void tick();
    void enableStatButton();
    void startSimulation(int n);

    void sendStatReportSignal(QString report);
    void changeScaleSignal(double scale);

public slots:
    void loadPlan(QString filename);

    void finishSimulation();

    void clear();

    void startSimulationSlot();

private:
    int m_timerTick = 10;//milliseconds
    int m_simulationTime = 0;

    bool m_isMouseMove = false;

    QPoint m_mousePrevPos;

    QString m_lastPlanFilePath;

    std::chrono::time_point<std::chrono::system_clock> m_timeFrame;
    std::unique_ptr<QTimer> m_timer;
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<Calculator> m_calculator;
    std::shared_ptr<ObjectsPool> m_objects_pool;
    std::unique_ptr<Statistics> m_stat;
    std::unique_ptr<QThread>m_stat_thread;

    std::vector<std::vector<QVector2D>> m_moveRecord;

    void writeRecordToFile();

    void readSchemeFromFile(QString filename);
};

#endif // ENGINE_H
