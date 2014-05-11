#include <QString>
#include "../rs232.h"

RS232::RS232()
    : isOpen(false)
{
}

bool RS232::OpenComport(QString commPortStr, QString baudRate)
{
    isOpen = false;

    if (commPortStr.length() == 0)
        return false;
    if (baudRate.length() == 0)
        return false;

    isOpen = true;
    return true;
}

int RS232::PollComport(char *buf, int size)
{
    if (!isOpen)
        return 0;
    if (responseList.size() == 0)
        return 0;
    int ret = qMin(size, responseList.at(0).size());
    // trick to implement, ok to fail
    if (ret < responseList.at(0).size())
        return -1;
    memcpy(buf, responseList.at(0).constData(), ret);
    return ret;
}

int RS232::PollComportLine(char *buf, int size)
{
    if (!isOpen)
        return 0;
    if (responseList.size() == 0)
        return 0;
    int ret = qMin(size, responseList.at(0).size());
    // trick to implement, ok to fail
    if (ret < responseList.at(0).size())
        return -1;
    memcpy(buf, responseList.at(0).constData(), ret);
    return ret;
}

int RS232::SendBuf(const char *buf, int size)
{
    return isOpen ? size : 0;
}

void RS232::CloseComport()
{
    isOpen = false;
}

void RS232::Reset()
{
    isOpen = false;
}

void RS232::flush()
{
}

bool RS232::isPortOpen()
{
    return isOpen;
}

QString RS232::getDetectedLineFeed()
{
    return "\n";
}

int RS232::bytesAvailable()
{
    if (responseList.size() == 0)
        return 0;
    return responseList.at(0).size();
}

void RS232::setCharSendDelayMs(int charSendDelayMs)
{
}

#ifdef QTTEST
void RS232::setResponseList(const QStringList& rl)
{
    responseList = rl;
}
#endif
