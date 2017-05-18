#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paint_widget.h"
#include "json_manager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_engine.reset(new Engine());

    PaintWidget * paint_widget = new PaintWidget(this, m_engine);

    ui->playButton->hide();
    ui->clearButton->hide();

    ui->gridLayout->addWidget(paint_widget);

    connect(m_engine.get(), &Engine::tick, paint_widget, &PaintWidget::update);
    connect(m_engine.get(), &Engine::tick, this, &MainWindow::updateTimeSlot);
    connect(this, &MainWindow::openedSchemeFile, m_engine.get(), &Engine::loadPlan);
    connect(m_engine.get(), &Engine::enableStatButton, this, [this](){ ui->stat_groupBox->show();});
    connect(this, &MainWindow::clearSimulation, m_engine.get(), &Engine::clear);
    connect(this, &MainWindow::startSimulation, m_engine.get(), &Engine::startSimulationSlot);

    connect(m_engine.get(), &Engine::changeScaleSignal, this, &MainWindow::updateScale);
    connect(m_engine.get(), &Engine::sendStatReportSignal, this, &MainWindow::updateStatSlot);

    //connect(this, &MainWindow::openedSaveFile, m_engine.get(), &Engine::setSaveFileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    if(ui->playButton->text() == "Почати")
        emit startSimulation();

    if (m_engine->isStarted())
    {
        ui->playButton->setText("Продовжити");
        m_engine->pause();
    }
    else
    {
        ui->playButton->setText("Зупинити");
        m_engine->resume();
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    m_engine->scrollEvent(event);
}

void MainWindow::updateTimeSlot()
{
    ui->totalTimeLineEdit->setText(QString::number(m_engine->getSimulationTime() / 1000.0));
}

void MainWindow::updateScale(double scale)
{
    ui->scaleDoubleSpinBox->setValue(ui->scaleDoubleSpinBox->value() * scale);
}

void MainWindow::updateStatSlot(QString report)
{
    ui->statTextEdit->setText(report);
}

void MainWindow::on_change_crowd_params_triggered()
{
    QJsonObject curParam = JsonManager::parseJsonFile(PATH_TO_CONF);

    m_crowdParameters.reset(new CrowdParameters(curParam));
    m_crowdParameters->show();
}

void MainWindow::on_open_shcheme_menu_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Відкрити креслення"), "/home/peter", tr("JSON Files (*.json)"));
    ui->clearButton->show();
    ui->playButton->show();
    emit openedSchemeFile(file_name);
}

void MainWindow::on_open_simulation_menu_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Відкрити файл симуляції"), "/home/peter", tr("JSON Files (*.sim)"));
    ui->clearButton->show();
    ui->playButton->show();
    emit openedSaveFile(file_name);
}

void MainWindow::on_path_to_crowd_params_file_menu_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Відкрити файл характеристики натовпу"), "/home", tr("JSON Files (*.json)"));
    emit openedCrowdParamsFile(file_name);
}

void MainWindow::on_path_to_simulations_menu_triggered()
{
    auto path = QFileDialog::getExistingDirectory( this, tr("Зберегти симуляції до..."), "/home", QFileDialog::ShowDirsOnly);
    emit changedPathToSimulations(path);
}

void MainWindow::on_clearButton_clicked()
{
    ui->playButton->setText("Почати");
    emit clearSimulation();
}

void MainWindow::on_endButton_clicked()
{
    m_engine->finishSimulation();
}

void MainWindow::on_statCleanPushButton_clicked()
{
    ui->statTextEdit->clear();
}

void MainWindow::on_statToFilePushButton_clicked()
{
    QFile file(QString(PATH_TO_STAT) + QDateTime::currentDateTime().toString(Qt::ISODate) + ".stat");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "MainWindow::on_statToFilePushButton_clicked() - can't open stat file";
    else
    {
        QTextStream out(&file);
        out << ui->statTextEdit->toPlainText();
    }
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    m_engine->setCollectStat(checked);
}
