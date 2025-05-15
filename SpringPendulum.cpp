#include "SpringPendulum.h"
#include "ui_SpringPendulum.h"
#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QVBoxLayout>

// Конструктор класса SpringPendulum
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
    setLayout(mainLayout);

    // Подключение кнопок интерфейса
    connect(ui->ButtonOKMass, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOKMass_clicked);
    connect(ui->ButtonResetMass, &QPushButton::clicked, this, &SpringPendulum::on_ButtonResetMass_clicked);
    connect(ui->ButtonOKPosition, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOKPosition_clicked);
    connect(ui->ButtonResetPosition, &QPushButton::clicked, this, &SpringPendulum::on_ButtonResetPosition_clicked);
    connect(ui->ButtonOKElasticity, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOKElasticity_clicked);
    connect(ui->ButtonResetElasticity, &QPushButton::clicked, this, &SpringPendulum::on_ButtonResetElasticity_clicked);
    connect(ui->ButtonOKAirFriction, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOKAirFriction_clicked);
    connect(ui->ButtonOffAirFriction, &QPushButton::clicked, this, &SpringPendulum::on_ButtonOffAirFriction_clicked);

    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");
    setInputsEnabled(true);

    isInitialState = true;
    maxStretch = DEFAULT_POSITION;
    position = DEFAULT_POSITION;
}

// Деструктор класса
SpringPendulum::~SpringPendulum()
{
    delete ui;
}

// Настройка меню
void SpringPendulum::setupMenu()
{
    menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu("Functions");

    QAction *startAction = new QAction("Start", this);
    QAction *pauseAction = new QAction("Pause", this);
    QAction *resetAction = new QAction("Reset", this);
    QAction *exitAction = new QAction("Exit", this);

    fileMenu->addAction(startAction);
    fileMenu->addAction(pauseAction);
    fileMenu->addAction(resetAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(startAction, &QAction::triggered, this, &SpringPendulum::on_actionStart_triggered);
    connect(pauseAction, &QAction::triggered, this, &SpringPendulum::on_actionPause_triggered);
    connect(resetAction, &QAction::triggered, this, &SpringPendulum::on_actionReset_triggered);
    connect(exitAction, &QAction::triggered, this, &SpringPendulum::on_actionExit_triggered);
}

// Отрисовка пружинного маятника
void SpringPendulum::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int pivotX = width / 2;
    int pivotY = height() / 8 - supportHeight;

    // Если колебания отключены, рисуем статичное состояние
    if (!oscillationsEnabled) {
        double currentSpringLength = equilibriumLength;
        int bobY = pivotY + currentSpringLength;

        // Отрисовка опоры
        painter.setPen(Qt::blue);
        painter.drawLine(pivotX - 20, pivotY, pivotX + 20, pivotY);

        // Отрисовка пружины
        drawSpring(painter, pivotX, pivotY, bobY - bobRadius);

        // Отрисовка груза
        painter.setBrush(QBrush(Qt::white));
        painter.drawEllipse(pivotX - bobRadius, bobY - bobRadius, bobRadius*2, bobRadius*2);
        return;
    }

    // Обычная отрисовка для активных колебаний
    double currentSpringLength;
    if (isInitialState) {
        currentSpringLength = compressedLength;
    } else {
        currentSpringLength = equilibriumLength + position;

        if (pivotY + currentSpringLength < pivotY + bobRadius) {
            currentSpringLength = pivotY + bobRadius - pivotY;
        }
    }

    int bobY = pivotY + currentSpringLength;

    // Отрисовка опоры
    painter.setPen(Qt::blue);
    painter.drawLine(pivotX - 20, pivotY, pivotX + 20, pivotY);

    // Отрисовка пружины
    drawSpring(painter, pivotX, pivotY, bobY - bobRadius);

    // Отрисовка груза
    painter.setBrush(QBrush(Qt::white));
    painter.drawEllipse(pivotX - bobRadius, bobY - bobRadius, bobRadius*2, bobRadius*2);
}

// Проверка допустимого диапазона колебаний
bool SpringPendulum::checkOscillationRange()
{
    double maxPossibleLength = equilibriumLength + maxStretch;
    double minPossibleLength = equilibriumLength - maxStretch;

    return !(maxPossibleLength > MAX_OSCILLATION_LENGTH ||
             minPossibleLength < MIN_OSCILLATION_LENGTH ||
             mass < MIN_MASS || mass > MAX_MASS ||
             springConstant < MIN_SPRING_CONST || springConstant > MAX_SPRING_CONST ||
             maxStretch < MIN_STRETCH || maxStretch > MAX_STRETCH);
}

// Отрисовка пружины
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

// Включение/отключение элементов ввода
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

// Расчет положения равновесия
void SpringPendulum::calculateEquilibrium()
{
    equilibriumLength = compressedLength + (mass * gravity) / springConstant;
}

// Расчет периода колебаний
double SpringPendulum::calculatePeriod() const
{
    return 2 * M_PI * sqrt(mass / springConstant);
}

