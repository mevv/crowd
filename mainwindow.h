#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QWheelEvent>

#include "engine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void wheelEvent(QWheelEvent *event);

public slots:
    void updateTime();

private slots:
    void on_playButton_clicked();


private:
    Ui::MainWindow *ui;

    std::shared_ptr<Engine> m_engine;

};

#endif // MAINWINDOW_H
