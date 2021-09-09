#include "mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QDesktopWidget>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <unistd.h>
#include <errno.h>
#include "ui_mainwindow.h"
#include <QThread>
#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <qdebug.h>
#include <QtDebug>
#include <QMenu>
#include <veres_defines_list.h>
//#include <QSettings>

// file types
#define NO_FILE     0
#define EXCELLON_F  1
#define DXF_F       2
#define OUT_ARR_SIZE    15


// global variables
QString versionOfProgramm = "0.0x pre-a";     // curennt version at this time
uint32_t shpindleSpeedMAX = 1000;
uint8_t codeFileType = NO_FILE;       // 255 different types of files can load

QFile programmCNC;          // here we only create variable! and initialize it in MainWindow::loadProgrammFile()
QTextStream codeData;       // here we only create variable ! and init. it in MainWindow::on_CheckCodeButton_clicked()
QString str;
QString defaultDirectory = QCoreApplication::applicationDirPath().toLocal8Bit();

QSettings *settings = new QSettings("settings.conf",QSettings::IniFormat);

// variables of settings
QString COM_name = "COM2",
        COM_parity = "none",
        COM_baud = "9600",
        COM_data = "8",
        COM_stop = "1",
        COM_control = "none";

QString SHPINDLE_speedMAX = "19000",
        SHPINDLE_delayON = "3",
        SHPINDLE_delayOFF = "3";

QString LASER_power = "123",
        LASER_dot = "12";

QString SOUND_programmEND = "TRUE",
        SOUND_cycleEND = "TRUE",
        SOUND_changeTool = "TRUE",
        SOUND_axisSensors = "TRUE",
        SOUND_repeat = "TRUE";

//settings->setValue("AXIS/mode", 2);
//settings->setValue("AXIS/lenght", 2);
//settings->setValue("AXIS/steps", 2);
//settings->setValue("AXIS/fastMove", 2);
//settings->setValue("AXIS/slowMove", 2);