// Расчет кинетической энергии
double SpringPendulum::calculateKineticEnergy() const
{
    return 0.5 * mass * velocity * velocity;
}

// Расчет потенциальной энергии
double SpringPendulum::calculatePotentialEnergy() const
{
    return 0.5 * springConstant * position * position;
}

// Расчет скорости груза
double SpringPendulum::calculateVelocity() const
{
    return fabs(velocity);
}

// Расчет амплитуды колебаний
double SpringPendulum::calculateAmplitude() const
{
    return maxStretch;
}

// Расчет смещения от положения равновесия
double SpringPendulum::calculateDisplacement() const
{
    return fabs(position);
}

// Обновление значений на интерфейсе
void SpringPendulum::updateOutputValues()
{
    double currentPotentialEnergy = calculatePotentialEnergy();
    double currentKineticEnergy = oscillationsEnabled ? calculateKineticEnergy() : 0.0;
    double currentVelocity = oscillationsEnabled ? calculateVelocity() : 0.0;
    double currentDisplacement = calculateDisplacement();

    ui->OutputDisplacementValue->setText(QString::number(currentDisplacement, 'f', 5));
    ui->OutputVelosityValue->setText(QString::number(currentVelocity, 'f', 5));
    ui->OutputGrEnValue->setText(QString::number(currentPotentialEnergy, 'f', 5));
    ui->OutputKinEnValue->setText(QString::number(currentKineticEnergy, 'f', 5));
    ui->OutputMechEnVlaue->setText(QString::number(totalMechanicalEnergy, 'f', 5));
    ui->OutputAmplitudeVlaue->setText(QString::number(calculateAmplitude(), 'f', 5));
}

// Запуск анимации
void SpringPendulum::startAnimation()
{
    if (timer->isActive() && !isPaused) {
        return;
    }

    if (maxStretch <= 0) {
        QMessageBox::warning(this, "Error", "Please set position first!");
        return;
    }

    if (isPaused) {
        timer->start(16);
        isAnimating = true;
        isPaused = false;
        return;
    }

    totalMechanicalEnergy = calculatePotentialEnergy();

    ui->OutputPeriodValue->setText(QString::number(calculatePeriod(), 'f', 5));
    ui->OutputMechEnVlaue->setText(QString::number(totalMechanicalEnergy, 'f', 5));
    updateOutputValues();

    double maxPossibleLength = equilibriumLength + maxStretch;
    double minPossibleLength = equilibriumLength - maxStretch;

    if (maxPossibleLength > MAX_OSCILLATION_LENGTH || minPossibleLength < MIN_OSCILLATION_LENGTH ||
        mass < MIN_MASS || mass > MAX_MASS ||
        springConstant < MIN_SPRING_CONST || springConstant > MAX_SPRING_CONST ||
        maxStretch < MIN_STRETCH || maxStretch > MAX_STRETCH) {

        oscillationsEnabled = false;
        QMessageBox::information(this, "Information",
                                 QString("Pendulum parameters are outside the safe oscillation range.\n"
                                         "Oscillations are disabled.\n\n"));

        timer->stop();
        isAnimating = false;

        ui->OutputPeriodValue->setText(QString::number(calculatePeriod(), 'f', 6));
        ui->OutputMechEnVlaue->setText(QString::number(totalMechanicalEnergy, 'f', 6));
        ui->OutputGrEnValue->setText(QString::number(calculatePotentialEnergy(), 'f', 6));
        ui->OutputKinEnValue->setText("0.000000");
        ui->OutputVelosityValue->setText("0.000000");
        ui->OutputDisplacementValue->setText(QString::number(calculateDisplacement(), 'f', 6));
        ui->OutputAmplitudeVlaue->setText(QString::number(calculateAmplitude(), 'f', 6));

        update();
        return;
    }

    oscillationsEnabled = true;
    setInputsEnabled(false);
    timer->start(16);
    isAnimating = true;
    isInitialState = false;
}

// Обновление анимации
void SpringPendulum::updateAnimation()
{
    if(!isAnimating || isInitialState || !oscillationsEnabled) {
        updateOutputValues();
        return;
    }

    double force = -springConstant * position;
    if(airFrictionEnabled) {
        force -= airFrictionCoeff * velocity;
    }

    double acceleration = force / mass;
    velocity += acceleration * 0.016;
    position += velocity * 0.016;

    int pivotY = height() / 8 - supportHeight;
    double currentLength = equilibriumLength + position;
    if (pivotY + currentLength < pivotY + bobRadius) {
        position = (pivotY + bobRadius - pivotY) - equilibriumLength;
        velocity = 0;
    }

    updateOutputValues();
    update();
}

// Слоты меню

// Обработчик кнопки Start
void SpringPendulum::on_actionStart_triggered()
{
    startAnimation();
}

// Обработчик кнопки Pause
void SpringPendulum::on_actionPause_triggered()
{
    if(timer->isActive()) {
        timer->stop();
        isAnimating = false;
        isPaused = true;
    }
    else if (isPaused) {
        timer->start(16);
        isAnimating = true;
        isPaused = false;
    }
}

