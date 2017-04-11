#include "calculator.h"

Calculator::Calculator(std::shared_ptr<ObjectsPool> pool) :
    m_pool(pool)
{

}

void Calculator::update(double delta)
{
    double time = delta / 1000;

    for (auto&& i : m_pool->getAgents())
    {
//        qDebug() << i.getSpeed();
//        qDebug() << i.getPos();

        if (i.getPos().x() > 500 || i.getPos().x() < 0)
            i.setSpeed(QVector2D(i.getSpeed().x() * -1, i.getSpeed().y()));

        if (i.getPos().y() > 500 || i.getPos().y() < 0)
            i.setSpeed(QVector2D(i.getSpeed().x(), i.getSpeed().y() * -1));

        i.setPos(QVector2D(i.getPos().x() + time * i.getSpeed().x(), i.getPos().y() + time * i.getSpeed().y()));
    }
}
