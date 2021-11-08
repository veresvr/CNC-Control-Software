#include "filesparsing.h"


returnCommand excellonparsing(QString str)
{
    returnCommand ExcellonCommand;
    uint8_t startSymbol = 0;
    double  axisValueX = 0,
            axisValueY = 0,
            toolDiam = 0;
/*
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
*/

    ExcellonCommand.StatusOfParsing = SUCCESS;
    return ExcellonCommand;
}

//-------------------------------------------------------------------------------
returnCommand dxfparsing()
{
    returnCommand DXFCommand;













    DXFCommand.StatusOfParsing = SUCCESS;
    return DXFCommand;
}

//-------------------------------------------------------------------------------
returnCommand gcodeparsing(QString str)
{
    returnCommand GCodeCommand;


    GCodeCommand.StatusOfParsing = SUCCESS;
    return GCodeCommand;
}

