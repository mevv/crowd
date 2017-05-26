#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "json_manager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_engine.reset(new Engine());

    m_paintWidget.reset(new PaintWidget(this, m_engine));

    ui->playButton->hide();
    ui->clearButton->hide();

    ui->doubleSpinBox->setValue(10);

    ui->gridLayout->addWidget(m_paintWidget.get());
    m_paintWidget->setMouseTracking(true);

    connect(m_engine.get(), &Engine::tick, m_paintWidget.get(), &PaintWidget::update);
    connect(m_engine.get(), &Engine::tick, this, &MainWindow::updateTimeSlot);
    connect(this, &MainWindow::openedSchemeFile, m_engine.get(), &Engine::loadPlan);
    connect(m_engine.get(), &Engine::enableStatButton, this, [this](){ ui->stat_groupBox->show();});
    connect(this, &MainWindow::clearSimulation, m_engine.get(), &Engine::reset);
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
    //qDebug() << m_paintWidget->mapFromGlobal(QCursor::pos());

    if (m_paintWidget->mapFromGlobal(QCursor::pos()).x() > 0 &&
        m_paintWidget->mapFromGlobal(QCursor::pos()).x() < m_paintWidget->size().width() &&
        m_paintWidget->mapFromGlobal(QCursor::pos()).y() > 0 &&
        m_paintWidget->mapFromGlobal(QCursor::pos()).y() < m_paintWidget->size().height())
    {
        m_engine->scrollEvent(event);
        //qDebug() << m_paintWidget->mapFromGlobal(QCursor::pos());
    }
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
    ui->statTextEdit->setText(ui->statTextEdit->toPlainText() + report);
}

void MainWindow::on_change_crowd_params_triggered()
{
    QJsonObject curParam = JsonManager::parseJsonFile(JsonManager::getConfPath());

    m_crowdParameters.reset(new CrowdParameters(curParam));
    m_crowdParameters->show();
}

void MainWindow::on_open_shcheme_menu_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Відкрити креслення"), "/home", tr("JSON Files (*.json)"));
    ui->clearButton->show();
    ui->playButton->show();
    ui->scaleDoubleSpinBox->setValue(1);
    emit openedSchemeFile(file_name);
}

void MainWindow::on_open_simulation_menu_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Відкрити файл симуляції"), "/home", tr("JSON Files (*.sim)"));
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
    ui->scaleDoubleSpinBox->setValue(1);
    emit clearSimulation();
}

void MainWindow::on_endButton_clicked()
{
    ui->playButton->setText("Продовжити");
    m_engine->finishSimulation();
}

void MainWindow::on_statCleanPushButton_clicked()
{
    ui->statTextEdit->clear();
}

void MainWindow::on_statToFilePushButton_clicked()
{
    qDebug() << QString(JsonManager::getStatPath()) + QDateTime::currentDateTime().toString(Qt::ISODate) + ".stat";
    QFile file(QString(JsonManager::getStatPath()) + QDateTime::currentDateTime().toString(Qt::ISODate) + ".stat");

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

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    qDebug() << arg1;
    m_engine->setTimerTick(arg1);
}

void MainWindow::on_pathfindingCheckBox_clicked(bool checked)
{
    if (checked)
        m_engine->getCalculator().setUsePathFinding();
    else
        m_engine->getCalculator().setUsePathFinding(false);
}

void MainWindow::on_showPathCheckBox_3_clicked(bool checked)
{
    if (checked)
        m_engine->getScene().setDrawPath();
    else
        m_engine->getScene().setDrawPath(false);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    qDebug() << position;
}
