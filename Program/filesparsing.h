#include <QObject>
#include <stdint.h>
#include <veres_defines_list.h>

//#define OUT_DATA_LENGTH  20;



typedef struct returnCommandType {
    uint8_t StatusOfParsing;
    QByteArray data[5];                     // number of data rows
    uint8_t numberOfLines;
};

returnCommandType excellonparsing(QString str);
returnCommandType dxfparsing();
returnCommandType gcodeparsing(QString str);

