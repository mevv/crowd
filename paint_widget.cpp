#include "paint_widget.h"

#include <QDebug>


PaintWidget::PaintWidget(QWidget *parent, int interval) : QWidget(parent)
{
    //m_painter.reset(new QPainter(this));
    m_timer.reset(new QTimer(this));

    std::shared_ptr<ObjectsPool> pool(new ObjectsPool());

    std::srand(std::time(0));

    for (size_t i = 0; i < 10000; i++)
        pool->addAgent(Agent(5, 1, QVector2D(50, 50), QVector2D(std::rand() % 50 + 1, std::rand() % 50 + 1), QVector2D()));

    m_scene.reset(new Scene(pool));
    m_calculator.reset(new Calculator(pool));

    qDebug() << "Pool count:" << pool.use_count();

    connect(m_timer.get(),  &QTimer::timeout, this, [this]{ this->update(); });
    m_timer->setInterval(interval);
    m_timer->start();

//for outside render
//    m_renderer = new Renderer();
//    m_renderThread = new QThread();
//    m_renderer->moveToThread(m_renderThread);
//    connect(m_renderer, &Renderer::update, this, [this]{ this->update(); });

//    m_renderThread->start();

}


void PaintWidget::update()
{
    //qDebug() << m_timer->remainingTime();

    m_calculator->update(m_timer->remainingTime());
    this->repaint();
}

void PaintWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter;

    painter.begin(this);
        painter.setBrush(Qt::white);
        painter.drawRect(0, 0, this->height(), this->width());

        m_scene->draw(painter);

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
