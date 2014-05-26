#ifndef TST_PRELOADTEST_H
#define TST_PRELOADTEST_H
#include <QString>
#include <QtTest>

#include "../coord3d.h"
#include "../rs232.h"
#include "../grblinterface.h"

class PreloadTest : public QObject
{
    Q_OBJECT

public:
    PreloadTest();

private:
    void connectTo(GrblInterface& grbl);

public slots:
    void addList(QString line) {}
    void addListFull(QStringList list) {}
    void addListOut(QString line) {}
    void sendMsg(QString msg) {}
    void portIsOpen(bool sendCode) { portOpen = true; }
    void portIsClosed(bool reopen) { portOpen = false; }
    void setQueuedCommands(int, bool) {}
    void updateCoordinates(Coord3D machineCoord, Coord3D workCoord) {}
    void setLastState(QString state) {}
    void setLivePoint(double x, double y, bool isMM, bool isLiveCP) {}
    void setLcdState(bool valid) {}
    void setVisualLivenessCurrPos(bool isLiveCP) {}

private Q_SLOTS:
    //void testCaseSuccessOpenPort();
    //void testCaseFailOpenPort();
    //void testCaseSend1();
    void testCaseFillSendBuf();

private:
    //RS232 mock;
    //GrblInterface grbl;

    bool portOpen;
};
#endif // TST_PRELOADTEST_H
