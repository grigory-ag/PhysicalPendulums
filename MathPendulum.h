#ifndef MATHPENDULUM_H
#define MATHPENDULUM_H

#include <QWidget>
#include <QMenuBar>
#include <QPropertyAnimation>
#include <QTimer>
#include <QLabel>

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
    QPropertyAnimation *animation;
    QTimer *timer;

    // Параметры маятника
    double DEFAULT_Y_OFFSET = 10.0;
    double length = 10.0;  // Длина для отображения
    double lengthForCalculations = 10.0;  // Длина для расчетов (если больше 50, используется она)
    double angle = 0.0;
    double mass = 1.0;
    double angularVelocity = 0.0;
    bool isAnimating = false;

    const double gravity = 9.81;  // Ускорение свободного падения (м/с^2)

    // Методы для расчета значений
    double calculateHeight();
    double calculateCurrentKineticEnergy();
    double calculateCurrentPotentialEnergy();
    double calculatePeriod();
    double calculateVelocity();
    double calculateAmplitude();

    // Метод для обновления отображения маятника
    void updatePendulum();

    // Метод для включения/отключения полей ввода
    void setInputsEnabled(bool enabled);

    // Метод для старта анимации
    void startAnimation();

    // Метод для обновления значений в интерфейсе
    void updateOutputValues();

private slots:
    void on_ButtonOKLength_clicked();
    void on_ButtonResetLength_clicked();
    void on_ButtonOKAngle_clicked();
    void on_ButtonResetAngle_clicked();
    void on_ButtonOKMass_clicked();
    void on_ButtonResetMass_clicked();
    void on_actionStart_triggered();
    void on_actionPause_triggered();
    void on_actionReset_triggered();
    void updateAnimation();
};

#endif // MATHPENDULUM_H