// Обработчик кнопки Reset
void SpringPendulum::on_actionReset_triggered()
{
    timer->stop();
    isAnimating = false;
    isPaused = false;
    isInitialState = true;
    oscillationsEnabled = true;

    mass = DEFAULT_MASS;
    springConstant = DEFAULT_ELASTICITY;
    maxStretch = DEFAULT_POSITION;
    position = DEFAULT_POSITION;
    velocity = 0.0;
    airFrictionEnabled = false;
    totalMechanicalEnergy = 0.0;
    maxPotentialEnergy = 0.0;
    maxKineticEnergy = 0.0;

    ui->MassInpEdit->clear();
    ui->PositionInpEdit->clear();
    ui->ElasticityInpEdit->clear();

    ui->OutputPeriodValue->clear();
    ui->OutputAmplitudeVlaue->clear();
    ui->OutputMechEnVlaue->clear();
    ui->OutputGrEnValue->clear();
    ui->OutputKinEnValue->clear();
    ui->OutputVelosityValue->clear();
    ui->OutputDisplacementValue->clear();

    ui->ButtonOKAirFriction->setStyleSheet("");
    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");

    setInputsEnabled(true);
    calculateEquilibrium();
    update();
}

// Обработчик кнопки Exit
void SpringPendulum::on_actionExit_triggered()
{
    if (timer->isActive()) {
        timer->stop();
    }

    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

    this->close();
}

// Обработчик кнопки OK для массы
void SpringPendulum::on_ButtonOKMass_clicked()
{
    QString data = ui->MassInpEdit->toPlainText();
    bool ok;
    double newMass = data.toDouble(&ok);

    if(!ok || data.isEmpty()) {
        newMass = DEFAULT_MASS;
    }
    else if(newMass <= MIN_MASS || newMass > MAX_MASS) {
        QMessageBox::warning(this, "Warning",
                             QString("Mass should be in range [%1, %2] kg!").arg(MIN_MASS).arg(MAX_MASS));
        return;
    }

    mass = newMass;
    calculateEquilibrium();
    update();
}

// Обработчик кнопки Reset для массы
void SpringPendulum::on_ButtonResetMass_clicked()
{
    mass = DEFAULT_MASS;
    ui->MassInpEdit->clear();
    calculateEquilibrium();
    checkOscillationRange();
    update();
}

// Обработчик кнопки OK для позиции
void SpringPendulum::on_ButtonOKPosition_clicked()
{
    QString data = ui->PositionInpEdit->toPlainText();
    bool ok;
    double newPos = data.toDouble(&ok);

    if(!ok || data.isEmpty()) {
        newPos = DEFAULT_POSITION;
    }
    else {
        if (newPos < MIN_STRETCH || newPos > MAX_STRETCH) {
            QMessageBox::warning(this, "Warning",
                                 QString("Stretch should be in range [%1, %2] m!").arg(MIN_STRETCH).arg(MAX_STRETCH));
            return;
        }
    }

    maxStretch = newPos;
    isInitialState = false;
    position = maxStretch;
    update();
}

// Обработчик кнопки Reset для позиции
void SpringPendulum::on_ButtonResetPosition_clicked()
{
    maxStretch = DEFAULT_POSITION;
    isInitialState = true;
    position = DEFAULT_POSITION;
    ui->PositionInpEdit->clear();
    checkOscillationRange();
    update();
}

// Обработчик кнопки OK для упругости
void SpringPendulum::on_ButtonOKElasticity_clicked()
{
    QString data = ui->ElasticityInpEdit->toPlainText();
    bool ok;
    double newK = data.toDouble(&ok);

    if(!ok || data.isEmpty()) {
        newK = DEFAULT_ELASTICITY;
    }
    else if(newK < MIN_SPRING_CONST || newK > MAX_SPRING_CONST) {
        QMessageBox::warning(this, "Warning",
                             QString("Spring constant should be in range [%1, %2] N/m!").arg(MIN_SPRING_CONST).arg(MAX_SPRING_CONST));
        return;
    }

    springConstant = newK;
    calculateEquilibrium();
    update();
}

// Обработчик кнопки Reset для упругости
void SpringPendulum::on_ButtonResetElasticity_clicked()
{
    springConstant = DEFAULT_ELASTICITY;
    ui->ElasticityInpEdit->clear();
    calculateEquilibrium();
    checkOscillationRange();
    update();
}

// Обработчик кнопки включения сопротивления воздуха
void SpringPendulum::on_ButtonOKAirFriction_clicked()
{
    airFrictionEnabled = true;
    ui->ButtonOKAirFriction->setStyleSheet("background-color: green");
    ui->ButtonOffAirFriction->setStyleSheet("");
}

// Обработчик кнопки выключения сопротивления воздуха
void SpringPendulum::on_ButtonOffAirFriction_clicked()
{
    airFrictionEnabled = false;
    ui->ButtonOKAirFriction->setStyleSheet("");
    ui->ButtonOffAirFriction->setStyleSheet("background-color: red");
}
