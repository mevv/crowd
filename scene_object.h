#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QVector2D>

class SceneObject
{
public:
    SceneObject(QVector2D pos);

    inline QVector2D getPos() const { return m_position; }

    void setPos(const QVector2D& pos) { m_position = pos; }

private:
    QVector2D m_position;

};

#endif // SCENEOBJECT_H
