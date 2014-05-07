#ifndef GRBLINTERFACE_H
#define GRBLINTERFACE_H
#include "log4qtdef.h"

#include <QString>
#include <QFile>
#include <QThread>
#include <QTextStream>
#include "definitions.h"
#include "rs232.h"
#include "coord3d.h"
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

class GrblInterface : public QObject
{
    Q_OBJECT

public:
    GrblInterface();
    bool sendWithBlock(QString line, QString& result, QStringList& grblCmdErr, bool recordResponseOnFail = false, int waitSec = -1, int currLine = 0);
    void initCmdSend();
    bool waitForAllResponses(QStringList& grblCmdErr);
    bool isPortOpen();
    void clearToHome();
    bool sendToPort(const char *buf);
    void resetPort();
    int PollComportLine(char *buf, int size);
    QString getDetectedLineFeed();
    void setParams(ControlParams controlParamsIn);
    void setDoubleDollarFormat(bool isDD);
    bool isDoubleDollarFormat();
    void clearErrorCount();
    void incErrorCount();
    int getErrorCount();
    bool isIncorrectMeasurementUnits();
    void clearIncorrectMeasurementUnits();
    bool isIncorrectLcdDisplayUnits();
    void clearIncorrectLcdDisplayUnits();
    bool isMotionOccurred();
    void clearMotionOccurred();
    double getMaxZ();
    void reduceMaxZ(double reduceAmount);
    void setSliderZCount(int count);
    int getNumAxis();
    void setNumAxis(int count);
    QString getLastState();
    Coord3D getMachineCoord();
    Coord3D getWorkCoord();
    void restartParseCoordTimer();

    void setAbortState(bool state);
    bool getAbortState();
    void setResetState(bool state);
    bool getResetState();
    void setShutdownState(bool state);
    bool getShutdownState();

    void sendStatusList(QStringList& listToSend);
    void setLivenessState(bool valid);

    static bool checkForGetPosStr(QString& line);

signals:
    void addList(QString line);
    void addListFull(QStringList list);
    void addListOut(QString line);
    void sendMsg(QString msg);
    void portIsOpen(bool sendCode);
    void portIsClosed(bool reopen);
    void setQueuedCommands(int, bool);
    void updateCoordinates(Coord3D machineCoord, Coord3D workCoord);
    void setLastState(QString state);
    void setLivePoint(double x, double y, bool isMM, bool isLiveCP);
    void setLcdState(bool valid);
    void setVisualLivenessCurrPos(bool isLiveCP);

public slots:
    void openPort(QString commPortStr, QString baudRate);
    void closePort(bool reopen);

protected:
    void timerEvent(QTimerEvent *event);

private:
    bool waitForOk(QString& result, int waitSec, bool sentReqForLocation, bool sentReqForParserState, bool finalize, QStringList& grblCmdErr);
    void parseCoordinates(const QString& received);


private:
    RS232 port;
    ControlParams controlParams;
    AtomicIntBool abortState;
    AtomicIntBool resetState;
    AtomicIntBool shutdownState;

    int errorCount;
    QString currComPort;
    bool doubleDollarFormat;
    QString lastState;
    bool incorrectMeasurementUnits;
    bool incorrectLcdDisplayUnits;
    Coord3D machineCoord, workCoord;
    double maxZ;
    QList<CmdResponse> sendCount;
    QTime parseCoordTimer;
    bool motionOccurred;
    int sliderZCount;
    bool positionValid;
    int numaxis;
};

#endif // GRBLINTERFACE_H
