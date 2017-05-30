#include "paint_widget.h"

#include <QDebug>

PaintWidget::PaintWidget(QWidget *parent, const std::shared_ptr<Engine>& engine) : QWidget(parent)
{
    m_engine = engine;

    //m_engine.reset(new Engine());

    //connect(m_engine.get(), &Engine::tick, this, &PaintWidget::update);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


void PaintWidget::update()
{  
    this->repaint();
}

void PaintWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter;

    painter.begin(this);
        painter.setBrush(Qt::white);
        painter.drawRect(0, 0, this->height(), this->width());

        m_engine->draw(painter);

    painter.end();
}


void PaintWidget::resizeEvent(QResizeEvent * event)
{
    //this->size(event->size());
}

void PaintWidget::mousePressEvent (QMouseEvent * event)
{
    m_engine->mouseClickEvent(event);
}

void PaintWidget::mouseReleaseEvent (QMouseEvent * event)
{
    m_engine->mouseReleaseEvent(event);
}
