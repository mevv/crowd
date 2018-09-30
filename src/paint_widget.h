#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <memory>
#include <chrono>

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QAction>

#include "engine.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0, const std::shared_ptr<Engine>& engine = nullptr);

    void paintEvent(QPaintEvent*);

public slots:
    void update();
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);

private:
    std::unique_ptr<QPainter> m_painter;
    std::shared_ptr<Engine> m_engine;
};

#endif // PAINTWIDGET_H
