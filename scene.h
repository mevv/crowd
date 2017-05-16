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

    void setScale(double delta) { m_scale = delta; }

    inline double getScale() const { return m_scale; }

    void scale(double scale) { m_scale *= scale; }

    void setSize(const QPoint& size) { m_size = size; }

    void moveBy(QPoint delta) { m_pos += delta; }

    QPoint getSize() {return this->m_size; }

    //get out this methods from here, need another entity for it
    std::vector<QPoint> movePolygonTo(const std::vector<QPoint>& polygon, QPoint place);

    std::vector<QPoint> scalePolygon(const std::vector<QPoint>& polygon, double scale);

private:
    QPoint m_size;//meters
    QPoint m_pos;//position of building on widget in pixels

    double m_scale = 1;//number of screen pixels in real meter

    std::shared_ptr<ObjectsPool> m_pool;

    //arrow primitive
    std::vector<QPoint> m_arrow;
};

#endif // SCENE_H
