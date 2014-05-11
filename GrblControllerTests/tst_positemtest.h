#ifndef TST_POSITEM_H
#define TST_POSITEM_H
#include <QString>
#include <QtTest>

class PositemTest : public QObject
{
    Q_OBJECT

public:
    PositemTest();

private Q_SLOTS:
    void testCaseSetMmMode();
    void testCaseSetInchesMode();
    void testCaseSwapMode();
};
#endif // TST_POSITEM_H
