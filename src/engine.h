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

    void setCollectStat(bool collectStat = true) { m_calculator->setCollectStat(collectStat); }

    void setSimulationStep(double step) { m_simulationStep = step; }

    void setDelay(double delay) { m_simulationDelay = delay; }

    bool isStarted() const { return m_timer->isActive(); }

    int getSimulationTime() const { return m_simulationTime; }

    int getTimerTick() const { return m_simulationStep; }

    QString getPlanFilePath() const { return m_lastPlanFilePath; }

    Calculator& getCalculator() const { return *m_calculator; }

    Scene& getScene() const { return *m_scene; }

    void update(bool isTimeRun = true);

    void draw(QPainter& painter);

    void pause();

    void resume();

    void scrollEvent(QWheelEvent * event);

    void mouseClickEvent(QMouseEvent*);

    void mouseReleaseEvent(QMouseEvent*);

signals:
    void tick();
    void enableStatButton();
    void startSimulation(int n);
    void sendStatReportSignal(QString report);
    void changeScaleSignal(double scale);
    void updateAgentInRoomSignal(int num);

public slots:
    void loadPlan(QString filename);

    void finishSimulation();

    void clear();

    void reset();

    void startSimulationSlot();

private:
    int m_simulationStep = 100; // milliseconds
    int m_simulationDelay = 100; // milliseconds
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

    void readSchemeFromFile(QString filename);
};

#endif // ENGINE_H
