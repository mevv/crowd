#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paint_widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PaintWidget * paint_widget = new PaintWidget(this, 10);

    ui->gridLayout->addWidget(paint_widget);


}

MainWindow::~MainWindow()
{
    delete ui;
}
