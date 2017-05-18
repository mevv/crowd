#ifndef ENTRY_H
#define ENTRY_H

#include "scene_object.h"

#include <QPair>

class Entry : public SceneObject
{
public:
    Entry(int id, QVector2D pos, QColor color, QVector2D end);


    void setPeriod(const QPair<double, double>& period) { m_period = period; }

    inline double getTimeFromLastGenerate() const { return m_timeFromLastGenerate; }

    void resetTimeFromLastGenerate() { m_timeFromLastGenerate = 0; }

    void setTimeFromLastGenerate(double time) { m_timeFromLastGenerate = time; }

    inline QPair<double, double> getPeriodRange() const { return m_period; }

    QPoint getCenter() const { return QPoint((getPos().x() + m_end.x()) / 2.0, (getPos().y() + m_end.y()) / 2.0); }

    QVector2D getEnd() const { return m_end; }

    QVector2D getBegin() const { return this->getPos(); }

private:
    QVector2D m_end;
    QPair<double, double> m_period;
    double m_timeFromLastGenerate = 0;

};

#endif // ENTRY_H
