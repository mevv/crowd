#include "paint_widget.h"

#include <QDebug>

PaintWidget::PaintWidget(QWidget *parent, const std::shared_ptr<Engine>& engine) : QWidget(parent)
{
    m_engine = engine;

    //m_engine.reset(new Engine());

    //connect(m_engine.get(), &Engine::tick, this, &PaintWidget::update);
}


void PaintWidget::update()
{
    //qDebug() << m_timer->remainingTime();

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
//    QPainter painter;

//    painter.begin(this);
//        painter.setBrush(Qt::black);
//        painter.drawRect(0, 0, this->height(), this->width());
//    painter.end();
}
