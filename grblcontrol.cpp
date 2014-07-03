#include <QObject>
#include "grblcontrol.h"
#include "grblinterface.h"

GrblControl::GrblControl(RS232& rs)
    : port(rs), doubleDollarFormat(false), motionOccurred(false)
{
}

bool GrblControl::sendCmd(QString line, int currLine)
{
    if (!port.isPortOpen())
    {
        return false;
    }

    bool ctrlX = line.size() > 0 ? (line.at(0).toLatin1() == CTRL_X) : false;

    bool sentReqForLocation = false;
    bool sentReqForSettings = false;
    bool sentReqForParserState = false;

    if (GrblInterface::checkForGetPosStr(line))
    {
        sentReqForLocation = true;
    }
    else if (!line.compare(REQUEST_PARSER_STATE_V08c))
    {
        sentReqForParserState = true;
    }
    else if (!line.compare(SETTINGS_COMMAND_V08a))
    {
        if (doubleDollarFormat)
            line = SETTINGS_COMMAND_V08c;

        sentReqForSettings = true;
    }
    else
        motionOccurred = true;

    if (line.size() == 0 || (!line.endsWith('\r') && !ctrlX))
        line.append('\r');

    char buf[BUF_SIZE + 1] = {0};
    if (line.length() >= BUF_SIZE)
    {
        return false;
    }

    for (int i = 0; i < line.length(); i++)
        buf[i] = line.at(i).toLatin1();

    while (!isGrblBufHaveRoom() || port.haveData())
    {
        if (!waitForResponses(1))
        {
            return false;
        }
    }

    if (ctrlX)
        sendCount.append(CmdResponse("(CTRL-X)", line.length(), currLine));
    else
        sendCount.append(CmdResponse(buf, line.length(), currLine));

    if (!port.SendBuf(buf, line.length()))
    {
        return false;
    }
    else
    {
    }
    return true;
}

bool GrblControl::isGrblBufHaveRoom()
{
    int total = 0;
    bool haveWait = false;
    foreach (CmdResponse cmdResp, sendCount)
    {
        total += cmdResp.count;
        if (cmdResp.waitForMe)
        {
            haveWait = true;
        }
    }

    //printf("Total out (a): %d (%d) (%d)\n", total, sendCount.size(), haveWait);

    if (!haveWait)
    {
        if (total < (GRBL_RX_BUFFER_SIZE - 1))
        {
            return true;
        }
    }

    return false;
}

bool GrblControl::waitForAllResponses(int waitSec)
{
    while (!sendCount.isEmpty())
    {
        if (!waitForResponses(waitSec))
            return false;
    }

    return true;
}

bool GrblControl::waitForResponses(int waitSec)
{
    char tmp[BUF_SIZE + 1] = {0};
    int count = 0;
    int waitCount = waitSec * 10;// multiplier depends on sleep values below
    bool status = true;
    QString result;
    while (!result.contains(RESPONSE_OK) && !result.contains(RESPONSE_ERROR))
    {
        if (!port.haveData())
        {
            if (sendCount.size() == 0)
                return false;

            count++;
            SLEEP(100);
        }
        else
        {
            int n = port.getLine(tmp,BUF_SIZE);
            tmp[n] = 0;
            result.append(tmp);

            QString tmpTrim(tmp);
            int pos = tmpTrim.indexOf(port.getDetectedLineFeed());
            if (pos != -1)
                tmpTrim.remove(pos, port.getDetectedLineFeed().size());
            QString received(tmp);

            if (received.contains(RESPONSE_OK))
            {
                if (sendCount.isEmpty()) {
                    err(qPrintable(tr("Unexpected: list is empty (o)!")));
                }
                else
                {
                    CmdResponse cmdResp = sendCount.takeFirst();
                    diag(qPrintable(tr("GOT[%d]: '%s' for '%s'\n")), cmdResp.line,
                         tmpTrim.toLocal8Bit().constData(), cmdResp.cmd.trimmed().toLocal8Bit().constData());
                }
            }
            else if (received.contains(RESPONSE_ERROR))
            {
                QString orig(tr("Error?"));
                if (sendCount.isEmpty())
                    err(qPrintable(tr("Unexpected: list is empty (e)!")));
                else
                {
                    CmdResponse cmdResp = sendCount.takeFirst();
                    orig = cmdResp.cmd;
                    diag(qPrintable(tr("GOT[%d]: '%s' for '%s'\n")), cmdResp.line,
                         tmpTrim.toLocal8Bit().constData(), cmdResp.cmd.trimmed().toLocal8Bit().constData());
                }
            }
            else
            {
                diag(qPrintable(tr("GOT: '%s'\n")), tmpTrim.trimmed().toLocal8Bit().constData());
                //parseCoordinates(received);
            }

            int total = 0;
            foreach (CmdResponse cmdResp, sendCount)
            {
                total += cmdResp.count;
            }

            //printf("Total out (b): %d (%d)\n", total, sendCount.size());

            if (total >= (GRBL_RX_BUFFER_SIZE - 1))
            {
                //diag("DG Loop again\n");
                result.clear();
                continue;
            }
            else if (port.haveData())
            {
                result.clear();
                continue;
            }
            else
            {
                return true;
            }

            if (!received.contains(RESPONSE_OK) && !received.contains(RESPONSE_ERROR))
            {

            }
            count = 0;
        }

        SLEEP(100);

        if (count > waitCount)
        {
            // waited too long for a response, fail
            status = false;
            break;
        }
    }

    if (result.contains(RESPONSE_ERROR))
    {

    }

    return status;
}
