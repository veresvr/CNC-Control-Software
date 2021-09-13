#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "port.h"
#include <QtGui>
#include <QtCore>
#include <QLabel>
#include <QProgressBar>
#include <settingscom.h>
#include <firmware.h>
#include <QFileDialog>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

signals:

    void savesettings(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);
    void writeData(QByteArray data);
    void setVisibleSettingsCOM();


private slots:
    void on_Btn_Serch_clicked();
    void checkCustomBaudRatePolicy(int idx);
    void Print(QString data);
    void on_cEnterText_returnPressed();
    void on_BtnSave_clicked();
    void on_pushButton_clicked();
    void on_BtnConnect_clicked();
    void on_pushButton_2_clicked();
    void on_cBtnSend_clicked();

    void showSettingsWindow();
    void showMakeFirmwareWindow();
    void loadProgrammFile();
    void closeProgrammFile();
    void aboutWindow();
    uint8_t calculateCRC(uint8_t *pcBlock, uint32_t len);
    double coordinatesTransfer(unsigned long int inches);
    double toolDiamTransfer(double toolDiam);
    void actionExcellonWasClicked();
    void actionDXFWasClicked();
    void settingsChangedInForm();
    void toolTipChange();

    void on_RunButton_clicked();
    void on_ShpindleRightButton_clicked();
    void on_ShpindleStopButton_clicked();
    void on_ShpindleLeftButton_clicked();
    void on_checkBoxEnableLED_toggled(bool checked);
    void on_checkBoxEnableLED_clicked();
    void on_checkBoxEnableAirCooling_clicked();
    void on_toPositiveEndOnZ_clicked();
    void on_toNegativeEndOnZ_clicked();
    void on_CheckCodeButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_AddCodeButton_clicked();
    void on_ReloadCodeButton_clicked();

    void on_checkBoxManualInput_clicked();
    void on_toNegativeAxisX_clicked();
    void on_toNegativeAxisZ_clicked();
    void on_toPositiveAxisZ_clicked();

    void on_pushButton_5_clicked();

    void on_toZeroOnZ_clicked();

    void on_pushButton_7_clicked();

    void on_pushButtonGOTO_clicked();

    void on_toPositiveAxisY_clicked();

    void on_toPositiveAxisX_clicked();

private:
    Ui::MainWindow *ui;
    Port PortNew;

    QLabel *StatLabelCOM;
    QLabel *RunTimeLabel;
    QLabel *StatProgramm;

    QProgressBar *CheckFileProgress;

    SettingsCOM *form = new SettingsCOM;        // create form with all settings
    firmware *firmwareForm = new firmware;

};



#endif // MAINWINDOW_H
