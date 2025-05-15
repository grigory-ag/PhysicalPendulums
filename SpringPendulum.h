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

    // Physical parameters
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

    // Energy values
    double totalMechanicalEnergy = 0.0;
    double maxPotentialEnergy = 0.0;
    double maxKineticEnergy = 0.0;

    // Constants
    const double gravity = 9.81;
    const int bobRadius = 20;
    const double airFrictionCoeff = 0.1;
    const int supportHeight = 40;
    const double compressedLength = 100.0;

    // Safe limits for proper animation
    const double MIN_MASS = pow(10,-6);
    const double MAX_MASS = pow(10,6);
    const double MIN_SPRING_CONST = pow(10,-6);
    const double MAX_SPRING_CONST = pow(10,6);
    const double MIN_STRETCH = pow(10,-6);
    const double MAX_STRETCH = pow(10,6);
    const double MIN_OSCILLATION_LENGTH = 50.0;
    const double MAX_OSCILLATION_LENGTH = 500.0;

    // Default values
    const double DEFAULT_MASS = 1.0;
    const double DEFAULT_ELASTICITY = 10.0;
    const double DEFAULT_POSITION = 0.0;

    // Helper methods
    void setupMenu();
    void drawSpring(QPainter &painter, int x1, int y1, int y2);
    void setInputsEnabled(bool enabled);
    void calculateEquilibrium();
    void updateOutputValues();
    void startAnimation();
    bool checkOscillationRange();
    bool isPaused = false;

    // Calculation methods
    double calculatePeriod() const;
    double calculateKineticEnergy() const;
    double calculatePotentialEnergy() const;
    double calculateVelocity() const;
    double calculateAmplitude() const;
    double calculateDisplacement() const;

private slots:
    // Menu slots
    void on_actionStart_triggered();
    void on_actionPause_triggered();
    void on_actionReset_triggered();
    void on_actionExit_triggered();

    // Button slots
    void on_ButtonOKMass_clicked();
    void on_ButtonResetMass_clicked();
    void on_ButtonOKPosition_clicked();
    void on_ButtonResetPosition_clicked();
    void on_ButtonOKElasticity_clicked();
    void on_ButtonResetElasticity_clicked();
    void on_ButtonOKAirFriction_clicked();
    void on_ButtonOffAirFriction_clicked();

    // Animation
    void updateAnimation();
};

#endif
