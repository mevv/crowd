#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "json_manager.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_engine.reset(new Engine());

    m_paintWidget.reset(new PaintWidget(this, m_engine));

    ui->playButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->endButton->setEnabled(false);

    ui->doubleSpinBox->setValue(100);

    ui->gridLayout->addWidget(m_paintWidget.get());

    m_paintWidget->setMouseTracking(true);

    connect(m_engine.get(), &Engine::tick, m_paintWidget.get(), &PaintWidget::update);
    connect(m_engine.get(), &Engine::tick, this, &MainWindow::updateTimeSlot);
    connect(this, &MainWindow::openedSchemeFile, m_engine.get(), &Engine::loadPlan);
    connect(m_engine.get(), &Engine::enableStatButton, this, [this](){ ui->stat_groupBox->show();});
    connect(this, &MainWindow::clearSimulation, m_engine.get(), &Engine::reset);
    connect(this, &MainWindow::clearSimulation, &(m_engine.get()->getCalculator()), &Calculator::buildPathfinfingMatrixSlot);
    connect(this, &MainWindow::startSimulation, m_engine.get(), &Engine::startSimulationSlot);

    connect(m_engine.get(), &Engine::changeScaleSignal, this, &MainWindow::updateScale);
    connect(m_engine.get(), &Engine::sendStatReportSignal, this, &MainWindow::updateStatSlot);
    connect(this, &MainWindow::pathAlgorithmChangedSignal,&(m_engine.get()->getCalculator()), &Calculator::pathAlgorithmChangedSlot);
    connect(this, &MainWindow::changePanicLevelSignal, &(m_engine->getCalculator()), &Calculator::changePanicLevelSlot);
    connect(m_engine.get(), &Engine::updateAgentInRoomSignal, this, &MainWindow::updateAgentsInRoomSlot);
    connect(&(m_engine->getCalculator()), &Calculator::enterAgentSignal, this, &MainWindow::updateEnterAgentSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    if(ui->playButton->text() == tr("Start"))
        emit startSimulation();

    if (m_engine->isStarted())
    {
        ui->playButton->setText(tr("Stop"));
        m_engine->pause();
    }
    else
    {
        ui->playButton->setText(tr("Pause"));
        m_engine->resume();
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (m_paintWidget->mapFromGlobal(QCursor::pos()).x() > 0 &&
        m_paintWidget->mapFromGlobal(QCursor::pos()).x() < m_paintWidget->size().width() &&
        m_paintWidget->mapFromGlobal(QCursor::pos()).y() > 0 &&
        m_paintWidget->mapFromGlobal(QCursor::pos()).y() < m_paintWidget->size().height())
    {
        m_engine->scrollEvent(event);
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
    m_crowdParameters.reset(new CrowdParameters(JsonManager::parseJsonFile(JsonManager::getConfPath())));
    m_crowdParameters->show();
}

void MainWindow::on_open_shcheme_menu_triggered()
{
    m_schemePath = QFileDialog::getOpenFileName(this, tr("Open scheme"), m_schemePath, tr("JSON Files (*.json)"));

    ui->clearButton->setEnabled(true);
    ui->playButton->setEnabled(true);
    ui->endButton->setEnabled(true);

    ui->scaleDoubleSpinBox->setValue(1);
    ui->enteredLineEdit->setText(QString::number(0));

    emit openedSchemeFile(m_schemePath);
}

void MainWindow::on_path_to_crowd_params_file_menu_triggered()
{
    JsonManager::setConfPath(QFileDialog::getOpenFileName(this, tr("Open config"), JsonManager::getConfPath(), tr("JSON Files (*.json)")));
}

void MainWindow::on_clearButton_clicked()
{
    ui->playButton->setText(tr("Start"));

    ui->scaleDoubleSpinBox->setValue(1);
    ui->enteredLineEdit->setText(QString::number(0));

    emit clearSimulation();
}

void MainWindow::on_endButton_clicked()
{
    ui->playButton->setText(tr("Continue"));
    m_engine->finishSimulation();
}

void MainWindow::on_statCleanPushButton_clicked()
{
    ui->statTextEdit->clear();
}

void MainWindow::on_statToFilePushButton_clicked()
{
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
    ui->statTextEdit->setEnabled(checked);
    ui->statCleanPushButton->setEnabled(checked);
    ui->statToFilePushButton->setEnabled(checked);

    m_engine->setCollectStat(checked);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    m_engine->setSimulationStep(arg1);
}

void MainWindow::on_pathfindingCheckBox_clicked(bool checked)
{
    if (checked)
    {
        ui->algorithmComboBox->setEnabled(true);

        m_engine->getCalculator().setUsePathFinding();
    }
    else
    {
        ui->algorithmComboBox->setEnabled(false);

        m_engine->getCalculator().setUsePathFinding(false);
    }
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
    emit changePanicLevelSignal((position + 1) / 100.0);
}

void MainWindow::on_algorithmComboBox_currentIndexChanged(int index)
{
    emit pathAlgorithmChangedSignal(index);
}

void MainWindow::updateAgentsInRoomSlot(int num)
{
    ui->inRoomLineEdit->setText(QString::number(num));
}

void MainWindow::updateEnterAgentSlot()
{
    ui->enteredLineEdit->setText(QString::number(ui->enteredLineEdit->text().toInt() + 1));
}

void MainWindow::on_actionQt_triggered()
{
    QMessageBox::about(this, tr("About Qt"), QString(tr("Qt v. ")) + QString(QT_VERSION_STR));
}

void MainWindow::on_action_triggered()
{
    QMessageBox::about(this, tr("About"), tr("Crowd Modeling Tool"));
}

void MainWindow::on_action_2_triggered()
{
    QMessageBox::about(this, tr("Authors"), tr("Mezeria Valeriy Vasilevich\nShcherbatyuk Petr Igorevich"));
}
