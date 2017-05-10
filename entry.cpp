#include "entry.h"

Entry::Entry(int id, QVector2D pos, QColor color, QVector2D end) :
    SceneObject(id, pos, color),
    m_end(end)
{

}

