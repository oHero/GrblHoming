#include <QString>
#include <QtTest>
#include "../grblcontrol.h"
#include "tst_grblcontroltest.h"

GrblControlTest::GrblControlTest()
{
}

//--------------- tests --------------------------
/*
void GrblControlTest::testCaseSendOneGood()
{
    RS232 mock;
    GrblControl grbl(mock);

    mock.addResponseListItem("ok\n", 400);
    mock.startClock();

    bool result = mock.OpenComport("a", "b");
    QVERIFY2(result, "Failed opening port");

    result = grbl.sendCmd("abc", 1);
    QVERIFY2(result, "Failed sending command");

    result = grbl.waitForAllResponses(1);
    QVERIFY2(result, "Failed waiting for command");
}

void GrblControlTest::testCaseSendOneNoResponse()
{
    RS232 mock;
    GrblControl grbl(mock);

    // no data
    QStringList responseList;
    mock.setResponseList(responseList);

    bool result = mock.OpenComport("a", "b");
    QVERIFY2(result, "Failed opening port");

    result = grbl.sendCmd("abc", 1);
    QVERIFY2(result, "Failed sending command");

    result = grbl.waitForAllResponses(1);
    QVERIFY2(!result, "Got command unexpectedly");
}

void GrblControlTest::testCaseSendTwoGood()
{
    RS232 mock;
    GrblControl grbl(mock);

    QStringList responseList;
    responseList.append("ok\n");
    responseList.append("ok\n");
    mock.setResponseList(responseList);

    bool result = mock.OpenComport("a", "b");
    QVERIFY2(result, "Failed opening port");

    result = grbl.sendCmd("abc", 1);
    QVERIFY2(result, "Failed sending command");

    result = grbl.sendCmd("def", 2);
    QVERIFY2(result, "Failed sending command");

    result = grbl.waitForAllResponses(1);
    QVERIFY2(result, "Failed waiting for commands");
}

void GrblControlTest::testCaseFillSendBuf()
{
    RS232 mock;
    GrblControl grbl(mock);
    const int itemCount = 10;

    bool result = mock.OpenComport("a", "b");
    QVERIFY2(result, "Failed opening port");

    const int switchPoint = 2;
    for (int i = 0; i < itemCount; i++)
    {
        if (i >= switchPoint)
        {
            if (i == switchPoint)
            {
                // add a bunch all at once to simulate delayed response
                for (int j = 0; j < i; j++)
                {
                    mock.addResponseListItem("ok\n");
                }
            }
            else
            {
                mock.addResponseListItem("ok\n");
            }
        }
        QVERIFY2(grbl.sendCmd(QString("a%1---------------------------------").arg(i), i + 1),
                 QString("Failed sending command for position %1").arg(i).toLatin1().constData());
    }

    mock.addResponseListItem("ok\n");

    result = grbl.waitForAllResponses(1);
    QVERIFY2(result, "Failed waiting for commands");
}
*/
void GrblControlTest::testCaseFillSendBuf()
{
    RS232 mock;
    GrblControl grbl(mock);
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
        QVERIFY2(grbl.sendCmd(QString("a%1---------------------------------").arg(i), i + 1),
                 QString("Failed sending command for position %1").arg(i).toLatin1().constData());
    }

    result = grbl.waitForAllResponses(1);
    QVERIFY2(result, "Failed waiting for commands");
}
