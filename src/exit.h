#ifndef EXIT_H
#define EXIT_H

#include <QVector2D>

#include "scene_object.h"

class Exit : public SceneObject
{
public:
    Exit(int id, QVector2D pos, QColor color, QVector2D end);

    QPoint getCenter() const { return QPoint((getPos().x() + m_end.x()) / 2.0, (getPos().y() + m_end.y()) / 2.0); }

    QVector2D getEnd() const { return m_end; }

    QVector2D getBegin() const { return m_begin; }

private:
    QVector2D m_begin;
    QVector2D m_end;//exit represented as line, begining of the line is m_position from base class and this is the end
    //QVector2D m_frontSide;
};

#endif // EXIT_H
