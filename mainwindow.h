#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MathPendulum.h"
#include "springpendulum.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_MathButtoon_clicked();

private:
    Ui::MainWindow *ui;
    MathPendulum *mathpen;
};
#endif // MAINWINDOW_H
