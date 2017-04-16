#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include <QPainter>
#include <QPoint>
#include <QDebug>

#include "objects_pool.h"

class Scene
{
public:
    Scene(QPoint size,std::shared_ptr<ObjectsPool> pool);

    void draw(QPainter& painter);

    void setScale(double delta) { m_scale += delta; }

private:
    QPoint m_size;//meters
    double m_scale = 1;//number of screen pixels in real meter

    std::shared_ptr<ObjectsPool> m_pool;
};

#endif // SCENE_H
