#ifndef GRBLCONTROL_H
#define GRBLCONTROL_H
#include "log4qtdef.h"

#include <QString>
#include <QStringList>
#include <QFile>
#include <QThread>
#include <QTextStream>

#include <QObject>
#include "rs232.h"
#include "controlparams.h"
#include "cmdresponse.h"

#define BUF_SIZE 300

#define RESPONSE_OK "ok"
#define RESPONSE_ERROR "error"

// as defined in the grbl project on github...
#define GRBL_RX_BUFFER_SIZE     128

#define CTRL_X '\x18'

#define DEFAULT_AXIS_COUNT      3
#define MAX_AXIS_COUNT          4

class GrblControl : public QObject
{
    Q_OBJECT
public:
    GrblControl(RS232& rs);
    
    bool isGrblBufHaveRoom();
    bool sendCmd(QString line, int currLine);
    bool waitForResponses(int waitSec);
    bool waitForAllResponses(int waitSec);

signals:
    
public slots:

private:
    RS232& port;
    bool doubleDollarFormat;
    bool motionOccurred;

    ControlParams controlParams;
    QList<CmdResponse> sendCount;
};

#endif // GRBLCONTROL_H
