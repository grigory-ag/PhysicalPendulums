#include "MathPendulum.h"
#include "ui_MathPendulum.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QMessageBox>

// Конструктор класса MathPendulum
MathPendulum::MathPendulum(QWidget *parent) :
    QWidget(parent), ui(new Ui::MathPendulum)
{
    ui->setupUi(this);
    this->showFullScreen();

    // Настройка меню
    menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu("Functions");
    QAction *startAction = new QAction("Start", this);
    QAction *pauseAction = new QAction("Pause", this);
    QAction *resetAction = new QAction("Reset", this);
    fileMenu->addAction(startAction);
    fileMenu->addAction(pauseAction);
    fileMenu->addAction(resetAction);

    // Подключение слотов к действиям меню
    connect(startAction, &QAction::triggered, this, &MathPendulum::on_actionStart_triggered);
    connect(pauseAction, &QAction::triggered, this, &MathPendulum::on_actionPause_triggered);
    connect(resetAction, &QAction::triggered, this, &MathPendulum::on_actionReset_triggered);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMenuBar(menuBar);

    // Подключение кнопок интерфейса
    connect(ui->ButtonOKLength, &QPushButton::clicked, this, &MathPendulum::on_ButtonOKLength_clicked);
    connect(ui->ButtonResetLength, &QPushButton::clicked, this, &MathPendulum::on_ButtonResetLength_clicked);
    connect(ui->ButtonOKAngle, &QPushButton::clicked, this, &MathPendulum::on_ButtonOKAngle_clicked);
    connect(ui->ButtonResetAngle, &QPushButton::clicked, this, &MathPendulum::on_ButtonResetAngle_clicked);
    connect(ui->ButtonOKMass, &QPushButton::clicked, this, &MathPendulum::on_ButtonOKMass_clicked);
    connect(ui->ButtonResetMass, &QPushButton::clicked, this, &MathPendulum::on_ButtonResetMass_clicked);
    connect(ui->ButtonOKAirFriction, &QPushButton::clicked, this, &MathPendulum::on_ButtonOKAirFriction_clicked);
    connect(ui->ButtonOffAirFriction, &QPushButton::clicked, this, &MathPendulum::on_ButtonOffAirFriction_clicked);

    // Настройка таймера для анимации
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MathPendulum::updateAnimation);

    // Начальные настройки интерфейса
    setInputsEnabled(true);
    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");
}

// Деструктор класса
MathPendulum::~MathPendulum() {
    delete ui;
}

// Включение/отключение элементов ввода
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
    ui->ButtonOKAirFriction->setEnabled(enabled);
    ui->ButtonOffAirFriction->setEnabled(enabled);
}

// Отрисовка маятника
void MathPendulum::paintEvent(QPaintEvent *pEvent) {
    QWidget::paintEvent(pEvent);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::blue);
    painter.setBrush(QBrush(Qt::white));

    int width = this->width();
    int height = this->height();
    int pivotX = width / 2;
    int pivotY = height / 6 - supportHeight;

    const int pendulumLength = length * 11;
    const int bobRadius = 20;

    double angleRad = angle * DEG_TO_RAD;
    int bobX = pivotX + pendulumLength * sin(angleRad);
    int bobY = pivotY + pendulumLength * cos(angleRad);

    painter.drawLine(pivotX - 10, pivotY, pivotX + 10, pivotY);
    painter.drawLine(pivotX, pivotY, bobX, bobY);
    painter.drawEllipse(bobX - bobRadius, bobY - bobRadius, bobRadius * 2, bobRadius * 2);
}

// Обновление отрисовки маятника
void MathPendulum::updatePendulum() {
    this->update();
}

// Запуск анимации маятника
void MathPendulum::startAnimation() {
    setInputsEnabled(false);

    // Проверка диапазона длины для колебаний
    if (lengthForCalculations < 0.05 || lengthForCalculations > 100.0) {
        angularVelocity = 0.0; // Остановка движения
        initialAngle = fabs(angle);
        initialPeriod = calculatePeriod();
        totalMechanicalEnergy = calculateCurrentPotentialEnergy();

        ui->OutputPeriodValue->setText(QString::number(initialPeriod, 'f', 5));
        updateOutputValues();
        updatePendulum();

        QMessageBox::information(this, "Information",
        "Pendulum length is outside the oscillation range [ 0.05, 100 ]. Oscillations are disabled.");
        return;
    }

    // Инициализация параметров перед запуском
    initialAngle = fabs(angle);
    initialPeriod = calculatePeriod();
    totalMechanicalEnergy = calculateCurrentPotentialEnergy();
    ui->OutputPeriodValue->setText(QString::number(initialPeriod, 'f', 5));
    updateOutputValues();
    timer->start(16);
}

// Обновление анимации (вызывается таймером)
void MathPendulum::updateAnimation() {
    double alpha = -gravity / lengthForCalculations * sin(angle * DEG_TO_RAD) * RAD_TO_DEG;

    if (airFrictionEnabled) {
        alpha -= airFrictionCoeff * angularVelocity;
    }

    angularVelocity += alpha * 0.016;
    angle += angularVelocity * 0.016;

    // Ограничение угла отклонения
    if (angle > 90) angle = 90;
    if (angle < -90) angle = -90;

    updatePendulum();
    updateOutputValues();
}

