#include "MathPendulum.h"
#include "ui_MathPendulum.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QMessageBox>

MathPendulum::MathPendulum(QWidget *parent) :
    QWidget(parent), ui(new Ui::MathPendulum)
{
    ui->setupUi(this);
    this->showFullScreen();

    menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu("Functions");
    QAction *startAction = new QAction("Start", this);
    QAction *pauseAction = new QAction("Pause", this);
    QAction *resetAction = new QAction("Reset", this);
    fileMenu->addAction(startAction);
    fileMenu->addAction(pauseAction);
    fileMenu->addAction(resetAction);

    connect(startAction, &QAction::triggered, this, &MathPendulum::on_actionStart_triggered);
    connect(pauseAction, &QAction::triggered, this, &MathPendulum::on_actionPause_triggered);
    connect(resetAction, &QAction::triggered, this, &MathPendulum::on_actionReset_triggered);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMenuBar(menuBar);

    connect(ui->ButtonOKLength, &QPushButton::clicked, this, &MathPendulum::on_ButtonOKLength_clicked);
    connect(ui->ButtonResetLength, &QPushButton::clicked, this, &MathPendulum::on_ButtonResetLength_clicked);
    connect(ui->ButtonOKAngle, &QPushButton::clicked, this, &MathPendulum::on_ButtonOKAngle_clicked);
    connect(ui->ButtonResetAngle, &QPushButton::clicked, this, &MathPendulum::on_ButtonResetAngle_clicked);
    connect(ui->ButtonOKMass, &QPushButton::clicked, this, &MathPendulum::on_ButtonOKMass_clicked);
    connect(ui->ButtonResetMass, &QPushButton::clicked, this, &MathPendulum::on_ButtonResetMass_clicked);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MathPendulum::updateAnimation);

    // Изначально отключаем поля ввода
    setInputsEnabled(true);  // Поля ввода доступны при старте
}

MathPendulum::~MathPendulum()
{
    delete ui;
}

void MathPendulum::setInputsEnabled(bool enabled) {
    ui->lengthInpEdit->setEnabled(enabled);
    ui->AngleInpEdit->setEnabled(enabled);
    ui->MassInpEdit->setEnabled(enabled);
    ui->ButtonOKLength->setEnabled(enabled);
    ui->ButtonResetLength->setEnabled(enabled);
    ui->ButtonOKAngle->setEnabled(enabled);
    ui->ButtonResetAngle->setEnabled(enabled);
    ui->ButtonOKMass->setEnabled(enabled);
    ui->ButtonResetMass->setEnabled(enabled);
}

void MathPendulum::paintEvent(QPaintEvent *pEvent)
{
    QWidget::paintEvent(pEvent);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::blue);
    painter.setBrush(QBrush(Qt::white));

    int width = this->width();
    int height = this->height();
    int pivotX = width / 2;
    int pivotY = height / 6;

    double angleInRadians = angle * M_PI / 180.0;
    int bobX = pivotX + length * 10 * sin(angleInRadians);
    int bobY = pivotY + length * 10 * cos(angleInRadians);

    painter.drawLine(pivotX - 10, pivotY - 80, pivotX + 10, pivotY - 80);
    painter.drawLine(pivotX, pivotY - 80, bobX, bobY);
    painter.drawEllipse(bobX - 20, bobY - 20, 40, 40);
}

void MathPendulum::updatePendulum() {
    this->update();
}

void MathPendulum::startAnimation() {
    setInputsEnabled(false);  // Отключаем поля ввода при старте анимации
    updateOutputValues();     // Обновляем значения в интерфейсе
    timer->start(16);         // Начинаем анимацию с интервалом 16 мс
}

void MathPendulum::updateAnimation() {
    double alpha = -gravity / lengthForCalculations * sin(angle * M_PI / 180.0);
    angularVelocity += alpha * 0.016;  // Обновляем угловую скорость
    angle += angularVelocity * 0.016 * 180 / M_PI;  // Обновляем угол

    // Ограничиваем угол от -90 до 90 градусов
    if (angle > 90) angle = 90;
    if (angle < -90) angle = -90;

    updatePendulum();  // Обновляем отображение маятника
    updateOutputValues();  // Обновляем значения в интерфейсе
}

double MathPendulum::calculateHeight() {
    return lengthForCalculations * (1 - cos(angle * M_PI / 180.0));
}

