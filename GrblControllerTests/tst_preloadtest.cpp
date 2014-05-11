#include <QString>
#include <QtTest>
#include "../grblinterface.h"
#include "tst_preloadtest.h"

PreloadTest::PreloadTest()
    : portOpen(false)
{
}

//--------------- tests --------------------------
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

    QStringList responseList;
    responseList.append("GRBL 0.84");
    mock.setResponseList(responseList);

    QString result;
    QStringList grblCmdErr;
    bool recordResponseOnFail = false;
    int waitSec = 2;
    bool status = grbl.sendWithBlock("\r\n", result, grblCmdErr, recordResponseOnFail, waitSec, true);
    QVERIFY2(status, "Failure");
    QVERIFY2(result.length() > 0, "Failure");
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
