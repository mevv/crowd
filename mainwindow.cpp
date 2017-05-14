#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paint_widget.h"
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_engine.reset(new Engine());

    PaintWidget * paint_widget = new PaintWidget(this, m_engine);

    ui->stat_groupBox->hide();

    ui->gridLayout->addWidget(paint_widget);

    connect(m_engine.get(), &Engine::tick, paint_widget, &PaintWidget::update);
    connect(m_engine.get(), &Engine::tick, this, &MainWindow::updateTime);
    connect(this, &MainWindow::openedSchemeFile, m_engine.get(), &Engine::loadPlan);
    //connect(this, &MainWindow::openedSaveFile, m_engine.get(), &Engine::setSaveFileName);
    connect(m_engine.get(), &Engine::enableStatButton, this, [this](){ ui->stat_groupBox->show();});

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    if (m_engine->isStarted())
    {
        ui->playButton->setText("Start");
        m_engine->pause();
    }
    else
    {
        ui->playButton->setText("Pause");
        m_engine->resume();
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    m_engine->scrollEvent(event);
}

void MainWindow::updateTime()
{
    ui->totalTimeLineEdit->setText(QString::number(m_engine->getSimulationTime() / 1000.0));
}

void MainWindow::on_change_crowd_params_triggered()
{
    // TODO: norm JSON
    QJsonObject tmp;
    CrowdParameters * a = new CrowdParameters(tmp);
    a->show();

    // TODO: fix memory leak - store pointer to form and catch Save button signal
}

void MainWindow::on_open_shcheme_menu_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Відкрити креслення"), "/home/peter", tr("JSON Files (*.json)"));
    emit openedSchemeFile(file_name);
}

void MainWindow::on_open_simulation_menu_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Відкрити файл симуляції"), "/home/peter", tr("JSON Files (*.sim)"));
    emit openedSaveFile(file_name);
}

void MainWindow::on_path_to_crowd_params_file_menu_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Відкрити файл характеристики натовпу"), "/home/peter", tr("JSON Files (*.json)"));
    emit openedCrowdParamsFile(file_name);
}

void MainWindow::on_path_to_simulations_menu_triggered()
{
    auto path = QFileDialog::getExistingDirectory( this, tr("Зберегти симуляції до..."), "/home/peter", QFileDialog::ShowDirsOnly);
    emit changedPathToSimulations(path);
}