//___________________________________________________________
// main code
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- general settings ---


    ui->SliderToSetShpindleSpeed->setMaximum(shpindleSpeedMAX);

    codeFileType = NO_FILE;
    // turm off manual input when programm starting
    ui->checkBoxManualInput->setChecked(false);
    ui->CodeLineEdit->setEnabled(false);
    ui->AddCodeButton->setEnabled(false);
    ui->radioButtonToSteps->setChecked(true);


    // ------------------------

    connect(ui->BaudRateBox, SIGNAL(currentIndexChanged(int)) ,this, SLOT(checkCustomBaudRatePolicy(int)));
    ui->BaudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->BaudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->BaudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->BaudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->BaudRateBox->addItem(QLatin1String("Custom"));

   // fill data bits
    ui->DataBitsBox->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->DataBitsBox->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->DataBitsBox->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->DataBitsBox->addItem(QLatin1String("8"), QSerialPort::Data8);
    ui->DataBitsBox->setCurrentIndex(3);

   // fill parity
    ui->ParityBox->addItem(QLatin1String("None"), QSerialPort::NoParity);
    ui->ParityBox->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    ui->ParityBox->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    ui->ParityBox->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    ui->ParityBox->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);

   // fill stop bits
    ui->StopBitsBox->addItem(QLatin1String("1"), QSerialPort::OneStop);
    #ifdef Q_OS_WIN
    ui->StopBitsBox->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
    #endif
    ui->StopBitsBox->addItem(QLatin1String("2"), QSerialPort::TwoStop);

   // fill flow control
    ui->FlowControlBox->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    ui->FlowControlBox->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    ui->FlowControlBox->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
    connect(ui->cBtnSend,SIGNAL(clicked()),this, SLOT(on_cEnterText_returnPressed()) );



    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    QThread *thread_New = new QThread;//Создаем поток для порта платы
    Port *PortNew = new Port();//Создаем обьект по классу

    PortNew->SettingsPort.name = "COM4";
    PortNew->SettingsPort.baudRate = QSerialPort::Baud9600;
    PortNew->SettingsPort.dataBits = QSerialPort::Data8;
    PortNew->SettingsPort.parity = QSerialPort::NoParity;
    PortNew->SettingsPort.stopBits = QSerialPort::OneStop;
    PortNew->SettingsPort.flowControl = QSerialPort::NoFlowControl;


    PortNew->moveToThread(thread_New);      //помешаем класс  в поток
    PortNew->thisPort.moveToThread(thread_New);     //Помещаем сам порт в поток
    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(Print(QString)));      //Лог ошибок
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));      //Переназначения метода run
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));        //Переназначение метода выход
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));      //Удалить к чертям поток
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));     //Удалить к чертям поток
    connect(this,SIGNAL(savesettings(QString,int,int,int,int,int)),PortNew,SLOT(Write_Settings_Port(QString,int,int,int,int,int)));//Слот - ввод настроек!
    connect(ui->BtnConnect, SIGNAL(clicked()),PortNew,SLOT(ConnectPort()));
    connect(ui->BtnDisconect, SIGNAL(clicked()),PortNew,SLOT(DisconnectPort()));
    connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(Print(QString)));     //Лог ошибок
    connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));
    connect(ui->menuBar, SIGNAL(actionOpen_programm()), this, SLOT(openProgramm()));        // open .txt file to load a code
    connect(ui->menuSettings, SIGNAL(triggered(bool)), this, SLOT(showSettingsWindow()));     // show window with settings
    connect(ui->actionMake_firmware, SIGNAL(triggered(bool)), this, SLOT(showMakeFirmwareWindow()));

    connect(ui->actionExcellon, SIGNAL(triggered(bool)), this, SLOT(loadProgrammFile()));
    connect(ui->actionExcellon, SIGNAL(triggered(bool)), this, SLOT(actionExcellonWasClicked()));
    connect(ui->actionDXF, SIGNAL(triggered(bool)), this, SLOT(loadProgrammFile()));
    connect(ui->actionDXF, SIGNAL(triggered(bool)), this, SLOT(actionDXFWasClicked()));

    connect(ui->actionAutors, SIGNAL(triggered(bool)), this, SLOT(aboutWindow()));
    connect(ui->menuClose, SIGNAL(triggered(bool)), this, SLOT(closeProgrammFile()));
    connect(ui->actionToolTip, SIGNAL(triggered(bool)), this, SLOT(toolTipChange()));
    connect(form, SIGNAL(saveAndApplySettingsInForm()), this, SLOT(settingsChangedInForm()));



    thread_New->start();

    on_Btn_Serch_clicked();         // automatically search all ports when programm is started

    // add all images
    {
    qDebug(QCoreApplication::applicationDirPath().toLocal8Bit());
    // menu
    ui->menuOpen->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
    ui->menuClose->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/close.png"));
    ui->menuSettings->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/settings.png"));
    ui->actionExit->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/exit.png"));

    ui->actionAutors->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/autors.png"));
    ui->actionG_M_codes->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/G_M.png"));
    ui->actionList_errors->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/er.png"));

    ui->menuOpen->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
    ui->menuOpen->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
    ui->menuOpen->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
    ui->menuOpen->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
    // controllers
    ui->toPositiveAxisX->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/arrowu.png"));
    ui->toPositiveAxisY->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/arrowr.png"));
    ui->toPositiveAxisZ->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/arrowu.png"));
    ui->toNegativeAxisX->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/arrowd.png"));
    ui->toNegativeAxisY->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/arrowl.png"));
    ui->toNegativeAxisZ->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/arrowd.png"));

    ui->ShpindleLeftButton->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/to_left.png"));
    ui->ShpindleRightButton->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/to_right.png"));
    ui->ShpindleStopButton->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/stop.png"));
// need add new logo!
//    ui->actionExcellon->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
//    ui->actionGerber_RS247X->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
//    ui->actionDXF->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
//    ui->actionPNG->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));
//    ui->AddCodeButton->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/open.png"));

}


}
//++++++++[Процедура закрытия приложения]+++++++++++++++++++++++++++++++++++++++++++++
MainWindow::~MainWindow()
{

    delete ui; //Удаление формы
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::showSettingsWindow()
{
    form->show();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::showMakeFirmwareWindow()
{
    firmwareForm->show();
}

//++++++++[Процедура пределения подключенных портов]+++++++++++++++++++++++++++++++++++
void MainWindow::on_Btn_Serch_clicked()
{
    ui->PortNameBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
        ui->PortNameBox->addItem(info.portName());
        }
}

void MainWindow::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->BaudRateBox->itemData(idx).isValid();
    ui->BaudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->BaudRateBox->clearEditText();
    }
}

