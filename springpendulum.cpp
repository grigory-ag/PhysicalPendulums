#include "springpendulum.h"
#include "ui_springpendulum.h"

SpringPendulum::SpringPendulum(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpringPendulum)
{
    ui->setupUi(this);
}

SpringPendulum::~SpringPendulum()
{
    delete ui;
}
