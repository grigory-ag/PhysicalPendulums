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

private slots:
    // Слоты для кнопок
    void on_actionStart_triggered();
    void on_actionPause_triggered();
    void on_actionReset_triggered();
    void updateAnimation();


    void on_ButtonOKMass_clicked();
    void on_ButtonResetMass_clicked();
    void on_ButtonOKPosition_clicked();
    void on_ButtonResetPosition_clicked();
    void on_ButtonOKElasticity_clicked();
    void on_ButtonResetElasticity_clicked();
    void on_ButtonOKAirFriction_clicked();
    void on_ButtonOffAirFriction_clicked();

private:
    Ui::SpringPendulum *ui;
    QMenuBar *menuBar;
    QTimer *timer;

    // Параметры маятника
    double mass = 1.0;
    double springConstant = 10.0;
    double initialPosition = 50.0;
    double position = 0.0;
    double velocity = 0.0;
    double acceleration = 0.0;
    double equilibriumPosition = 0.0;
    double maxMechanicalEnergy = 0.0;
    bool animationRunning = false;
    bool airFrictionEnabled = false;

    // Физические константы
    const double gravity = 9.81;
    const int bobRadius = 20;
    const double airFrictionCoeff = 0.1;
    const int supportHeight = 40;
    const double springLength = 200.0;

    // Вспомогательные методы
    void setupMenu();
    void drawSpring(QPainter &painter, int x1, int y1, int y2);
    void setInputsEnabled(bool enabled);
    void updateOutputValues();
    void calculateEquilibrium();

    // Методы расчетов
    double calculatePeriod();
    double calculateKineticEnergy();
    double calculatePotentialEnergy();
    double calculateMechanicalEnergy();
    double calculateVelocity();
    double calculateAmplitude();
    double calculateDisplacement();
};

#endif