//+++++++++++++[Процедура ввода данных из строки]++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_cEnterText_returnPressed()
{
    QByteArray data; // Текстовая переменная
    data = ui->cEnterText->text().toLocal8Bit() + '\r'; // Присвоение "data" значения из EnterText
    writeData(data); // Отправка данных в порт
    Print(data); // Вывод данных в консоль
    qDebug("show sending data");
}

//++++++++++++++func for load work programm++++++++++++++++++++++++++++++
// маленькое обьяснение: на самом деле программа не работает ПОСТОЯННО с внешним файлом, она только копирует текст
// в текстБраузер и тут же закрывает файл. ну а последующая функция ЗАКРЫТЬ сделана чтоб можно было очистить поля от текста.
void MainWindow::loadProgrammFile()
{
    QString fileTypeStr, fileNameStr;

    if (QObject::sender()->objectName() == "actionExcellon")        // this "QObject::sender()->objectName()"
    {                                                               //  needs for know who send signal to these function
        fileNameStr = "Open excellon drill file";
        fileTypeStr = "Text files (*.txt)";
        codeFileType = EXCELLON_F;
//        qDebug("loadProgrammFile():  codeFileType = EXCELLON_F");
    }
    if (QObject::sender()->objectName() == "actionDXF")             // this "QObject::sender()->objectName()"
    {                                                               //  needs for know who send signal to these function
        fileNameStr = "Open 2D drawing file";
        fileTypeStr = "DXF files (*.dxf)";
        codeFileType = DXF_F;
//        qDebug("loadProgrammFile():  codeFileType =  DXF_F");
    }

    str = QFileDialog::getOpenFileName(this,                                    // search the file what u need
                                               tr(fileNameStr.toLocal8Bit()),   // header of the window
                                               defaultDirectory,                // directory, contains these programm
                                               tr(fileTypeStr.toLocal8Bit()));  // type of searching file
    programmCNC.setFileName(str);
    if (!programmCNC.open(QIODevice::ReadOnly))                                 // if any error was occured - say it
    {
        if (programmCNC.error() == QFileDevice::OpenError)         // if a man close window of opening, without open any file - say it
            QMessageBox::information(0, "Info", "Файл не открыт!");
        else
            QMessageBox::information(0, "Info", programmCNC.errorString());     // in another case...

        codeFileType = NO_FILE;
        return;
    }

    ui->pathOfProgramm->setText(str);
    defaultDirectory = str;                             // change default dir for ease to use
    codeData.setDevice(&programmCNC);

    ui->ProgrammCNC->setText(codeData.readAll());

//    programmCNC.close();    // needs close after end of work! not in that function!
}

void MainWindow::closeProgrammFile()
{
    ui->pathOfProgramm->clear();
    ui->pathOfProgramm->setText("Path...");
    ui->ProgrammCNC->clear();
    ui->textBrowser->clear();
    codeFileType = NO_FILE;
}

//+++++++++++++[Процедура вывода данных в консоль]++++++++++++++++++++++++++++++++++++++++
void MainWindow::Print(QString data)
{
    ui->consol->textCursor().insertText(data); // Вывод текста в консоль
    ui->consol->moveCursor(QTextCursor::End);//Scroll

    ui->textEdit->textCursor().insertText(data.toLocal8Bit().toHex() +" "); // Вывод текста в консоль
    ui->textEdit->moveCursor(QTextCursor::End);//Scroll
}

