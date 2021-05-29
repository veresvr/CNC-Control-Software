#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <QMainWindow>

namespace Ui {
class firmware;
}

class firmware : public QMainWindow
{
    Q_OBJECT

public:
    explicit firmware(QWidget *parent = 0);
    ~firmware();

private:
    Ui::firmware *ui;
};

#endif // FIRMWARE_H
