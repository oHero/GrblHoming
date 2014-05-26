#include <QString>
#include "../rs232.h"

RS232::RS232()
    : isOpen(false), cmdCount(0)
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
    // Use same as for 'line'. Technically this is not the correct approach, but it works ok for mocking
    return pollTest(buf, size);
}

int RS232::PollComportLine(char *buf, int size)
{
    return pollTest(buf, size);
}

int RS232::SendBuf(const char *buf, int size)
{
    if (!isOpen)
    {
        err("Port not open\n");
        return 0;
    }
    if (cmdCount >= responseList.size())
    {
        err("Too many commands for response list\n");
        return 0;
    }
    int elapsed = responseTimer.elapsed();
    diag("Sending command at elapsed time %d\n", elapsed);

    responseList[cmdCount].haveCmd = true;
    responseList[cmdCount].timeDelayMs += elapsed;

    cmdCount++;

    return size;
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
    if (!responseList.size())
        return 0;

    int elapsed = responseTimer.elapsed();
    foreach (ItemTimed it, responseList)
    {
        if (!it.haveCmd)
            return 0;

        if (it.timeDelayMs < elapsed)
            return it.item.size();
    }

    return 0;
}

void RS232::setCharSendDelayMs(int charSendDelayMs)
{
}

#ifdef QTTEST
void RS232::addResponseListItem(const QString item, int timeDelayMs /* = -1 */)
{
    responseList.append(ItemTimed(item, timeDelayMs));
}

void RS232::startClock()
{
    responseTimer.restart();
}

int RS232::pollTest(char *buf, int size)
{
    if (!isOpen)
        return 0;
    if (!responseList.size())
        return 0;

    int elapsed = responseTimer.elapsed();
    foreach (ItemTimed it, responseList)
    {
        if (!it.haveCmd)
            return 0;

        if (it.timeDelayMs < elapsed)
        {
            int ret = qMin(size, it.item.size());
            // trick to implement, ok to fail
            if (ret < it.item.size())
                return -1;
            memcpy(buf, it.item.toAscii().constData(), ret);
            responseList.removeAt(0);
            cmdCount--;
            return ret;
        }
    }
    return 0;
}

#endif
