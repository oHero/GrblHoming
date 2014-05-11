#include <QObject>
#include "grblinterface.h"


GrblInterface::GrblInterface(RS232& rs)
  : port(rs), errorCount(0), doubleDollarFormat(false),
    incorrectMeasurementUnits(false), incorrectLcdDisplayUnits(false),
    maxZ(0), motionOccurred(false),
    sliderZCount(0),
    positionValid(false),
    numaxis(DEFAULT_AXIS_COUNT)
{
  // use base class's timer - use it to capture random text from the controller
  startTimer(1000);
}

bool GrblInterface::sendWithBlock(QString line, QString& result, QStringList& grblCmdErr, bool recordResponseOnFail, int waitSec, int currLine /* = 0 */)
{
    if (!port.isPortOpen())
    {
        QString msg = tr("Port not available yet")  ;
        err("%s", msg.toLocal8Bit().constData());
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }

    bool ctrlX = line.size() > 0 ? (line.at(0).toLatin1() == CTRL_X) : false;

    bool sentReqForLocation = false;
    bool sentReqForSettings = false;
    bool sentReqForParserState = false;

    if (GrblInterface::checkForGetPosStr(line))
    {
        sentReqForLocation = true;
        setLivenessState(true);
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

    // adds to UI list, but prepends a > indicating a sent command
    if (ctrlX)
    {
        emit addListOut("(CTRL-X)");
    }
    else if (!sentReqForLocation)// if requesting location, don't add that "noise" to the output view
    {
        emit addListOut(line);
    }

    if (line.size() == 0 || (!line.endsWith('\r') && !ctrlX))
        line.append('\r');

    char buf[BUF_SIZE + 1] = {0};
    if (line.length() >= BUF_SIZE)
    {
        QString msg = tr("Buffer size too small");
        err("%s", qPrintable(msg));
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }
    for (int i = 0; i < line.length(); i++)
        buf[i] = line.at(i).toLatin1();

    if (ctrlX)
        diag(qPrintable(tr("SENDING[%d]: 0x%02X (CTRL-X)\n")), currLine, buf[0]);
    else
        diag(qPrintable(tr("SENDING[%d]: %s\n")), currLine, buf);

    int waitSecActual = waitSec == -1 ? controlParams.waitTime : waitSec;

    if (ctrlX)
        sendCount.append(CmdResponse("(CTRL-X)", line.length(), currLine));
    else
        sendCount.append(CmdResponse(buf, line.length(), currLine));

    //diag("DG Buffer Add %d", sendCount.size());

    emit setQueuedCommands(sendCount.size(), true);

    waitForOk(result, waitSecActual, false, false, false, grblCmdErr);

    if (shutdownState.get())
        return false;

    if (!port.SendBuf(buf, line.length()))
    {
        QString msg = tr("Sending to port failed")  ;
        err("%s", qPrintable(msg));
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }
    else
    {
        if (!waitForOk(result, waitSecActual, sentReqForLocation, sentReqForParserState, false, grblCmdErr))
        {
            diag(qPrintable(tr("WAITFOROK FAILED\n")));
            if (shutdownState.get())
                return false;

            if (!recordResponseOnFail && !(resetState.get() || abortState.get()))
            {
                QString msg = tr("Wait for ok failed");
                emit addList(msg);
                emit sendMsg(msg);
            }

            return false;
        }
        else
        {
            if (sentReqForSettings)
            {
                QStringList list = result.split("$");
                for (int i = 0; i < list.size(); i++)
                {
                    QString item = list.at(i);
                    const QRegExp rx(REGEXP_SETTINGS_LINE);

                    if (rx.indexIn(item, 0) != -1 && rx.captureCount() == 3)
                    {
                        QStringList capList = rx.capturedTexts();
                        if (!capList.at(1).compare("13"))
                        {
                            if (!capList.at(2).compare("0"))
                            {
                                if (!controlParams.useMm)
                                    incorrectLcdDisplayUnits = true;
                            }
                            else
                            {
                                if (controlParams.useMm)
                                    incorrectLcdDisplayUnits = true;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void GrblInterface::initCmdSend()
{
    sendCount.clear();
    //if (sendCount.size() == 0)
    //{
    //    diag("DG Buffer 0 at start\n"));
    //}

    emit setQueuedCommands(sendCount.size(), true);
}

bool GrblInterface::waitForOk(QString& result, int waitSec, bool sentReqForLocation, bool sentReqForParserState, bool finalize, QStringList& grblCmdErr)
{
    int okcount = 0;

    //if (!port.bytesAvailable()) //more conservative code
    if (!finalize || !port.bytesAvailable())
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
    }

    char tmp[BUF_SIZE + 1] = {0};
    int count = 0;
    int waitCount = waitSec * 10;// multiplier depends on sleep values below
    bool status = true;
    result.clear();
    while (!result.contains(RESPONSE_OK) && !result.contains(RESPONSE_ERROR) && !resetState.get())
    {
        int n = port.PollComportLine(tmp, BUF_SIZE);
        if (n == 0)
        {
            if (sendCount.size() == 0)
                return false;

            count++;
            SLEEP(100);
        }
        else if (n < 0)
        {
            QString Mes(tr("Error reading data from COM port\n"))  ;
            err(qPrintable(Mes));

            if (sendCount.size() == 0)
                return false;
        }
        else
        {
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
                    //diag("DG Buffer %d", sendCount.size());

                    emit setQueuedCommands(sendCount.size(), true);
                }
                okcount++;
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
                    //diag("DG Buffer %d", sendCount.size());

                    emit setQueuedCommands(sendCount.size(), true);
                }
                errorCount++;
                QString result;
                QTextStream(&result) << received << " [for " << orig << "]";
                emit addList(result);
                grblCmdErr.append(result);
            }
            else
            {
                diag(qPrintable(tr("GOT: '%s'\n")), tmpTrim.trimmed().toLocal8Bit().constData());
                parseCoordinates(received);
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
            else if (port.bytesAvailable())
            {
                // comment out this block for more conservative approach
                if (!finalize && okcount > 0)
                {
                    //diag("DG Leave early\n");
                    return true;
                }

                result.clear();
                continue;
            }
            else
            {
                return true;
            }

            if (!received.contains(RESPONSE_OK) && !received.contains(RESPONSE_ERROR))
            {
                if (sentReqForParserState)
                {
                    const QRegExp rx("\\[([\\s\\w\\.\\d]+)\\]");

                    if (rx.indexIn(received, 0) != -1 && rx.captureCount() == 1)
                    {
                        QStringList list = rx.capturedTexts();
                        if (list.size() == 2)
                        {
                            QStringList items = list.at(1).split(" ");
                            if (items.contains("G20"))// inches
                            {
                                if (controlParams.useMm)
                                    incorrectMeasurementUnits = true;
                                else
                                    incorrectMeasurementUnits = false;
                            }
                            else if (items.contains("G21"))// millimeters
                            {
                                if (controlParams.useMm)
                                    incorrectMeasurementUnits = false;
                                else
                                    incorrectMeasurementUnits = true;
                            }
                            else
                            {
                                // not in list!
                                incorrectMeasurementUnits = true;
                            }
                        }
                    }
                }
                else
                {
                    parseCoordinates(received);
                }
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

    if (shutdownState.get())
    {
        return false;
    }

    if (status)
    {
        if (resetState.get())
        {
            QString msg(tr("Wait interrupted by user"));
            err("%s", qPrintable(msg));
            emit addList(msg);
        }
    }

    if (result.contains(RESPONSE_ERROR))
    {
        errorCount++;
        // skip over errors
        //status = false;
    }

    QStringList list = QString(result).split(port.getDetectedLineFeed());
    QStringList listToSend;
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i).length() > 0 && list.at(i) != RESPONSE_OK && !sentReqForLocation && !list.at(i).startsWith("MPos:["))
            listToSend.append(list.at(i));
    }

    sendStatusList(listToSend);

    if (resetState.get())
    {
        // we have been told by the user to stop.
        status = false;
    }

    return status;
}

void GrblInterface::parseCoordinates(const QString& received)
{
    int ms = parseCoordTimer.elapsed();
    if (ms < 500)
        return;

    parseCoordTimer.restart();

    bool good = false;
    int captureCount ;
    QString state;
    QString prepend;
    QString append;
    QString preamble = "([a-zA-Z]+),MPos:";
    if (!doubleDollarFormat)
    {
        prepend = "\\[";
        append = "\\]";
        preamble = "MPos:" ;
    }
    QString coordRegExp;
    QRegExp rxStateMPos;
    QRegExp rxWPos;
    /// 3 axis
    QString format("(-*\\d+\\.\\d+),(-*\\d+\\.\\d+)") ;
    int maxaxis = MAX_AXIS_COUNT, naxis ;
    for (naxis = DEFAULT_AXIS_COUNT; naxis <= maxaxis; naxis++) {
        if (!doubleDollarFormat)
            captureCount = naxis ;
        else
            captureCount = naxis + 1 ;
        //
        format += ",(-*\\d+\\.\\d+)" ;
        coordRegExp = prepend + format + append ;
        rxStateMPos = QRegExp(preamble + coordRegExp);
        rxWPos = QRegExp(QString("WPos:") + coordRegExp);
        good = rxStateMPos.indexIn(received, 0) != -1
               && rxStateMPos.captureCount() == captureCount
               && rxWPos.indexIn(received, 0) != -1
               && rxWPos.captureCount() == naxis
               ;
        // find ...
        if (good)
            break;
    }
    if (good) {  /// naxis contains number axis
        if (numaxis <= DEFAULT_AXIS_COUNT)
        {
            if (naxis > DEFAULT_AXIS_COUNT)
            {
                QString msg = tr("Incorrect - extra axis present in hardware but options set for only 3 axes. Please fix options.");
                emit addList(msg);
                emit sendMsg(msg);
            }
        }
        else
        {
            if (naxis <= DEFAULT_AXIS_COUNT)
            {
                QString msg = tr("Incorrect - extra axis not present in hardware but options set for > 3 axes. Please fix options.");
                emit addList(msg);
                emit sendMsg(msg);
            }
        }

        numaxis = naxis;
        QStringList list = rxStateMPos.capturedTexts();
        int index = 1;

        if (doubleDollarFormat)
            state = list.at(index++);

        machineCoord.x = list.at(index++).toFloat();
        machineCoord.y = list.at(index++).toFloat();
        machineCoord.z = list.at(index++).toFloat();
        if (numaxis == MAX_AXIS_COUNT)
            machineCoord.fourth = list.at(index++).toFloat();
        list = rxWPos.capturedTexts();
        workCoord.x = list.at(1).toFloat();
        workCoord.y = list.at(2).toFloat();
        workCoord.z = list.at(3).toFloat();
        if (numaxis == MAX_AXIS_COUNT)
            workCoord.fourth = list.at(4).toFloat();
        if (state != "Run")
            workCoord.stoppedZ = true;
        else
            workCoord.stoppedZ = false;

        workCoord.sliderZIndex = sliderZCount;
        if (numaxis == DEFAULT_AXIS_COUNT)
            diag(qPrintable(tr("Decoded: State:%s MPos: %f,%f,%f WPos: %f,%f,%f\n")),
                 qPrintable(state),
                 machineCoord.x, machineCoord.y, machineCoord.z,
                 workCoord.x, workCoord.y, workCoord.z
                 );
        else if (numaxis == MAX_AXIS_COUNT)
            diag(qPrintable(tr("Decoded: State:%s MPos: %f,%f,%f,%f WPos: %f,%f,%f,%f\n")),
                 qPrintable(state),
                 machineCoord.x, machineCoord.y, machineCoord.z, machineCoord.fourth,
                 workCoord.x, workCoord.y, workCoord.z, workCoord.fourth
                 );

        if (workCoord.z > maxZ)
            maxZ = workCoord.z;

        emit updateCoordinates(machineCoord, workCoord);
        emit setLivePoint(workCoord.x, workCoord.y, controlParams.useMm, positionValid);
        emit setLastState(state);

        lastState = state;
        return;
    }
    // TODO fix to print
    //if (!good /*&& received.indexOf("MPos:") != -1*/)
    //    err(qPrintable(tr("Error decoding position data! [%s]\n")), qPrintable(received));

    lastState = "";
}

bool GrblInterface::waitForAllResponses(QStringList& grblCmdErr)
{
    int limitCount = 5000;
    while (sendCount.size() > 0 && limitCount)
    {
        QString result;
        waitForOk(result, controlParams.waitTime, false, false, true, grblCmdErr);
        SLEEP(100);

        if (shutdownState.get())
            return false;

        if (abortState.get())
            break;

        limitCount--;
    }

    if (!limitCount)
    {
        err(qPrintable(tr("Gave up waiting for OK\n")));
    }
    return true;
}

// static
bool GrblInterface::checkForGetPosStr(QString& line)
{
    return (!line.compare(REQUEST_CURRENT_POS)
        || (line.startsWith(REQUEST_CURRENT_POS) && line.endsWith('\r') && line.length() == 2));
}

void GrblInterface::setLivenessState(bool valid)
{
    positionValid = valid;
    emit setVisualLivenessCurrPos(valid);
    emit setLcdState(valid);
}

void GrblInterface::sendStatusList(QStringList& listToSend)
{
    if (listToSend.size() > 1)
    {
        emit addListFull(listToSend);
    }
    else if (listToSend.size() == 1)
    {
        emit addList(listToSend.at(0));
    }
}

// called once a second to capture any random strings that come from the controller
void GrblInterface::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (port.isPortOpen())
    {
        char tmp[BUF_SIZE + 1] = {0};
        QString result;

        for (int i = 0; i < 10 && !shutdownState.get() && !resetState.get(); i++)
        {
            int n = port.PollComport(tmp, BUF_SIZE);
            if (n == 0)
                break;

            tmp[n] = 0;
            result.append(tmp);
            diag(qPrintable(tr("GOT-TE:%s\n")), tmp);
        }

        if (shutdownState.get())
        {
            return;
        }

        QStringList list = QString(result).split(port.getDetectedLineFeed());
        QStringList listToSend;
        for (int i = 0; i < list.size(); i++)
        {
            if (list.at(i).length() > 0 && (list.at(i) != "ok" || (list.at(i) == "ok" && abortState.get())))
                listToSend.append(list.at(i));
        }

        sendStatusList(listToSend);
    }
}

bool GrblInterface::sendToPort(const char *buf)
{
    if (!port.SendBuf(buf, 1))
    {
        QString msg = tr("Sending to port failed");
        err("%s", qPrintable(msg));
        emit addList(msg);
        emit sendMsg(msg);
        return false;
    }
    return true;
}

void GrblInterface::resetPort()
{
    port.Reset();
}

int GrblInterface::PollComportLine(char *buf, int size)
{
    return port.PollComportLine(buf, size);
}

QString GrblInterface::getDetectedLineFeed()
{
    return port.getDetectedLineFeed();
}

void GrblInterface::setParams(ControlParams controlParamsIn)
{
    controlParams = controlParamsIn;
    port.setCharSendDelayMs(controlParams.charSendDelayMs);
}

void GrblInterface::openPort(QString commPortStr, QString baudRate)
{
    numaxis = controlParams.useFourAxis ? MAX_AXIS_COUNT : DEFAULT_AXIS_COUNT;

    currComPort = commPortStr;

    port.setCharSendDelayMs(controlParams.charSendDelayMs);

    if (port.OpenComport(commPortStr, baudRate))
    {
        emit portIsOpen(true);
    }
    else
    {
        emit portIsClosed(false);
        QString msg = tr("Can't open COM port ") + commPortStr;
        sendMsg(msg);
        addList(msg);
        warn("%s", qPrintable(msg));

        addList(tr("-Is hardware connected to USB?") );
        addList(tr("-Is correct port chosen?") );
        addList(tr("-Does current user have sufficient permissions?") );
#if defined(Q_OS_LINUX)
        addList("-Is current user in sudoers group?");
#endif
        //QMessageBox(QMessageBox::Critical,"Error","Could not open port.",QMessageBox::Ok).exec();
    }
}

void GrblInterface::closePort(bool reopen)
{
    port.CloseComport();
    emit portIsClosed(reopen);
}


bool GrblInterface::isPortOpen()
{
    return port.isPortOpen();
}

void GrblInterface::clearToHome()
{
    maxZ = 0;
    motionOccurred = false;
}

void GrblInterface::setDoubleDollarFormat(bool isDD)
{
    doubleDollarFormat = isDD;
}

bool GrblInterface::isDoubleDollarFormat()
{
    return doubleDollarFormat;
}

void GrblInterface::clearErrorCount()
{
    errorCount = 0;
}

void GrblInterface::incErrorCount()
{
    errorCount++;
}

int GrblInterface::getErrorCount()
{
    return errorCount;
}

bool GrblInterface::isIncorrectMeasurementUnits()
{
    return incorrectMeasurementUnits;
}

void GrblInterface::clearIncorrectMeasurementUnits()
{
    incorrectMeasurementUnits = false;
}

bool GrblInterface::isIncorrectLcdDisplayUnits()
{
    return incorrectLcdDisplayUnits;
}

void GrblInterface::clearIncorrectLcdDisplayUnits()
{
    incorrectLcdDisplayUnits = false;
}

bool GrblInterface::isMotionOccurred()
{
    return motionOccurred;
}

void GrblInterface::clearMotionOccurred()
{
    motionOccurred = false;
}

double GrblInterface::getMaxZ()
{
    return maxZ;
}

void GrblInterface::reduceMaxZ(double reduceAmount)
{
    maxZ -= reduceAmount;
}

void GrblInterface::setSliderZCount(int count)
{
    sliderZCount = count;
}

int GrblInterface::getNumAxis()
{
    return numaxis;
}

void GrblInterface::setNumAxis(int count)
{
    numaxis = count;
}

void GrblInterface::setAbortState(bool state)
{
    abortState.set(state);
}

bool GrblInterface::getAbortState()
{
    return abortState.get();
}

void GrblInterface::setResetState(bool state)
{
    resetState.set(state);
}

bool GrblInterface::getResetState()
{
    return resetState.get();
}

void GrblInterface::setShutdownState(bool state)
{
    shutdownState.set(state);
}

bool GrblInterface::getShutdownState()
{
    return shutdownState.get();
}

QString GrblInterface::getLastState()
{
    return lastState;
}

Coord3D GrblInterface::getMachineCoord()
{
    return machineCoord;
}

Coord3D GrblInterface::getWorkCoord()
{
    return workCoord;
}

void GrblInterface::restartParseCoordTimer()
{
    parseCoordTimer.restart();
}
