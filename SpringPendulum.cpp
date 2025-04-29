#include "SpringPendulum.h"
#include "ui_SpringPendulum.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QPainterPath>

SpringPendulum::SpringPendulum(QWidget *parent) :
    QWidget(parent), ui(new Ui::SpringPendulum)
{
    ui->setupUi(this);
    this->showFullScreen();

    setupMenu();
    calculateEquilibrium();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SpringPendulum::updateAnimation);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setMenuBar(menuBar);
    this->setLayout(mainLayout);

    connect(ui->ButtonOKMass, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOKMass_clicked);
    connect(ui->ButtonResetMass, &QPushButton::clicked, this, &SpringPendulum::on_ButtonResetMass_clicked);
    connect(ui->ButtonOKPosition, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOKPosition_clicked);
    connect(ui->ButtonResetPosition, &QPushButton::clicked, this, &SpringPendulum::on_ButtonResetPosition_clicked);
    connect(ui->ButtonOKElasticity, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOKElasticity_clicked);
    connect(ui->ButtonResetElasticity, &QPushButton::clicked, this, &SpringPendulum::on_ButtonResetElasticity_clicked);
    connect(ui->ButtonOKAirFriction, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOKAirFriction_clicked);
    connect(ui->ButtonOffAirFriction, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOffAirFriction_clicked);

    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");
}

SpringPendulum::~SpringPendulum()
{
    delete ui;
}

void SpringPendulum::setupMenu()
{

    menuBar = new QMenuBar(nullptr);


    QMenu *functionsMenu = menuBar->addMenu(tr("Functions"));


    QAction *startAction = functionsMenu->addAction(tr("Start"));
    QAction *pauseAction = functionsMenu->addAction(tr("Pause"));
    QAction *resetAction = functionsMenu->addAction(tr("Reset"));


    startAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    pauseAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
    resetAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));


    connect(startAction, &QAction::triggered, this, &SpringPendulum::on_actionStart_triggered);
    connect(pauseAction, &QAction::triggered, this, &SpringPendulum::on_actionPause_triggered);
    connect(resetAction, &QAction::triggered, this, &SpringPendulum::on_actionReset_triggered);


    QMenu *settingsMenu = menuBar->addMenu(tr("Settings"));
    QAction *prefsAction = settingsMenu->addAction(tr("Preferences"));
    connect(prefsAction, &QAction::triggered, this, &SpringPendulum::showPreferences);
}

void SpringPendulum::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int pivotX = width / 2;
    int pivotY = height() / 8 - supportHeight;


    int bobY = pivotY + springLength + (mass * gravity / springConstant) + position;


    painter.setPen(Qt::blue);
    painter.drawLine(pivotX - 20, pivotY, pivotX + 20, pivotY);


    drawSpring(painter, pivotX, pivotY, bobY - bobRadius);


    painter.setBrush(QBrush(Qt::white));
    painter.drawEllipse(pivotX - bobRadius, bobY - bobRadius, bobRadius*2, bobRadius*2);
}

void SpringPendulum::drawSpring(QPainter &painter, int x1, int y1, int y2)
{
    painter.setPen(Qt::blue);
    const int numCoils = 10;
    const int amplitude = 10;

    int length = y2 - y1;
    double coilHeight = static_cast<double>(length) / numCoils;

    QPainterPath path;
    path.moveTo(x1, y1);

    for(int i = 0; i < numCoils; i++) {
        int yStart = y1 + i * coilHeight;
        int yEnd = y1 + (i + 1) * coilHeight;
        int direction = (i % 2 == 0) ? 1 : -1;

        path.cubicTo(x1 + amplitude*direction, yStart + coilHeight/3,
                     x1 + amplitude*direction, yEnd - coilHeight/3,
                     x1, yEnd);
    }

    painter.drawPath(path);
}

void SpringPendulum::setInputsEnabled(bool enabled)
{
    ui->MassInpEdit->setEnabled(enabled);
    ui->PositionInpEdit->setEnabled(enabled);
    ui->ElasticityInpEdit->setEnabled(enabled);
    ui->ButtonOKMass->setEnabled(enabled);
    ui->ButtonResetMass->setEnabled(enabled);
    ui->ButtonOKPosition->setEnabled(enabled);
    ui->ButtonResetPosition->setEnabled(enabled);
    ui->ButtonOKElasticity->setEnabled(enabled);
    ui->ButtonResetElasticity->setEnabled(enabled);
    ui->ButtonOKAirFriction->setEnabled(enabled);
    ui->ButtonOffAirFriction->setEnabled(enabled);
}

void SpringPendulum::calculateEquilibrium()
{
    equilibriumPosition = springLength + (mass * gravity) / springConstant;
}

double SpringPendulum::calculatePeriod()
{
    return 2 * M_PI * sqrt(mass / springConstant);
}

double SpringPendulum::calculateKineticEnergy()
{
    return 0.5 * mass * velocity * velocity;
}

double SpringPendulum::calculatePotentialEnergy()
{
    return 0.5 * springConstant * position * position;
}

double SpringPendulum::calculateMechanicalEnergy()
{
    return calculateKineticEnergy() + calculatePotentialEnergy();
}

double SpringPendulum::calculateVelocity()
{
    return velocity;
}

