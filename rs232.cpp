/****************************************************************
 * rs232.cpp
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include "rs232.h"

RS232::RS232()
    : port(NULL), detectedEOL(0), charSendDelayMs(DEFAULT_CHAR_SEND_DELAY_MS), detectedLineFeedSize(0)
{
}

RS232::~RS232()
{
    CloseComport();
}

bool RS232::OpenComport(QString commPortStr, QString baudRate)
{
    if (port != NULL)
        CloseComport();

    bool ok;
    BaudRateType baud = (BaudRateType)baudRate.toInt(&ok);
    if (!ok)
    {
        baud = BAUD9600;
    }
    else
    {
        int possibleBaudRates[] = {BAUD110,BAUD300,BAUD600,BAUD1200,BAUD2400,BAUD4800,BAUD9600,BAUD19200,BAUD38400,BAUD57600,BAUD115200};
        int pbrCount = sizeof possibleBaudRates / sizeof possibleBaudRates[0];

        bool found = false;
        for (int i = 0; i < pbrCount; i++)
        {
            if (baud == possibleBaudRates[i])
            {
                found = true;
                break;
            }
        }
        if (!found)
            baud = BAUD9600;
    }

    PortSettings settings = {baud, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};

    port = new QextSerialPort(commPortStr, settings);// default is event driven, alternate:, QextSerialPort::Polling);

    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));

    port->open(QIODevice::ReadWrite);

    return port->isOpen();
}

bool RS232::SendBuf(const char *buf, int size)
{
    if (port == NULL || !port->isOpen())
        return false;
/// LETARTARE  for test
//err(buf) ;
    if (size <= 0)
    {
        err( qPrintable(QObject::tr("Unexpected: Told to send %d bytes\n")), size) ;
        return false;
    }

    char b[300] = {0};
    memcpy(b, buf, size);
#ifdef DIAG
    printf("Sending to port %s [%s]:", port->portName().toLocal8Bit().constData(), b);
    for (int x= 0; x < size; x++)
    {
        printf("%02X ", buf[x]);
    }
    printf("\n");
    fflush(stdout);
#endif

    port->waitForBytesWritten(-1);// this usually doesn't do anything, but let's put it here in case

#if 1
    // On very fast PCs running Windows we have to slow down the sending of bytes to grbl
    // because grbl loses bytes due to its interrupt service routine (ISR) taking too many clock
    // cycles away from serial handling.
    int result = 0;
    for (int i = 0; i < size; i++)
    {
        result = port->write(&buf[i], 1);
        /* only applicable if port opened in poll mode
        if (result == 0)
        {
            err("Unable to write bytes to port probably due to outgoing queue full. Write data lost!");
            break;
        }
        else */ if (result < 0)
        {
            err("Error writing to port. Write data lost!");
            result = 0;
            break;
        }

        if (charSendDelayMs > 0)
        {
            SLEEP(charSendDelayMs);
        }
    }

#else
    // DO NOT RUN THIS CODE
    int result = port->write(buf, size);
    if (result == 0)
    {
        err("Unable to write bytes to port probably due to outgoing queue full. Write data lost!");
        /* the following code doesn't seem to help. Generate an error instead
        int limit = 0;
        while (!result && limit < 100)
        {
            SLEEP(100);
            result = port->write(buf, size);
            limit++;
        }

        if (!result)
        {
            err("Unable to write %d bytes to port!", size);
        }
        else if (result != size)
            err("Unexpected: Retry send wrote %d bytes out of expected %d\n", result, size);
        */
    }
    else if (result == -1)
    {
        err("Error writing to port. Write data lost!");
        result = 0;
    }
#endif
    return result >= 0;
}


void RS232::CloseComport()
{
    if (port != NULL)
    {
        port->close();
        delete port;
        port = NULL;
    }
}

void RS232::Reset() //still to test
{
    if (port != NULL)
        port->reset();
}

/*
void RS232::flush()
{
    int n=1;
    char buf[255];

    while (n > 0)
        n = PollComport(buf,255);
}
*/

bool RS232::isPortOpen()
{
    if (port == NULL)
        return false;

    return port->isOpen();
}

QString RS232::getDetectedLineFeed()
{
    return detectedLineFeed;
}

bool RS232::haveData()
{
    return responses.size() > 0;
}

int RS232::getLine(char *buf, const int bufSize)
{
    if (bufSize == 0)
        return 0;

    if (responses.isEmpty())
    {
        warn("Our reponse list is empty. Should call haveData before calling getLine");
        return 0;
    }

    QString s = responses.takeFirst();
    int sz = qMin(bufSize, s.size());
    if (s.size() > bufSize)
        warn("Get line lost data because buffer size too small (Buf size:%d) (Item size:%d)\n", bufSize, s.size());
    memcpy(buf, s.toAscii().constData(), sz);
    return sz;
}

void RS232::setCharSendDelayMs(int csd)
{
    charSendDelayMs = csd;
}

void RS232::onDataAvailable()
{
    receivedData.append(port->readAll());

    if (detectedEOL == 0)
    {
        int pos = 0;
        char firstEOL = 0;
        char secondEOL = 0;
        for (int i = 0; i < receivedData.size(); i++)
        {
            char b = receivedData.at(i).toLatin1();
            if (b == '\n' || b == '\r')
            {
                if (firstEOL == 0)
                {
                    firstEOL = b;
                    pos = i;
                }
                else if ((pos + 1) == i)
                {
                    secondEOL = b;
                    break;
                }
                else
                    break;
            }
        }

        if (firstEOL != 0)
        {
            if (secondEOL != 0)
            {
                detectedEOL = secondEOL;
                detectedLineFeed = firstEOL;
                detectedLineFeed += secondEOL;
            }
            else
            {
                detectedEOL = firstEOL;
                detectedLineFeed = firstEOL;
            }

            detectedLineFeedSize = detectedLineFeed.size();
        }
    }

    if (detectedEOL)
    {
        // ok, we have EOL, we can no start splitting things up

        // first, check if end of bytes contains our line feed string
        if (receivedData.size() >= detectedLineFeedSize)
        {
            int last = receivedData.lastIndexOf(detectedLineFeed);
            if (last > 0)
            {
                QString s(receivedData.right(last + detectedLineFeedSize));
                responses.append(s.split(detectedLineFeed, QString::SkipEmptyParts));
                receivedData.remove(0, last + detectedLineFeedSize);
            }
        }
    }
}
