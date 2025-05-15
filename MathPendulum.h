#ifndef MATHPENDULUM_H
#define MATHPENDULUM_H

#include <QWidget>
#include <QMenuBar>
#include <QPropertyAnimation>
#include <QTimer>
#include <QLabel>

class MainWindow;

namespace Ui {
class MathPendulum;
}

class MathPendulum : public QWidget {
    Q_OBJECT

public:
    explicit MathPendulum(QWidget *parent = nullptr);
    ~MathPendulum();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MathPendulum *ui;
    QMenuBar *menuBar;
    QTimer *timer;

    // Параметры маятника
    double initialAngle = 0.0;
    double initialPeriod = 0.0;
    double totalMechanicalEnergy = 0.0;
    double maxPotentialEnergy = 0.0;
    double maxKineticEnergy = 0.0;
    double maxMechanicalEnergy = 0.0;
    double DEFAULT_Y_OFFSET = 10.0;
    double length = 10.0;
    double lengthForCalculations = 10;
    double angle = 0.0;
    double mass = 1.0;
    double angularVelocity = 0.0;
    bool isAnimating = false;
    bool airFrictionEnabled = false;
    bool isPaused = false;

    // Физические константы
    const double gravity = 9.81;
    const double DEG_TO_RAD = M_PI / 180.0;
    const double RAD_TO_DEG = 180.0 / M_PI;
    const int supportHeight = 80;
    const double airFrictionCoeff = 0.02;
    const double MIN_MASS = pow(10,-6);
    const double MAX_MASS = pow(10,6);
    const double MIN_LENGTH = pow(10,-6);
    const double MAX_LENGTH = pow(10,6);

    // Методы расчетов
    double calculateHeight();
    double calculateCurrentKineticEnergy();
    double calculateCurrentPotentialEnergy();
    double calculatePeriod();
    double calculateVelocity();
    double calculateAmplitude();

    // Вспомогательные методы
    void updatePendulum();
    void setInputsEnabled(bool enabled);
    void startAnimation();
    void updateOutputValues();

private slots:
    // Слоты для кнопок
    void on_ButtonOKLength_clicked();
    void on_ButtonResetLength_clicked();
    void on_ButtonOKAngle_clicked();
    void on_ButtonResetAngle_clicked();
    void on_ButtonOKMass_clicked();
    void on_ButtonResetMass_clicked();
    void on_ButtonOKAirFriction_clicked();
    void on_ButtonOffAirFriction_clicked();

    // Слоты для меню
    void on_actionStart_triggered();
    void on_actionPause_triggered();
    void on_actionReset_triggered();
    void on_actionExit_triggered();

    void updateAnimation();
};

#endif
