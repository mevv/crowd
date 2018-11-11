#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QVector2D>
#include <QColor>

class SceneObject
{
public:
    SceneObject();
    SceneObject(int id, QVector2D pos, QColor color);

    QVector2D getPos() const { return m_position; }
    int getID() const { return m_id; }
    QColor getColor() const { return m_color; }

    void setPos(const QVector2D& pos) { m_position = pos; }

private:
    int m_id;
    QVector2D m_position;//up left corner
    QColor m_color;

};

#endif // SCENEOBJECT_H
