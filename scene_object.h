#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QVector2D>

class SceneObject
{
public:
    SceneObject(int id, QVector2D pos);

    inline QVector2D getPos() const { return m_position; }

    inline int getID() const { return m_id; }

    void setPos(const QVector2D& pos) { m_position = pos; }

private:
    int m_id;
    QVector2D m_position;

};

#endif // SCENEOBJECT_H
