#include "firmware.h"
#include "ui_firmware.h"

firmware::firmware(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::firmware)
{
    ui->setupUi(this);
}

firmware::~firmware()
{
    delete ui;
}
