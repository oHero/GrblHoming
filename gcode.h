/****************************************************************
 * gcode.h
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef GCODE_H
#define GCODE_H

#include "log4qtdef.h"

#include <QString>
#include <QFile>
#include <QThread>
#include <QTextStream>
#include "definitions.h"
#include "coord3d.h"
#include "controlparams.h"
#include "cmdresponse.h"
#include "grblinterface.h"

#define BUF_SIZE 300

#define RESPONSE_OK "ok"
#define RESPONSE_ERROR "error"

// as defined in the grbl project on github...
#define GRBL_RX_BUFFER_SIZE     128

#define CTRL_X '\x18'

#define DEFAULT_AXIS_COUNT      3
#define MAX_AXIS_COUNT          4


class DecimalFilter
{
public:
    DecimalFilter(QString t) : token(t), decimals(0) {}
public:
    QString token;
    int decimals;
};

class GCode : public QObject
{
    Q_OBJECT

public:
    GCode(GrblInterface& grblIfc);
    void setAbort();
    void setReset();
    void setShutdown();

    static void trimToEnd(QString& strline, QChar);

signals:
    void addList(QString line);
    void addListFull(QStringList list);
    void addListOut(QString line);
    void sendMsg(QString msg);
    void stopSending();
    void setCommandText(QString value);
    void adjustedAxis();
    void gcodeResult(int id, QString result);
    void setProgress(int);
    void setQueuedCommands(int, bool);
    void resetTimer(bool timeIt);
    void enableGrblDialogButton();
    void setUnitsWork(QString value);
    void setUnitsMachine(QString value);
    void setVisCurrLine(int currLine);

public slots:
    void sendGcode(QString line);
    void sendGcodeAndGetResult(int id, QString line);
    void sendFile(QString path);
    void gotoXYZFourth(QString line);
    void axisAdj(char axis, float coord, bool inv, bool absoluteAfterAxisAdj, int sliderZCount);
    void setResponseWait(ControlParams controlParams);
    void grblSetHome();
    void sendGrblReset();
    void sendGrblUnlock();
    void goToHome();

private:
    enum PosReqStatus
    {
        POS_REQ_RESULT_OK,
        POS_REQ_RESULT_ERROR,
        POS_REQ_RESULT_TIMER_SKIP,
        POS_REQ_RESULT_UNAVAILABLE
    };
private:
    bool sendGcodeLocal(bool waitForResp, QString line, bool recordResponseOnFail = false, int waitSec = -1, int currLine = 0);
    bool waitForStartupBanner(QString& result, int waitSec, bool failOnNoFound);
    QString removeUnsupportedCommands(QString line);
    QString reducePrecision(QString line);
    bool isGCommandValid(float value, bool& toEndOfLine);
    bool isMCommandValid(float value);
    QString getMoveAmountFromString(QString prefix, QString item);
    bool SendJog(QString strline, bool absoluteAfterAxisAdj);
    void pollPosWaitForIdle(bool checkMeasurementUnits);
    void checkAndSetCorrectMeasurementUnits();
    void setOldFormatMeasurementUnitControl();
    void setUnitsTypeDisplay(bool millimeters);
    void setConfigureMmMode(bool setGrblUnits);
    void setConfigureInchesMode(bool setGrblUnits);
    QStringList doZRateLimit(QString strline, QString& msg, bool& xyRateSet);
    bool checkGrbl(const QString& result);
    PosReqStatus positionUpdate(bool forceIfEnabled = false);

private:
    ControlParams controlParams;
    Coord3D machineCoordLastIdlePos, workCoordLastIdlePos;
    QStringList grblCmdErrors;
    QStringList grblFilteredCmds;
    QTime pollPosTimer;
    GrblInterface& grbl;
};

#endif // GCODE_H
