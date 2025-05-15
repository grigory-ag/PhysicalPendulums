#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MathPendulum;
class SpringPendulum;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_MathButtoon_clicked();
    void on_SprPenButton_clicked();

private:
    Ui::MainWindow *ui;
    MathPendulum *mathpen;
    SpringPendulum *springpen;
};

#endif
