#include "exit.h"

Exit::Exit(int id, QVector2D pos, QColor color, QVector2D end) :
    SceneObject(id, pos, color),
    m_end(end)
{

}

