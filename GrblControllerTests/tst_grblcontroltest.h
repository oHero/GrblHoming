#ifndef TST_GRBLCONTROLTEST_H
#define TST_GRBLCONTROLTEST_H
#include <QString>
#include <QtTest>

#include "../coord3d.h"
#include "../rs232.h"
#include "../grblcontrol.h"

class GrblControlTest : public QObject
{
    Q_OBJECT

public:
    GrblControlTest();

private:

public slots:

private Q_SLOTS:
    /*
    void testCaseSendOneGood();
    void testCaseSendOneNoResponse();
    void testCaseSendTwoGood();
    */
    void testCaseFillSendBuf();





};
#endif // TST_GRBLCONTROLTEST_H
