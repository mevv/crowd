#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QWheelEvent>
#include <QFileDialog>
#include <QJsonObject>

#include "engine.h"
#include "crowd_parameters.h"
#include "paint_widget.h"

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
    void updateStatSlot(QString report);

private slots:
    void on_playButton_clicked();

    void on_change_crowd_params_triggered();

    void on_open_shcheme_menu_triggered();

    void on_open_simulation_menu_triggered();

    void on_path_to_crowd_params_file_menu_triggered();

    void on_path_to_simulations_menu_triggered();

    void on_clearButton_clicked();

    void on_endButton_clicked();

    void on_statCleanPushButton_clicked();

    void on_statToFilePushButton_clicked();

    void on_checkBox_2_clicked(bool checked);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pathfindingCheckBox_clicked(bool checked);

    void on_showPathCheckBox_3_clicked(bool checked);

    void on_horizontalSlider_sliderMoved(int position);

    void on_algorithmComboBox_currentIndexChanged(int index);


    void updateAgentsInRoomSlot(int num);

    void updateEnterAgentSlot();

    void on_actionQt_triggered();

    void on_action_triggered();

    void on_action_2_triggered();

private:
    Ui::MainWindow *ui;

    std::shared_ptr<Engine> m_engine;
    std::unique_ptr<CrowdParameters> m_crowdParameters;
    std::unique_ptr<PaintWidget> m_paintWidget;

    QString m_configPath;

signals:
    void openedSchemeFile(QString file_name);
    void openedSaveFile(QString file_name);
    void changedPathToSimulations(QString path);
    void clearSimulation();
    void startSimulation();
    void pathAlgorithmChangedSignal(int index);
    void changePanicLevelSignal(double panicLevel);

};

#endif // MAINWINDOW_H
