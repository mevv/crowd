#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QWheelEvent>
#include <QFileDialog>
#include <QJsonObject>

#include "engine.h"
#include "crowd_parameters.h"


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
    void updateTimeSlot();
    void updateScale(double scale);

private slots:
    void on_playButton_clicked();

    void on_change_crowd_params_triggered();

    void on_open_shcheme_menu_triggered();

    void on_open_simulation_menu_triggered();

    void on_path_to_crowd_params_file_menu_triggered();

    void on_path_to_simulations_menu_triggered();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;

    std::shared_ptr<Engine> m_engine;

    std::unique_ptr<CrowdParameters> m_crowdParameters;

signals:
    void openedSchemeFile(QString file_name);
    void openedSaveFile(QString file_name);
    void changedPathToSimulations(QString path);
    void openedCrowdParamsFile(QString file_name);
    void clearSimulation();
    void startSimulation();

};

#endif // MAINWINDOW_H
