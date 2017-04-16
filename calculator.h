#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <memory>

#include <QDebug>

#include "objects_pool.h"

class Calculator
{
public:
    Calculator(QPoint sceneSize, std::shared_ptr<ObjectsPool> pool);

    void update(double delta);

private:
    QPoint m_sceneSize;//meters

    std::shared_ptr<ObjectsPool> m_pool;

};

#endif // CALCULATOR_H
