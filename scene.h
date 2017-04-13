#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include <QPainter>

#include "objects_pool.h"

class Scene
{
public:
    Scene(std::shared_ptr<ObjectsPool> pool);

    void draw(QPainter& painter);

private:
    double m_scale = 1;//number of screen pixels in real meter


    std::shared_ptr<ObjectsPool> m_pool;
};

#endif // SCENE_H
