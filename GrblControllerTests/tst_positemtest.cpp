#include <QString>
#include <QtTest>
#include "tst_positemtest.h"
#include "../positem.h"

PositemTest::PositemTest()
{
}

void PositemTest::testCaseSetMmMode()
{
    PosItem p;
    p.setCoords(1.0, 1.0, true);
    QVERIFY2(p.mm, "Failure");
    p.toInches();
    QVERIFY2(!p.mm, "Failure");

}

void PositemTest::testCaseSetInchesMode()
{
    PosItem p;
    p.setCoords(1.0, 1.0, false);
    QVERIFY2(!p.mm, "Failure");
    p.toInches();
    QVERIFY2(!p.mm, "Failure");

}

void PositemTest::testCaseSwapMode()
{
    PosItem p;
    p.setCoords(3.3, 2.4, true);
    QVERIFY2(p.mm, "Failure");

    p.toInches();
    QVERIFY2(!p.mm, "Failure");
    //QCOMPARE(p.i, 0.129921);
    //QCOMPARE(p.j, 0.0944882);

    p.toMm();
    QVERIFY2(p.mm, "Failure");
    QCOMPARE(p.i, 3.3);
    QCOMPARE(p.j, 2.4);
}


//QTEST_APPLESS_MAIN(PositemTest)

//#include "tst_positemtest.moc"