double SpringPendulum::calculateAmplitude()
{
    return fabs(position);
}

double SpringPendulum::calculateDisplacement()
{
    return position;
}

void SpringPendulum::updateOutputValues()
{
    ui->OutputDisplacementValue->setText(QString::number(calculateDisplacement(), 'f', 3));
    ui->OutputVelosityValue->setText(QString::number(calculateVelocity(), 'f', 3));
    ui->OutputKinEnValue->setText(QString::number(calculateKineticEnergy(), 'f', 3));
    ui->OutputGrEnValue->setText(QString::number(calculatePotentialEnergy(), 'f', 3));
    ui->OutputMechEnVlaue->setText(QString::number(maxMechanicalEnergy, 'f', 3));
}


void SpringPendulum::on_ButtonOKMass_clicked()
{
    QString data = ui->MassInpEdit->toPlainText();
    bool ok;
    double newMass = data.toDouble(&ok);

    if(!ok || newMass <= 0) {
        QMessageBox::warning(this, "Error", "Invalid mass value!");
        return;
    }
    mass = newMass;
    calculateEquilibrium();
    update();
}

void SpringPendulum::on_ButtonResetMass_clicked()
{
    mass = 1.0;
    ui->MassInpEdit->clear();
    calculateEquilibrium();
    update();
}

void SpringPendulum::on_ButtonOKPosition_clicked()
{
    QString data = ui->PositionInpEdit->toPlainText();
    bool ok;
    double newPos = data.toDouble(&ok);

    const double MIN_POS = 10.0;
    const double MAX_POS = 500.0;

    if(!ok || newPos < MIN_POS || newPos > MAX_POS) {
        QMessageBox::warning(this, "Error",
        QString("Position must be between %1 and %2!").arg(MIN_POS).arg(MAX_POS));
        return;
    }
    initialPosition = newPos;
    position = initialPosition - equilibriumPosition;
    update();
}

void SpringPendulum::on_ButtonResetPosition_clicked()
{
    initialPosition = 50.0;
    ui->PositionInpEdit->clear();
    position = initialPosition - equilibriumPosition;
    update();
}

void SpringPendulum::on_ButtonOKElasticity_clicked()
{
    QString data = ui->ElasticityInpEdit->toPlainText();
    bool ok;
    double newK = data.toDouble(&ok);

    const double MIN_K = 0.1;
    const double MAX_K = 1000.0;

    if(!ok || newK < MIN_K || newK > MAX_K) {
        QMessageBox::warning(this, "Error",
        QString("Spring constant should be in range [%1, %2]!").arg(MIN_K).arg(MAX_K));
        return;
    }
    springConstant = newK;
    calculateEquilibrium();
    update();
}

void SpringPendulum::on_ButtonResetElasticity_clicked()
{
    springConstant = 10.0;
    ui->ElasticityInpEdit->clear();
    calculateEquilibrium();
    update();
}

void SpringPendulum::on_ButtonOKAirFriction_clicked()
{
    airFrictionEnabled = true;
    ui->ButtonOKAirFriction->setStyleSheet("background-color: green");
    ui->ButtonOffAirFriction->setStyleSheet("");
}

void SpringPendulum::on_ButtonOffAirFriction_clicked()
{
    airFrictionEnabled = false;
    ui->ButtonOKAirFriction->setStyleSheet("");
    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");
}


void SpringPendulum::on_actionStart_triggered()
{
    setInputsEnabled(false);
    calculateEquilibrium();
    position = initialPosition - equilibriumPosition;
    velocity = 0.0;
    maxMechanicalEnergy = calculateMechanicalEnergy();

    ui->OutputPeriodValue->setText(QString::number(calculatePeriod(), 'f', 3));
    ui->OutputAmplitudeVlaue->setText(QString::number(calculateAmplitude(), 'f', 3));

    timer->start(16);
    animationRunning = true;
}

void SpringPendulum::on_actionPause_triggered()
{
    if(animationRunning) {
        timer->stop();
        animationRunning = false;
    } else {
        timer->start(16);
        animationRunning = true;
    }
}

void SpringPendulum::on_actionReset_triggered()
{
    timer->stop();
    animationRunning = false;

    mass = 1.0;
    springConstant = 10.0;
    initialPosition = 50.0;
    calculateEquilibrium();
    position = initialPosition - equilibriumPosition;
    velocity = 0.0;
    airFrictionEnabled = false;
    maxMechanicalEnergy = 0.0;

    ui->MassInpEdit->clear();
    ui->PositionInpEdit->clear();
    ui->ElasticityInpEdit->clear();
    ui->OutputPeriodValue->clear();
    ui->OutputAmplitudeVlaue->clear();
    ui->OutputMechEnVlaue->clear();

    ui->ButtonOKAirFriction->setStyleSheet("");
    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");

    setInputsEnabled(true);
    update();
}

void SpringPendulum::updateAnimation()
{
    if(!animationRunning) return;

    double force = -springConstant * position;
    if(airFrictionEnabled) {
        force -= airFrictionCoeff * velocity;
    }

    acceleration = force / mass;
    velocity += acceleration * 0.016;
    position += velocity * 0.016;

    updateOutputValues();
    update();
}
