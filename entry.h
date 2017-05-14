#ifndef ENTRY_H
#define ENTRY_H

#include "scene_object.h"

#include <QPair>

class Entry : public SceneObject
{
public:
    Entry(int id, QVector2D pos, QColor color, QVector2D end);


    void setFreq(const QPair<double, double>& freq) { m_freq = freq; }


    QPoint getCenter() const { return QPoint((getPos().x() + m_end.x()) / 2.0, (getPos().y() + m_end.y()) / 2.0); }

    QVector2D getEnd() const { return m_end; }

    QVector2D getBegin() const { return this->getPos(); }

private:
    QVector2D m_end;
    QPair<double, double> m_freq;

};

#endif // ENTRY_H
