#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QVector2D>
#include <QColor>

class SceneObject
{
public:
    SceneObject();
    SceneObject(int id, QVector2D pos, QColor color);

    inline QVector2D getPos() const { return m_position; }

    inline int getID() const { return m_id; }

    void setPos(const QVector2D& pos) { m_position = pos; }

private:
    int m_id;
    QVector2D m_position;//up left corner
    QColor m_color;

};

#endif // SCENEOBJECT_H
