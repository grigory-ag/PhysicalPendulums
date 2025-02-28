#ifndef SPRINGPENDULUM_H
#define SPRINGPENDULUM_H

#include <QWidget>

namespace Ui {
class SpringPendulum;
}

class SpringPendulum : public QWidget
{
    Q_OBJECT

public:
    explicit SpringPendulum(QWidget *parent = nullptr);
    ~SpringPendulum();

private:
    Ui::SpringPendulum *ui;
};

#endif // SPRINGPENDULUM_H
