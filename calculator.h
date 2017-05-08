#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <memory>

#include <QDebug>

#include "objects_pool.h"

class Calculator
{
public:
    Calculator(QPoint sceneSize, std::shared_ptr<ObjectsPool> pool);

    std::vector<QVector2D> update(double delta);


private:
    QPoint m_sceneSize;//meters

    std::shared_ptr<ObjectsPool> m_pool;

    double time;

    void setDirection(Agent &agent);

    bool isInExit(Agent agent);

};

#endif // CALCULATOR_H
