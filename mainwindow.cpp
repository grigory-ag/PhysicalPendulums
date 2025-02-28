#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mathpen(nullptr) {
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
