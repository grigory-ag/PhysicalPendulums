#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mathpen(nullptr), springpen(nullptr) {

    ui->setupUi(this);
}



MainWindow::~MainWindow() {
    delete ui;
    if (mathpen) {
        delete mathpen;
    }
}



void MainWindow::on_MathButtoon_clicked() {
    mathpen = new MathPendulum();
    mathpen->show();
    this->close();
}



void MainWindow::on_SprPenButton_clicked() {
    springpen = new SpringPendulum();
    springpen->show();
    this->close();
}