double MathPendulum::calculateCurrentKineticEnergy() {
    return 0.5 * mass * pow(angularVelocity * lengthForCalculations, 2);
}

double MathPendulum::calculateCurrentPotentialEnergy() {
    return mass * gravity * calculateHeight();
}

double MathPendulum::calculatePeriod() {
    return 2 * M_PI * sqrt(lengthForCalculations / gravity);
}

double MathPendulum::calculateVelocity() {
    return sqrt(2 * gravity * lengthForCalculations * (1 - cos(angle * M_PI / 180.0)));
}

double MathPendulum::calculateAmplitude() {
    return angle;
}

void MathPendulum::updateOutputValues() {
    double currentPotentialEnergy = calculateCurrentPotentialEnergy();
    double currentKineticEnergy = calculateCurrentKineticEnergy();

    ui->OutputGrEnValue->setText(QString::number(currentPotentialEnergy, 'f', 2));
    ui->OutputKinEnValue->setText(QString::number(currentKineticEnergy, 'f', 2));
    ui->OutputPeriodValue->setText(QString::number(calculatePeriod(), 'f', 2));
    ui->OutputVelosityValue->setText(QString::number(calculateVelocity(), 'f', 2));
    ui->OutputAmplitudeVlaue->setText(QString::number(calculateAmplitude(), 'f', 2));
    ui->OutputHighValue->setText(QString::number(calculateHeight(), 'f', 2));
}


void MathPendulum::on_ButtonOKLength_clicked() {
    QString Datal = ui->lengthInpEdit->toPlainText();
    bool ok;
    double newLength = Datal.toDouble(&ok);
    if (!ok || newLength <= 0) {
        QMessageBox::warning(this, "Error", "Length should be positive value!");
        return;
    }

    if (newLength > 50) {
        lengthForCalculations = newLength;  // Длина для расчетов
        length = 50;  // Визуально длина ограничена 50
    } else {
        lengthForCalculations = newLength;  // Длина для расчетов
        length = newLength;  // Визуально длина совпадает с вводом
    }

    updatePendulum();
}

void MathPendulum::on_ButtonResetLength_clicked() {
    length = DEFAULT_Y_OFFSET;
    ui->lengthInpEdit->clear();
    updatePendulum();
}

void MathPendulum::on_ButtonOKAngle_clicked() {
    QString DataAngle = ui->AngleInpEdit->toPlainText();
    bool ok;
    double newAngle = DataAngle.toDouble(&ok);
    if (!ok || newAngle < -90 || newAngle > 90) {
        QMessageBox::warning(this, "Error", "Angle should be in range [-90;90]!");
        return;
    }
    angle = newAngle;
    updatePendulum();
}

void MathPendulum::on_ButtonResetAngle_clicked() {
    angle = 0;
    ui->AngleInpEdit->clear();
    updatePendulum();
}

void MathPendulum::on_ButtonOKMass_clicked() {
    QString DataMass = ui->MassInpEdit->toPlainText();
    bool ok;
    double newMass = DataMass.toDouble(&ok);
    if (!ok || newMass < 0) {
        QMessageBox::warning(this, "Error", "Mass should be positive value!");
        return;
    }
    mass = newMass;
}

void MathPendulum::on_ButtonResetMass_clicked() {
    mass = 1.0;
    ui->MassInpEdit->clear();
}

void MathPendulum::on_actionStart_triggered() {
    startAnimation();
}

void MathPendulum::on_actionPause_triggered() {
    if (timer->isActive()) {
        timer->stop();
    } else {
        timer->start(16);
    }
}

void MathPendulum::on_actionReset_triggered() {
    // Сброс всех параметров и остановка анимации
    length = DEFAULT_Y_OFFSET;
    angle = 0;
    mass = 1.0;
    angularVelocity = 0;
    ui->OutputGrEnValue->clear();
    ui->OutputKinEnValue->clear();
    ui->OutputAmplitudeVlaue->clear();
    ui->OutputHighValue->clear();
    ui->OutputVelosityValue->clear();
    ui->OutputPeriodValue->clear();
    ui->lengthInpEdit->clear();
    ui->AngleInpEdit->clear();
    ui->MassInpEdit->clear();
    timer->stop();
    setInputsEnabled(true);  // Включаем поля ввода после сброса
    updatePendulum();
}
