#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MathPendulum.h"
#include "SpringPendulum.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mathpen(nullptr)
    , springpen(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Pendulum Simulator");
}

MainWindow::~MainWindow()
{
    delete ui;

    if (mathpen) {
        delete mathpen;
    }

    if (springpen) {
        delete springpen;
    }
}

void MainWindow::on_MathButtoon_clicked()
{
    if (!mathpen) {
        mathpen = new MathPendulum();
    }
    mathpen->show();
    this->hide();
}

void MainWindow::on_SprPenButton_clicked()
{
    if (!springpen) {
        springpen = new SpringPendulum();
    }
    springpen->show();
    this->hide();
}
