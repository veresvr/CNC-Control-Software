#include "settingscom.h"
#include "ui_settingscom.h"
#include <QSettings>
#include <QMessageBox>

QSettings *settings_another = new QSettings("settings.conf",QSettings::IniFormat);

SettingsCOM::SettingsCOM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsCOM)
{
    ui->setupUi(this);

    // baud
    ui->COMBaudBox->addItem(QLatin1String("9600"));
    ui->COMBaudBox->addItem(QLatin1String("19200"));
    ui->COMBaudBox->addItem(QLatin1String("38400"));
    ui->COMBaudBox->addItem(QLatin1String("115200"));
    ui->COMBaudBox->addItem(QLatin1String("Custom"));

    // numb of data
     ui->COMDataBox->addItem(QLatin1String("5"));
     ui->COMDataBox->addItem(QLatin1String("6"));
     ui->COMDataBox->addItem(QLatin1String("7"));
     ui->COMDataBox->addItem(QLatin1String("8"));
     ui->COMDataBox->setCurrentIndex(3);

     // parity
     ui->COMParityBox->addItem(QLatin1String("None"));
     ui->COMParityBox->addItem(QLatin1String("Even"));
     ui->COMParityBox->addItem(QLatin1String("Odd"));
     ui->COMParityBox->addItem(QLatin1String("Mark"));
     ui->COMParityBox->addItem(QLatin1String("Space"));

     // fill stop bits
     ui->COMStopBox->addItem(QLatin1String("1"));
     #ifdef Q_OS_WIN
     ui->COMStopBox->addItem(QLatin1String("1.5"));
     #endif
     ui->COMStopBox->addItem(QLatin1String("2"));

      // fill flow control
     ui->COMFlowBox->addItem(QLatin1String("None"));
     ui->COMFlowBox->addItem(QLatin1String("RTS/CTS"));
     ui->COMFlowBox->addItem(QLatin1String("XON/XOFF"));

}

SettingsCOM::~SettingsCOM()
{
    delete ui;
}

void SettingsCOM::on_loadSettingsButton_clicked()           // дописать!!!!!
{
    //ui->lineEdit->setText( settings_another->value("COM/name","COM24").toString());

//    SHPINDLE
    ui->spinBoxNumbOfTurns->setValue(settings_another->value("SHPINDLE/speedMAX",19000).toInt());
    ui->spinBoxDelayAfterON->setValue(settings_another->value("SHPINDLE/delayON",3).toInt());
    ui->spinBoxDelayAfterOFF->setValue(settings_another->value("SHPINDLE/delayOFF",3).toInt());

//    SOUND
    ui->checkBoxSoundAtChangeTool->setChecked(settings_another->value("SOUND/programmEND", "TRUE").toBool());
    ui->checkBoxSoundAtEndCycle->setChecked(settings_another->value("SOUND/cycleEND", "TRUE").toBool());
    ui->checkBoxSoundAtEndProgramm->setChecked(settings_another->value("SOUND/programmEND", "TRUE").toBool());
    ui->checkBoxSoundRepeat->setChecked(settings_another->value("SOUND/repeat", "TRUE").toBool());
    ui->checkBoxSoundAtEndsSensors->setChecked(settings_another->value("SOUND/axisSensors", "TRUE").toBool());


}

void SettingsCOM::on_defaultSettingsButton_clicked()
{

}

void SettingsCOM::on_applyButton_clicked()  // send signal to MainForm
{
    QMessageBox::information(0, "Info", "Are u sure?");
    saveAndApplySettingsInForm();
}

void SettingsCOM::on_checkBox_clicked()
{
//    if (ui->checkBoxManualInput->isChecked())
//    {
//        ui->CodeLineEdit->setEnabled(true);
//        ui->AddCodeButton->setEnabled(true);
//    } else
//    {
//        ui->CodeLineEdit->setEnabled(false);
//        ui->AddCodeButton->setEnabled(false);
//    }
}