void MainWindow::on_BtnSave_clicked()
{
savesettings(ui->PortNameBox->currentText(),
             ui->BaudRateBox->currentText().toInt(),
             ui->DataBitsBox->currentText().toInt(),
             ui->ParityBox->currentText().toInt(),
             ui->StopBitsBox->currentText().toInt(),
             ui->FlowControlBox->currentText().toInt() );
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_BtnConnect_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{
 savesettings("COM4",
              QSerialPort::Baud9600,
              QSerialPort::Data8,
              QSerialPort::NoParity,
              QSerialPort::OneStop,
              QSerialPort::NoFlowControl);
}

void MainWindow::on_cBtnSend_clicked()
{

}

void MainWindow::aboutWindow()
{
    QMessageBox::about(0, "About", "My Program Ver. " + versionOfProgramm);
}

uint8_t MainWindow::calculateCRC(uint8_t *pcBlock, uint32_t len)     // func from wikipedia
{   // func receive uint8_t array of data and lenght of this data (count will start from 1!)
    uint8_t crc = 0;//0xFF;
    uint32_t i;

    while (len--)
    {
        crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
}

double MainWindow::coordinatesTransfer(unsigned long int inches)     // func must take an inch value and return float value in millimeters. for excellon files only!
{
    unsigned int sign = 100;
    unsigned long int temp = 0;
    double result = 0;

    result = inches*25.4/100000;
    temp = result;
    result -= temp;
    result = (int)((result*sign)+0.5);
    result /= sign;

    return (double)temp + result;
}

double MainWindow::toolDiamTransfer(double toolDiam)        // convert inch to mms. for excellon files only!
{
    double value = toolDiam*2540;
    uint16_t temp = (uint16_t)value;
    return ((temp)/100.0);
}

void MainWindow::actionExcellonWasClicked()                 // // this function only for signal-slot system
{
}

void MainWindow::actionDXFWasClicked()                      // this function only for signal-slot system
{
}

void MainWindow::settingsChangedInForm()    // if we change & press "save" settings in form "Settings" - we must reload it in main form
{
    // read from file
ui->ProgrammCNC->clear();

// send to cnc if it works

}

void MainWindow::toolTipChange()
{
    if(ui->actionToolTip->isChecked() == true)
    {   // if checked


    } else
    {   // if not checked
 //       ui->ShpindleStopButton->too

    }
}

void MainWindow::on_RunButton_clicked()
{
    writeData("hiiiiiiiiiiiiiiii");
}

void MainWindow::on_ShpindleRightButton_clicked()
{
    uint8_t arr[OUT_ARR_SIZE] = {'[', 'M', 'R', 0xD5, ']' };
    writeData(QByteArray((char*)arr,5));
}

void MainWindow::on_ShpindleStopButton_clicked()
{
    uint8_t arr[OUT_ARR_SIZE] = {'[', 'M', 'S', 0xE4, ']' };
    writeData(QByteArray((char*)arr,5));
}

void MainWindow::on_ShpindleLeftButton_clicked()
{
    uint8_t arr[OUT_ARR_SIZE] = {'[', 'M', 'L', 0x89, ']' };
    writeData(QByteArray((char*)arr,5));
}

void MainWindow::on_checkBoxEnableLED_toggled(bool checked)
{
    if (checked) return;
}

void MainWindow::on_checkBoxEnableLED_clicked()
{
    uint8_t arr[OUT_ARR_SIZE] = {'[', 'A', 'L', 'Q', 0xAB, ']' };;
    if (ui->checkBoxEnableLED->isChecked() == true)
    {
        arr[3] = 'N';
        arr[4] = 0xAB;
//        qDebug(QByteArray((char*)arr,5));
    }
    else
    {
        arr[3] = 'F';
        arr[4] = 0x12;
//        qDebug(QByteArray((char*)arr,5));
    }
    writeData(QByteArray((char*)arr,5));
}

void MainWindow::on_checkBoxEnableAirCooling_clicked()
{
        uint8_t arr[OUT_ARR_SIZE] = {'[', 'A', 'F', 'Q', 0xAB, ']' };;
        if (ui->checkBoxEnableAirCooling->isChecked() == true)
        {
            arr[3] = 'N';
            arr[4] = 0x45;
        }
        else
        {
            arr[3] = 'F';
            arr[4] = 0xFC;
        }
        writeData(QByteArray((char*)arr,5));
}

void MainWindow::on_toPositiveEndOnZ_clicked()
{
    uint8_t arr[OUT_ARR_SIZE] = {'[', 'Z', 'P', 0x77, ']' };
    writeData(QByteArray((char*)arr,5));
}

void MainWindow::on_toNegativeEndOnZ_clicked()
{
    uint8_t arr[OUT_ARR_SIZE] = {'[', 'Z', 'M', 0x78, ']' };
    writeData(QByteArray((char*)arr,5));
}

void MainWindow::on_CheckCodeButton_clicked()
{
    uint32_t currentLine = 0;       // max value = 4294967295U

    if (codeFileType == NO_FILE)
    {
        QMessageBox::information(0, "Info", "Код обработки не загружен!");
        return;
    }

    QString currentString = ui->ProgrammCNC->toPlainText();
    QStringList strList = currentString.split('\n');

    if (strList.isEmpty())
    {
        QMessageBox::information(0, "Info", "Файл обработки пустой!");
        return;
    }
// and here we going to parsing all data
// so, firstly check file type
    if (codeFileType == EXCELLON_F)                                                         // if we opened excellon file...
    {
        uint8_t startSymbol = 0;
        double  axisValueX = 0,
                axisValueY = 0,
                toolDiam = 0;
        for (currentLine = 0; currentLine < strList.size(); currentLine++)                  // look all lines at loaded programm
        {
            QString str = strList.at(currentLine);                                          // making sring from stringList

                if (str.toStdString()[0] == '%')                                            // search for '%'
                {
                    if (startSymbol == 0)
                    {
                        ui->textBrowser->append("Список диаметров инструмента:");
                        startSymbol = 1;
                    } else
                    if (startSymbol == 1)
                    {
                        ui->textBrowser->append("Точки сверления:");
                        startSymbol = 3;
                    }
                }

                if (str.toStdString()[0] == 'T')                                                // search for 'T'
                {
                    if (startSymbol == 1)
                    {
                        toolDiam = toolDiamTransfer(str.mid(4).toDouble());
                        ui->textBrowser->append(str.left(3) +" = " +QString::number(toolDiam) +" mm.");      // Txx =
                    }

                    if (startSymbol == 3)  ui->textBrowser->append("\nNeeds tool " + str.left(3));
                }

                if (str.toStdString()[0] == 'X')                                                // search for 'X&Y'
                {
                    if (str.toStdString()[6] == 'Y')
                    {
                        axisValueX = coordinatesTransfer(str.mid(1,5).toLong());
                        axisValueY = coordinatesTransfer(str.mid(7).toLong());
                    }

                    if (str.toStdString()[7] == 'Y')
                    {
                        axisValueX = coordinatesTransfer(str.mid(1,6).toLong());
                        axisValueY = coordinatesTransfer(str.mid(8).toLong());
                    }

                    if (str.toStdString()[8] == 'Y')
                    {
                        axisValueX = coordinatesTransfer(str.mid(1,7).toLong());
                        axisValueY = coordinatesTransfer(str.mid(9).toLong());
                    }

                    ui->textBrowser->append("X=" + QString::number(axisValueX) + " Y=" + QString::number(axisValueY));
                }

                if (str.left(3) == "M30")                                                       // end of programm
                {
                        ui->textBrowser->append("End of parsing! :)");
                        startSymbol = 5;
                }
        }
    }
// ----algorithm of parsing excellon is ended----

// here parse for .dfx file
    if (codeFileType == DXF_F)
    {
        uint8_t startSymbol = 0;
        double  axisValueX = 0,
                axisValueY = 0;
        QString str;

       for (currentLine = 0; currentLine < strList.size(); currentLine++)                  // look all lines at loaded programm
       {
            str = strList.at(currentLine);
            if (str == "ENTITIES")                                    // looking for drawing immediately
            {
                ui->textBrowser->append("Объекты:");
                break;
            }
       }

       if (currentLine == strList.size())
       {
            ui->textBrowser->append("Ниодного объекта не найдено!");                        // if no drawing - go out
            return;
       }

       currentLine +=2;                                                         // jump to any entity type
       for (currentLine; currentLine < strList.size(); currentLine++)                  //
       {
            if (strList.at(currentLine) == "LINE")           // if entity is line..
            {
                ui->textBrowser->append("Линия:");
                ui->textBrowser->append("x1=" +strList.at(currentLine+20) +" y1=" +strList.at(currentLine+22));
                ui->textBrowser->append("x2=" +strList.at(currentLine+26) +" y2=" +strList.at(currentLine+28) +"\n");
                currentLine+=32;
            }

            if (strList.at(currentLine) == "CIRCLE")         // if entity is circle..
            {
                ui->textBrowser->append("Окружность:");
            }

            if (strList.at(currentLine) == "ARC")            // if entity is arc..
            {
                ui->textBrowser->append("Дуга:");
            }

            if (strList.at(currentLine) == "ELLIPSE")        // if entity is ellipse..
            {
                ui->textBrowser->append("Эллипс:");
            }
        }

    ui->textBrowser->append("Парсинг окончен!");

    }

}

void MainWindow::on_pushButton_3_clicked()      // тут мы переводим число из инчей в милиметры
{
    settings->setValue("COM/name", "COM2");
    settings->setValue("COM/baud", 9600);
    settings->setValue("COM/data", 8);
    settings->setValue("COM/parity", "none");
    settings->setValue("COM/stop", 1);
    settings->setValue("COM/control", "none");

    settings->setValue("SHPINDLE/speedMAX", 19000);
//    settings->setValue("SHPINDLE/rotation", "R");
    settings->setValue("SHPINDLE/delayON", 3);
    settings->setValue("SHPINDLE/delayOFF", 3);
//    settings->setValue("SHPINDLE/speedActual", 2000);

    settings->setValue("LASER/power", 123);
    settings->setValue("LASER/mode", 0.1);

    settings->setValue("AXIS/mode", 2);
    settings->setValue("AXIS/lenght", 2);
    settings->setValue("AXIS/steps", 2);
    settings->setValue("AXIS/fastMove", 2);
    settings->setValue("AXIS/slowMove", 2);

    settings->setValue("SOUND/programmEND", "TRUE");
    settings->setValue("SOUND/cycleEND", "TRUE");
    settings->setValue("SOUND/changeTool", "TRUE");
    settings->setValue("SOUND/axisSensors", "TRUE");
    settings->setValue("SOUND/repeat", "TRUE");



//    double q =  coordinatesTransfer(ui->cEnterText->text().toLong());
//    QString str = QString::number(q);
//    ui->consol->insertPlainText(str + "\n");
}

void MainWindow::on_pushButton_4_clicked()
{

}

void MainWindow::on_AddCodeButton_clicked()
{
//    QString temp = ui->CodeLineEdit->text();
//    ui->ProgrammCNC->append(temp);
    //here must be check for wrong type
    // writeData(temp);

    QByteArray data; // Текстовая переменная
    data = ui->CodeLineEdit->text().toLocal8Bit() + '\r'; // Присвоение "data" значения из EnterText
    writeData(data); // Отправка данных в порт
    Print(data); // Вывод данных в консоль
    qDebug("show sending data");

    ui->CodeLineEdit->clear();
    ui->CodeLineEdit->setFocus();
}

void MainWindow::on_ReloadCodeButton_clicked()
{
//    ui->ProgrammCNC->append(settings->value("general/value").toString());
}

void MainWindow::on_checkBoxManualInput_clicked()
{
    if (ui->checkBoxManualInput->isChecked())
    {
        ui->CodeLineEdit->setEnabled(true);
        ui->AddCodeButton->setEnabled(true);
    } else
    {
        ui->CodeLineEdit->setEnabled(false);
        ui->AddCodeButton->setEnabled(false);
    }
}

void MainWindow::on_toNegativeAxisX_clicked()
{

}

void MainWindow::on_toNegativeAxisZ_clicked()
{

}

void MainWindow::on_toPositiveAxisZ_clicked()
{

}

void MainWindow::on_pushButton_5_clicked()
{

}

void MainWindow::on_toZeroOnZ_clicked()
{

}


void MainWindow::on_pushButtonGOTO_clicked()
{
    if (ui->comboBoxGOTO->currentIndex() != NONE){
        uint8_t arr[OUT_ARR_SIZE] = {'[',
                                     GOTO_COMMAND,
                                     (uint8_t)(ui->comboBoxGOTO->currentIndex()),
                                     0x77,
                                     ']' };

        writeData(QByteArray((char*)arr,5));

    }
}












void MainWindow::on_toPositiveAxisY_clicked()
{

}
