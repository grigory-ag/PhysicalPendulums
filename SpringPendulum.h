#ifndef SPRINGPENDULUM_H
#define SPRINGPENDULUM_H

#include <QWidget>
#include <QMenuBar>
#include <QTimer>
#include <QMessageBox>
#include <cmath>

namespace Ui {
class SpringPendulum;
}

class SpringPendulum : public QWidget
{
    Q_OBJECT

public:
    explicit SpringPendulum(QWidget *parent = nullptr);
    ~SpringPendulum();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::SpringPendulum *ui;
    QMenuBar *menuBar;
    QTimer *timer;

    // Параметры маятника
    double mass = 1.0;
    double springConstant = 10.0;
    double maxStretch = 0.0;
    double position = 0.0;
    double velocity = 0.0;
    double equilibriumLength = 200.0;
    bool isAnimating = false;
    bool airFrictionEnabled = false;
    bool isInitialState = true;
    bool oscillationsEnabled = true;

    // Значения энергии маятника
    double totalMechanicalEnergy = 0.0;
    double maxPotentialEnergy = 0.0;
    double maxKineticEnergy = 0.0;

    // Физические константы
    const double gravity = 9.81;
    const int bobRadius = 20;
    const double airFrictionCoeff = 0.1;
    const int supportHeight = 40;
    const double compressedLength = 100.0;

    // Значения для корректной визуализации движения
    const double MIN_MASS = pow(10,-6);
    const double MAX_MASS = pow(10,6);
    const double MIN_SPRING_CONST = pow(10,-6);
    const double MAX_SPRING_CONST = pow(10,6);
    const double MIN_STRETCH = pow(10,-6);
    const double MAX_STRETCH = pow(10,6);
    const double MIN_OSCILLATION_LENGTH = 50.0;
    const double MAX_OSCILLATION_LENGTH = 500.0;

    // Начальные значения системы
    const double DEFAULT_MASS = 1.0;
    const double DEFAULT_ELASTICITY = 10.0;
    const double DEFAULT_POSITION = 0.0;

    // Вспомогательные методы
    void setupMenu();
    void drawSpring(QPainter &painter, int x1, int y1, int y2);
    void setInputsEnabled(bool enabled);
    void calculateEquilibrium();
    void updateOutputValues();
    void startAnimation();
    bool checkOscillationRange();
    bool isPaused = false;

    // Методы расчетов
    double calculatePeriod() const;
    double calculateKineticEnergy() const;
    double calculatePotentialEnergy() const;
    double calculateVelocity() const;
    double calculateAmplitude() const;
    double calculateDisplacement() const;

private slots:
    // Слоты для меню
    void on_actionStart_triggered();
    void on_actionPause_triggered();
    void on_actionReset_triggered();
    void on_actionExit_triggered();

    // Слоты для кнопок
    void on_ButtonOKMass_clicked();
    void on_ButtonResetMass_clicked();
    void on_ButtonOKPosition_clicked();
    void on_ButtonResetPosition_clicked();
    void on_ButtonOKElasticity_clicked();
    void on_ButtonResetElasticity_clicked();
    void on_ButtonOKAirFriction_clicked();
    void on_ButtonOffAirFriction_clicked();

    // Анимация
    void updateAnimation();
};

#endif
