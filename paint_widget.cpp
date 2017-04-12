#include "paint_widget.h"

#include <QDebug>


PaintWidget::PaintWidget(QWidget *parent, int interval) : QWidget(parent)
{
    //get out all this shit from here! just for demo
    m_timer.reset(new QTimer(this));

    std::shared_ptr<ObjectsPool> pool(new ObjectsPool());

    std::srand(std::time(0));

    for (size_t i = 0; i < 10000; i++)
        pool->addAgent(Agent(i, 5, 1, QVector2D(50, 50), QVector2D(std::rand() % 50 + 1, std::rand() % 50 + 1), QVector2D(), QColor()));

    pool->addObstacle(Obstacle(1, QVector2D(10, 10),QColor(), {QPoint(100, 0), QPoint(100, 100), QPoint(0, 100)}));

    m_scene.reset(new Scene(pool));
    m_calculator.reset(new Calculator(pool));

    qDebug() << "Pool count:" << pool.use_count();

    connect(m_timer.get(),  &QTimer::timeout, this, [this]{ this->update(); });
    m_timer->setInterval(interval);
    m_timer->start();
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
