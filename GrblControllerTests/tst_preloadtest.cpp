#include <QString>
#include <QtTest>
#include "../grblinterface.h"
#include "tst_preloadtest.h"

PreloadTest::PreloadTest()
    : portOpen(false)
{
}

//--------------- tests --------------------------
/*
void PreloadTest::testCaseSuccessOpenPort()
{
    RS232 mock;
    GrblInterface grbl(mock);
    connectTo(grbl);

    grbl.openPort("blah", "123");

    QVERIFY2(portOpen, "Failure");
}

void PreloadTest::testCaseFailOpenPort()
{
    RS232 mock;
    GrblInterface grbl(mock);
    connectTo(grbl);

    grbl.openPort("", "");

    QVERIFY2(!portOpen, "Failure");
}

void PreloadTest::testCaseSend1()
{
    RS232 mock;
    GrblInterface grbl(mock);
    connectTo(grbl);

    grbl.openPort("blah", "123");

    QVERIFY2(portOpen, "Failure");

    mock.addResponseListItem("GRBL 0.84");

    QString result;
    QStringList grblCmdErr;
    bool recordResponseOnFail = false;
    int waitSec = 2;
    bool status = grbl.sendWithBlock("\r\n", result, grblCmdErr, recordResponseOnFail, waitSec, true);
    QVERIFY2(status, "Failure");
    QVERIFY2(result.length() > 0, "Failure");
}
*/
void PreloadTest::testCaseFillSendBuf()
{
    RS232 mock;
    GrblInterface grbl(mock);
    connectTo(grbl);

    const int itemCount = 10;

    bool result = mock.OpenComport("a", "b");
    QVERIFY2(result, "Failed opening port");

    mock.addResponseListItem("ok\n", 500);
    mock.addResponseListItem("ok\n", 500);
    mock.addResponseListItem("ok\n", 500);
    mock.addResponseListItem("ok\n", 500);
    mock.addResponseListItem("ok\n", 1000);

    mock.addResponseListItem("ok\n", 1000);
    mock.addResponseListItem("ok\n", 1000);
    mock.addResponseListItem("ok\n", 1000);
    mock.addResponseListItem("ok\n", 1000);
    mock.addResponseListItem("ok\n", 1000);

    mock.startClock();


    for (int i = 0; i < itemCount; i++)
    {
        QString result;
        QStringList grblCmdErr;
        QVERIFY2(grbl.sendCmd(QString("a%1---------------------------------").arg(i), result, grblCmdErr, false, 1, i + 1),
                 QString("Failed sending command for position %1").arg(i).toLatin1().constData());
    }

//    result = grbl.waitForAllResponses(1);
    QVERIFY2(result, "Failed waiting for commands");
}

//-------------- private ---------------------------
void PreloadTest::connectTo(GrblInterface& grbl)
{
    connect(&grbl, SIGNAL(sendMsg(QString)),this,SLOT(sendMsg(QString)));
    connect(&grbl, SIGNAL(addList(QString)),this,SLOT(addList(QString)));
    connect(&grbl, SIGNAL(addListFull(QStringList)),this,SLOT(addListFull(QStringList)));
    connect(&grbl, SIGNAL(addListOut(QString)),this,SLOT(addListOut(QString)));
    connect(&grbl, SIGNAL(portIsClosed(bool)), this, SLOT(portIsClosed(bool)));
    connect(&grbl, SIGNAL(portIsOpen(bool)), this, SLOT(portIsOpen(bool)));
    connect(&grbl, SIGNAL(setQueuedCommands(int, bool)), this, SLOT(setQueuedCommands(int, bool)));
    connect(&grbl, SIGNAL(updateCoordinates(Coord3D,Coord3D)), this, SLOT(updateCoordinates(Coord3D,Coord3D)));
    connect(&grbl, SIGNAL(setLastState(QString)), this, SLOT(setLastState(QString)));
    connect(&grbl, SIGNAL(setLivePoint(double, double, bool, bool)), this, SLOT(setLivePoint(double, double, bool, bool)));
    connect(&grbl, SIGNAL(setVisualLivenessCurrPos(bool)), this, SLOT(setVisualLivenessCurrPos(bool)));
    connect(&grbl, SIGNAL(setLcdState(bool)), this, SLOT(setLcdState(bool)));
}
