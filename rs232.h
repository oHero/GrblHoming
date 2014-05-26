/****************************************************************
 * rs232.h
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef RS232_H
#define RS232_H

#include <QtGlobal>

#include <stdio.h>
#include <string.h>
#if defined(Q_OS_LINUX) || defined(Q_OS_MACX) || defined(Q_OS_ANDROID)
#include <termios.h>
#include "../termiosext.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#else
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <Windows.h>
#else
// TODO - R - if I leave out Windows.h then Sleep is not found???
#include <Windows.h>
#include <QtGui/QWindow>
#endif
#endif

#ifndef QTTEST
#include <QMessageBox>
#include <qextserialport.h>
#include <qextserialenumerator.h>
#else
#include <QStringList>
#endif

#include "definitions.h"


#if defined(Q_OS_LINUX) || defined(Q_OS_MACX) || defined(Q_OS_ANDROID)
#define SLEEP(x) usleep(1000 * x);
#else
#define SLEEP(x) Sleep(x);
#endif


class RS232
{
public:
    RS232();
    //methods
    bool OpenComport(QString commPortStr, QString baudRate);
    int PollComport(char *buf, int size);
    int PollComportLine(char *buf, int size);
    int SendBuf(const char *buf, int size);
    void CloseComport();
    void Reset();
    void flush();
    bool isPortOpen();
    QString getDetectedLineFeed();
    int bytesAvailable();
    void setCharSendDelayMs(int charSendDelayMs);

private:
#ifndef QTTEST
    QextSerialPort *port;
    char detectedEOL;
    QString detectedLineFeed;
    int charSendDelayMs;
#else
    class ItemTimed
    {
    public:
        ItemTimed(QString i, int t) : item(i), timeDelayMs(t), haveCmd(false) {}
        QString item;
        int timeDelayMs;
        bool haveCmd;
    };

    bool isOpen;
    QList<ItemTimed> responseList;
    QTime responseTimer;
    int cmdCount;

public:
    void addResponseListItem(const QString item, int timeDelayMs = -1);
    void startClock();
private:
    int pollTest(char *buf, int size);

#endif
};


#endif // RS232_H
