#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <chrono>

#include <QObject>

#include "scene.h"
#include "calculator.h"
#include "objects_pool.h"

//not used, alternative way of render

class Renderer : public QObject
{
    Q_OBJECT

public:
    Renderer();
    ~Renderer();

public slots:
    void process();

    std::unique_ptr<Scene>& getScene() { return m_scene; }

signals:
    void update();
    void finished();
    void error(QString e);

private:
    bool m_isStart = false;

    std::chrono::time_point<std::chrono::system_clock> m_prev;

    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<Calculator> m_calculator;
};

#endif // RENDERER_H
