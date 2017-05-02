#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paint_widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_engine.reset(new Engine());

    PaintWidget * paint_widget = new PaintWidget(this, m_engine);

    ui->gridLayout->addWidget(paint_widget);

    connect(m_engine.get(), &Engine::tick, paint_widget, &PaintWidget::update);
    connect(m_engine.get(), &Engine::tick, this, &MainWindow::updateTime);
    connect(this, &MainWindow::openedSchemeFile, m_engine.get(), &Engine::setSchemeFileName);
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

void MainWindow::on_action_2_triggered()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Open draft"), "/home/peter", tr("JSON Files (*.json)"));
    emit openedSchemeFile(file_name);
}
