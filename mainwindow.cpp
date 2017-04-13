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