// Расчет высоты подъема груза
double MathPendulum::calculateHeight() {
    return lengthForCalculations * (1 - cos(angle * DEG_TO_RAD));
}

// Расчет кинетической энергии
double MathPendulum::calculateCurrentKineticEnergy() {
    return 0.5 * mass * pow((angularVelocity * DEG_TO_RAD) * lengthForCalculations, 2);
}

// Расчет потенциальной энергии
double MathPendulum::calculateCurrentPotentialEnergy() {
    return mass * gravity * calculateHeight();
}

// Расчет периода колебаний
double MathPendulum::calculatePeriod() {
    const double smallAngleThreshold = 20.0; // Порог в градусах

    if (fabs(angle) <= smallAngleThreshold) {
        return 2 * M_PI * sqrt(lengthForCalculations / gravity);
    } else {
        double theta0 = fabs(angle); // В градусах
        double theta0Rad = theta0 * DEG_TO_RAD;
        double theta0Squared = theta0Rad * theta0Rad;

        return 2 * M_PI * sqrt(lengthForCalculations / gravity) *
               (1 + theta0Squared/16.0 + 11.0/3072.0 * theta0Squared * theta0Squared);
    }
}

// Расчет скорости груза
double MathPendulum::calculateVelocity() {
    return sqrt(2 * calculateCurrentKineticEnergy()/mass);
}

// Расчет амплитуды колебаний
double MathPendulum::calculateAmplitude() {
    return lengthForCalculations * sin(fabs(initialAngle) * DEG_TO_RAD);
}

// Обновление значений на интерфейсе
void MathPendulum::updateOutputValues() {
    double currentPotentialEnergy = calculateCurrentPotentialEnergy();
    double currentKineticEnergy = calculateCurrentKineticEnergy();

    ui->OutputGrEnValue->setText(QString::number(currentPotentialEnergy, 'f', 5));
    ui->OutputKinEnValue->setText(QString::number(currentKineticEnergy, 'f', 5));
    ui->OutputMechEnVlaue->setText(QString::number(totalMechanicalEnergy, 'f', 5));
    ui->OutputVelosityValue->setText(QString::number(calculateVelocity(), 'f', 5));
    ui->OutputAmplitudeVlaue->setText(QString::number(calculateAmplitude(), 'f', 5));
    ui->OutputHighValue->setText(QString::number(calculateHeight(), 'f', 5));
}

// Обработчики событий кнопок

void MathPendulum::on_ButtonOKLength_clicked() {
    QString Datal = ui->lengthInpEdit->toPlainText();
    bool ok;
    double newLength = Datal.toDouble(&ok);

    if (!ok || newLength <= 0) {
        QMessageBox::warning(this, "Error", "Length should be positive value!");
        return;
    }

    if (newLength > 1000000 || newLength < 0.00001) {
        QMessageBox::warning(this, "Error", "Value of the length should be in range: [ 0.00001, 1000000 ]!");
        return;
    }

    if (newLength > 50) {
        lengthForCalculations = newLength;
        length = 50;
    } else {
        if (newLength < 3) {
            lengthForCalculations = newLength;
            length = 3;
        } else {
            lengthForCalculations = newLength;
            length = newLength;
        }
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
        QMessageBox::warning(this, "Error", "Angle should be in range [ -90, 90 ]!");
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

void MathPendulum::on_ButtonOKAirFriction_clicked() {
    airFrictionEnabled = true;
    ui->ButtonOKAirFriction->setStyleSheet("background-color: green");
    ui->ButtonOffAirFriction->setStyleSheet("");
}

void MathPendulum::on_ButtonOffAirFriction_clicked() {
    airFrictionEnabled = false;
    ui->ButtonOKAirFriction->setStyleSheet("");
    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");
}

// Обработчики событий меню

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
    // Сброс всех параметров к начальным значениям
    length = DEFAULT_Y_OFFSET;
    lengthForCalculations = 10.0;
    angle = 0;
    mass = 1.0;
    angularVelocity = 0;
    airFrictionEnabled = false;

    totalMechanicalEnergy = calculateCurrentPotentialEnergy();
    maxPotentialEnergy = totalMechanicalEnergy;
    maxKineticEnergy = 0.0;
    maxMechanicalEnergy = totalMechanicalEnergy;

    // Очистка полей вывода
    ui->OutputMechEnVlaue->clear();
    ui->OutputGrEnValue->clear();
    ui->OutputKinEnValue->clear();
    ui->OutputAmplitudeVlaue->clear();
    ui->OutputHighValue->clear();
    ui->OutputVelosityValue->clear();
    ui->OutputPeriodValue->clear();
    ui->lengthInpEdit->clear();
    ui->AngleInpEdit->clear();
    ui->MassInpEdit->clear();

    // Сброс стилей кнопок
    ui->ButtonOKAirFriction->setStyleSheet("");
    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");

    timer->stop();
    setInputsEnabled(true);
    updatePendulum();
}
