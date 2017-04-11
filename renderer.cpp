#include "renderer.h"

//not used, alternative way of render

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
    std::shared_ptr<ObjectsPool> pool(new ObjectsPool());
    pool->addAgent(Agent(5, 1, QVector2D(50, 50), QVector2D(10, 15), QVector2D()));
    pool->addAgent(Agent(5, 1, QVector2D(50, 50), QVector2D(15, 20), QVector2D()));
    pool->addAgent(Agent(5, 1, QVector2D(50, 50), QVector2D(-10, -15), QVector2D()));
    pool->addAgent(Agent(5, 1, QVector2D(50, 50), QVector2D(-30, 20), QVector2D()));
    pool->addAgent(Agent(5, 1, QVector2D(50, 50), QVector2D(10, -15), QVector2D()));

    m_scene.reset(new Scene(pool));
    m_calculator.reset(new Calculator(pool));

    qDebug() << "Pool count:" << pool.use_count();
}

void Renderer::process()
{
    while (m_isStart)
    {
        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> delta = m_prev - now;
        m_prev = now;

        //qDebug() << "Prev:" << m_prev. << "Now:" << now << "Delta:" << delta;

        m_calculator->update(delta.count() / 1000.0);

        m_prev = std::chrono::system_clock::now();

        emit update();
    }
}
