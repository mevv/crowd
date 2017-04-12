#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <memory>
#include <chrono>

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QAction>
#include <QThread>

#include "scene.h"
#include "calculator.h"
#include "objects_pool.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0, int interval = 0);

    void paintEvent(QPaintEvent * event);

    void resizeEvent(QResizeEvent * event);
signals:

public slots:
    void update();

private:
    std::unique_ptr<QPainter> m_painter;
    std::unique_ptr<QTimer> m_timer;

    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<Calculator> m_calculator;

//    Renderer * m_renderer;
//    QThread * m_renderThread;


};

#endif // PAINTWIDGET_H
