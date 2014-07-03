/****************************************************************
 * gcode.cpp
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include "gcode.h"

#include <QObject>

GCode::GCode(GrblInterface& grblIfc)
    : grbl(grblIfc)
{
    // for position polling
    pollPosTimer.start();
}

// Abort means stop file send after the end of this line
void GCode::setAbort()
{
    // cross-thread operation, only set one atomic variable in this method (bool in this case) or add critsec
    grbl.setAbortState(true);
}

// Reset means immediately stop waiting for a response
void GCode::setReset()
{
    // cross-thread operation, only set one atomic variable in this method (bool in this case) or add critsec
    grbl.setResetState(true);
}

// Shutdown means app is shutting down - we give thread about .3 sec to exit what it is doing
void GCode::setShutdown()
{
    // cross-thread operation, only set one atomic variable in this method (bool in this case) or add critsec
    grbl.setShutdownState(true);
}

// Slot for interrupting current operation or doing a clean reset of grbl without changing position values
void GCode::sendGrblReset()
{
    grbl.clearToHome();

    QString x(CTRL_X);
    sendGcodeLocal(true, x, true, SHORT_WAIT_SEC);
}

void GCode::sendGrblUnlock()
{
    sendGcodeLocal(true, SET_UNLOCK_STATE_V08c);
}

// Slot for gcode-based 'zero out the current position values without motion'
void GCode::grblSetHome()
{
    grbl.clearToHome();

    if (grbl.getNumAxis() == MAX_AXIS_COUNT)
        gotoXYZFourth(QString("G92 x0 y0 z0 ").append(QString(controlParams.fourthAxisType)).append("0"));
	else
        gotoXYZFourth("G92 x0 y0 z0");
}

void GCode::goToHome()
{
    if (!grbl.isMotionOccurred())
        return;

    double maxZOver = grbl.getMaxZ();

    if (grbl.isDoubleDollarFormat())
    {
        maxZOver += (controlParams.useMm ? PRE_HOME_Z_ADJ_MM : (PRE_HOME_Z_ADJ_MM / MM_IN_AN_INCH));
    }
    else
    {
        // all reporting is in mm
        maxZOver += PRE_HOME_Z_ADJ_MM;
    }

    QString zpos = QString::number(maxZOver);

    gotoXYZFourth(QString("G0 z").append(zpos));

    if (grbl.getNumAxis() == MAX_AXIS_COUNT)
        gotoXYZFourth(QString("G1 x0 y0 z0 ").append(QString(controlParams.fourthAxisType)).append("0"));
	else
        gotoXYZFourth("G1 x0 y0 z0");

    grbl.reduceMaxZ(maxZOver);

    grbl.clearMotionOccurred();
}

// Slot called from other threads (i.e. main window, grbl dialog, etc.)
void GCode::sendGcode(QString line)
{
    bool checkMeasurementUnits = false;

    // empty line means we have just opened the com port
    if (line.length() == 0)
    {
        grbl.setResetState(false);

        QString result;
        if (!waitForStartupBanner(result, SHORT_WAIT_SEC, false))
        {
            if (grbl.getShutdownState() || grbl.getResetState())
                return;
            // it is possible that we are already connected and missed the
            // signon banner. Force a reset (is this ok?) to get the banner

            emit addListOut("(CTRL-X)");

            char buf[2] = {0};

            buf[0] = CTRL_X;

			diag(qPrintable(tr("SENDING: 0x%02X (CTRL-X) to check presence of Grbl\n")), buf[0])  ;

            grbl.sendToPort(buf);

            if (!waitForStartupBanner(result, SHORT_WAIT_SEC, true))
                return;
        }

        checkMeasurementUnits = true;
    }
    else
    {
        pollPosWaitForIdle(false);
        // normal send of actual commands
        sendGcodeLocal(false, line, false);
    }

    pollPosWaitForIdle(checkMeasurementUnits);
}

// keep polling our position and state until we are done running
void GCode::pollPosWaitForIdle(bool checkMeasurementUnits)
{
    if (controlParams.usePositionRequest
            && (controlParams.positionRequestType == PREQ_ALWAYS_NO_IDLE_CHK
                    || controlParams.positionRequestType == PREQ_ALWAYS
                    || checkMeasurementUnits))
    {
        bool immediateQuit = false;
        for (int i = 0; i < 10000; i++)
        {
            GCode::PosReqStatus ret = positionUpdate();
            if (ret == POS_REQ_RESULT_ERROR || ret == POS_REQ_RESULT_UNAVAILABLE)
            {
                immediateQuit = true;
                break;
            }
            else if (ret == POS_REQ_RESULT_TIMER_SKIP)
            {
                SLEEP(250);
                continue;
            }

            if (grbl.isDoubleDollarFormat())
            {
                if (grbl.getLastState().compare("Run") != 0)
                    break;
            }
            else
            {
                if (machineCoordLastIdlePos == grbl.getMachineCoord()
                    && workCoordLastIdlePos == grbl.getWorkCoord())
                {
                    break;
                }

                machineCoordLastIdlePos = grbl.getMachineCoord();
                workCoordLastIdlePos = grbl.getWorkCoord();
            }

            if (grbl.getShutdownState())
                return;
        }

        if (immediateQuit)
            return;

        if (checkMeasurementUnits)
        {
            if (grbl.isDoubleDollarFormat())
                checkAndSetCorrectMeasurementUnits();
            else
                setOldFormatMeasurementUnitControl();
        }
    }
    else
    {
        grbl.setLivenessState(false);
    }
}

// Slot called from other thread that returns whatever text comes back from the controller
void GCode::sendGcodeAndGetResult(int id, QString line)
{
    QString result;

    emit sendMsg("");
    grbl.setResetState(false);
    if (!grbl.sendCmd(line, result, grblCmdErrors, false, SHORT_WAIT_SEC))
        result.clear();

    emit gcodeResult(id, result);
}

// To be called only from this class, not from other threads. Use above two methods for that.
// Wraps sendGcodeInternal() to allow proper handling of failure cases, etc.
bool GCode::sendGcodeLocal(bool waitForResp, QString line, bool recordResponseOnFail /* = false */, int waitSec /* = -1 */, int currLine /* = 0 */)
{
    QString result;
    sendMsg("");
    grbl.setResetState(false);

    bool ret = grbl.sendCmd(line, result, grblCmdErrors, recordResponseOnFail, waitSec, currLine);
    if (grbl.getShutdownState())
        return false;

    if (!ret && (!recordResponseOnFail || grbl.getResetState()))
    {
        if (!grbl.getResetState())
            emit stopSending();

        if (!ret && grbl.getResetState())
        {
            grbl.setResetState(false);
            grbl.resetPort();
        }
    }
    else
    {
        if (waitForResp)
        {
            if (!grbl.waitForAllResponses(grblCmdErrors))
                return false;
        }

        if (checkGrbl(result))
        {
            emit enableGrblDialogButton();
        }

        if (!waitForResp)
        {
            grbl.setWait();
        }
    }
    grbl.setResetState(false);
    return ret;
}

bool GCode::checkGrbl(const QString& result)
{
    if (result.contains("Grbl"))
    {
        QRegExp rx("Grbl (\\d+)\\.(\\d+)(\\w*)");
        if (rx.indexIn(result) != -1 && rx.captureCount() > 0)
        {
            grbl.setDoubleDollarFormat(false);

            QStringList list = rx.capturedTexts();
            if (list.size() >= 3)
            {
                int majorVer = list.at(1).toInt();
                int minorVer = list.at(2).toInt();
                char letter = ' ';
                if (list.size() == 4 && list.at(3).size() > 0)
                {
                    letter = list.at(3).toLatin1().at(0);
                }

                if (majorVer > 0 || (minorVer > 8 && minorVer < 51) || letter > 'a')
                {
                    grbl.setDoubleDollarFormat(true);
                }

                diag(qPrintable(tr("Got Grbl Version (Parsed:) %d.%d%c ($$=%d)\n")),
                            majorVer, minorVer, letter, grbl.isDoubleDollarFormat());
            }

            if (!grbl.isDoubleDollarFormat())
                setUnitsTypeDisplay(true);
        }
        return true;
    }
    return false;
}

bool GCode::waitForStartupBanner(QString& result, int waitSec, bool failOnNoFound)
{
    char tmp[BUF_SIZE + 1] = {0};
    int count = 0;
    int waitCount = waitSec * 10;// multiplier depends on sleep values below
    bool status = true;
    result.clear();
    while (!grbl.getResetState())
    {
        int n = grbl.PollComportLine(tmp, BUF_SIZE);
        if (n == 0)
        {
            count++;
            SLEEP(100);
        }
        else if (n < 0)
        {
            err(qPrintable(tr("Error reading data from COM port\n")) );
        }
        else
        {
            tmp[n] = 0;
            result.append(tmp);

            QString tmpTrim(tmp);
            int pos = tmpTrim.indexOf(grbl.getDetectedLineFeed());
            if (pos != -1)
                tmpTrim.remove(pos, grbl.getDetectedLineFeed().size());
            diag(qPrintable(tr("GOT:%s\n")), tmpTrim.toLocal8Bit().constData());

            if (tmpTrim.length() > 0)
            {
                if (!checkGrbl(tmpTrim))
                {
                    if (failOnNoFound)
                    {
                        QString msg(tr("Expecting Grbl version string. Unable to parse response."));
                        emit addList(msg);
                        emit sendMsg(msg);

                        grbl.closePort(false);
                    }
                    status = false;
                }
                else
                {
                    emit enableGrblDialogButton();
                }
                break;
            }
        }

        SLEEP(100);

        if (count > waitCount)
        {
            if (failOnNoFound)
            {
                // waited too long for a response, fail

                QString msg(tr("No data from COM port after connect. Expecting Grbl version string."));
                emit addList(msg);
                emit sendMsg(msg);

                grbl.closePort(false);
            }

            status = false;
            break;
        }
    }

    if (grbl.getShutdownState())
    {
        return false;
    }

    if (status)
    {
        if (grbl.getResetState())
        {
            QString msg(tr("Wait interrupted by user (startup)"));
            err("%s", qPrintable(msg));
            emit addList(msg);
        }
    }

    if (result.contains(RESPONSE_ERROR))
    {
        grbl.incErrorCount();
        // skip over errors
        //status = false;
    }

    QStringList list = QString(result).split(grbl.getDetectedLineFeed());
    QStringList listToSend;
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i).length() > 0 && list.at(i) != RESPONSE_OK)
            listToSend.append(list.at(i));
    }

    grbl.sendStatusList(listToSend);

    if (grbl.getResetState())
    {
        // we have been told by the user to stop.
        status = false;
    }

    return status;
}

void GCode::sendFile(QString path)
{
    addList(QString(tr("Sending file '%1'")).arg(path));

    // send something to be sure the controller is ready
    //sendGcodeLocal("", true, SHORT_WAIT_SEC);

    setProgress(0);
    emit setQueuedCommands(0, false);
    grblCmdErrors.clear();
    grblFilteredCmds.clear();
    grbl.clearErrorCount();
    grbl.setAbortState(false);
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        float totalLineCount = 0;
        QTextStream code(&file);
        while ((code.atEnd() == false))
        {
            totalLineCount++;
            code.readLine();
        }
        if (totalLineCount == 0)
            totalLineCount = 1;

        code.seek(0);

        grbl.initCmdSend();

        emit resetTimer(true);

        grbl.restartParseCoordTimer();

        int currLine = 0;
        bool xyRateSet = false;

        do
        {
            QString strline = code.readLine();

            emit setVisCurrLine(currLine + 1);

            if (controlParams.filterFileCommands)
            {
                trimToEnd(strline, '(');
                trimToEnd(strline, ';');
                trimToEnd(strline, '%');
            }

            strline = strline.trimmed();

            if (strline.size() == 0)
            {}//ignore comments
            else
            {
                if (controlParams.filterFileCommands)
                {
                    strline = strline.toUpper();
                    strline.replace(QRegExp("([A-Z])"), " \\1");
                    strline = removeUnsupportedCommands(strline);
                }

                if (strline.size() != 0)
                {
                    if (controlParams.reducePrecision)
                    {
                        strline = reducePrecision(strline);
                    }

                    QString rateLimitMsg;
                    QStringList outputList;
                    if (controlParams.zRateLimit)
                    {
                        outputList = doZRateLimit(strline, rateLimitMsg, xyRateSet);
                    }
                    else
                    {
                        outputList.append(strline);
                    }

                    bool ret = false;
                    if (outputList.size() == 1)
                    {
                        ret = sendGcodeLocal(false, outputList.at(0), false, -1, currLine + 1);
                    }
                    else
                    {
                        foreach (QString outputLine, outputList)
                        {
                            ret = sendGcodeLocal(false, outputLine, false, -1, currLine + 1);

                            if (!ret)
                                break;
                        }
                    }

                    if (rateLimitMsg.size() > 0)
                        addList(rateLimitMsg);

                    if (!ret)
                    {
                        grbl.setAbortState(true);
                        break;
                    }
                }
            }

            float percentComplete = (currLine * 100.0) / totalLineCount;
            setProgress((int)percentComplete);

            positionUpdate();
            currLine++;
        } while ((code.atEnd() == false) && (!grbl.getAbortState()));
        file.close();

        if (!grbl.waitForAllResponses(grblCmdErrors))
            return;

        positionUpdate();

        emit resetTimer(false);

        if (grbl.getShutdownState())
        {
            return;
        }

        QString msg;
        if (!grbl.getAbortState())
        {
            setProgress(100);
            if (grbl.getErrorCount() > 0)
            {
                msg = QString(tr("Code sent successfully with %1 error(s):")).arg(QString::number(grbl.getErrorCount()));
                emit sendMsg(msg);
                emit addList(msg);

                foreach(QString errItem, grblCmdErrors)
                {
                    emit sendMsg(errItem);
                }
                emit addListFull(grblCmdErrors);
            }
            else
            {
                msg = tr("Code sent successfully with no errors.");
                emit sendMsg(msg);
                emit addList(msg);
            }

            if (grblFilteredCmds.size() > 0)
            {
                msg = QString(tr("Filtered %1 commands:")).arg(QString::number(grblFilteredCmds.size()));
                emit sendMsg(msg);
                emit addList(msg);

                foreach(QString errItem, grblFilteredCmds)
                {
                    emit sendMsg(errItem);
                }
                emit addListFull(grblFilteredCmds);
            }
        }
        else
        {
            msg = tr("Process interrupted.");
            emit sendMsg(msg);
            emit addList(msg);
        }
    }

    pollPosWaitForIdle(true);

    if (!grbl.getResetState())
    {
        emit stopSending();
    }

    emit setQueuedCommands(0, false);
}

void GCode::trimToEnd(QString& strline, QChar ch)
{
    int pos = strline.indexOf(ch);
    if (pos != -1)
        strline = strline.left(pos);
}

QString GCode::removeUnsupportedCommands(QString line)
{
    QStringList components = line.split(" ", QString::SkipEmptyParts);
    QString tmp;
    QString s;
    QString following;
    bool toEndOfLine = false;
    foreach (s, components)
    {
        if (toEndOfLine)
        {
            QString msg(QString(tr("Removed unsupported command '%1' part of '%2'")).arg(s).arg(following));
            warn("%s", qPrintable(msg));
            grblFilteredCmds.append(msg);
            emit addList(msg);
            continue;
        }

        if (s.at(0) == 'G')
        {
            float value = s.mid(1,-1).toFloat();
            if (isGCommandValid(value, toEndOfLine))
                tmp.append(s).append(" ");
            else
            {
                if (toEndOfLine)
                    following = s;
                QString msg(QString(tr("Removed unsupported G command '%1'")).arg(s));
                warn("%s", qPrintable(msg));
                grblFilteredCmds.append(msg);
                emit addList(msg);
            }
        }
        else if (s.at(0) == 'M')
        {
            float value = s.mid(1,-1).toFloat();
            if (isMCommandValid(value))
                tmp.append(s).append(" ");
            else
            {
                QString msg(QString(tr("Removed unsupported M command '%1'")).arg(s));
                warn("%s", qPrintable(msg));
                grblFilteredCmds.append(msg);
                emit addList(msg);
            }
        }
        else if (s.at(0) == 'N')
        {
            // skip line numbers
        }
        else if (s.at(0) == 'X' || s.at(0) == 'Y' || s.at(0) == 'Z'
                 || s.at(0) == 'A' || s.at(0) == 'B' || s.at(0) == 'C'
                 || s.at(0) == 'I' || s.at(0) == 'J' || s.at(0) == 'K'
                 || s.at(0) == 'F' || s.at(0) == 'L' || s.at(0) == 'S')
        {
            tmp.append(s).append(" ");
        }
        else
        {
            QString msg(QString(tr("Removed unsupported command '%1'")).arg(s));
            warn("%s", qPrintable(msg));
            grblFilteredCmds.append(msg);
            emit addList(msg);
        }
    }

    return tmp.trimmed();
}

QString GCode::reducePrecision(QString line)
{
    // first remove all spaces to determine what are line length is
    QStringList components = line.split(" ", QString::SkipEmptyParts);
    QString result;
    foreach(QString token, components)
    {
        result.append(token);
    }

    if (result.length() == 0)
        return line;// nothing to do

    if (!result.at(0).isLetter())
        return line;// leave as-is if not a command

    // find first comment and eliminate
    int pos = result.indexOf('(');
    if (pos >= 0)
        result = result.left(pos);

    int charsToRemove = result.length() - (controlParams.grblLineBufferLen - 1);// subtract 1 to account for linefeed sent with command later

    if (charsToRemove > 0)
    {
        // ok need to do something with precision
        // split apart based on letter
        pos = 0;
        components.clear();
        int i;
        for (i = 1; i < result.length(); i++)
        {
            if (result.at(i).isLetter())
            {
                components.append(result.mid(pos, i - pos));
                pos = i;
            }
        }

        if (pos == 0)
        {
            // we get here if only one command
            components.append(result);
        }
        else
        {
            // add last item
            components.append(result.mid(pos, i));
        }

        QList<DecimalFilter> items;
        foreach (QString tmp, components)
        {
            items.append(DecimalFilter(tmp));
        }

        int totalDecCount = 0;
        int eligibleArgumentCount = 0;
        int largestDecCount = 0;
        for (int j = 0; j < items.size(); j++)
        {
            DecimalFilter& item = items[j];
            pos = item.token.indexOf('.');
            if ((item.token.at(1).isDigit() || item.token.at(1) == '-' || item.token.at(1) == '.') && pos >= 0)
            {
                // candidate to modify
                // count number of decimal places
                int decPlaceCount = 0;
                for (i = pos + 1; i < item.token.length(); i++, decPlaceCount++)
                {
                    if (!item.token.at(i).isDigit())
                        break;
                }

                // skip commands that have a single decimal place
                if (decPlaceCount > 1)
                {
                    item.decimals = decPlaceCount;
                    totalDecCount += decPlaceCount - 1;// leave at least the last decimal place
                    eligibleArgumentCount++;
                    if (decPlaceCount > largestDecCount)
                        largestDecCount = decPlaceCount;
                }
            }
        }

        bool failRemoveSufficientDecimals = false;
        if (totalDecCount < charsToRemove)
        {
            // remove as many as possible, then grbl will truncate
            charsToRemove = totalDecCount;
            failRemoveSufficientDecimals = true;
        }

        if (eligibleArgumentCount > 0)
        {
            for (int k = largestDecCount; k > 1 && charsToRemove > 0; k--)
            {
                for (int j = 0; j < items.size() && charsToRemove > 0; j++)
                {
                    DecimalFilter& item = items[j];
                    if (item.decimals == k)
                    {
                        item.token.chop(1);
                        item.decimals--;
                        charsToRemove--;
                    }
                }
            }

            //chk.clear();
            //chk.append("CORRECTED:");
            result.clear();
            foreach (DecimalFilter item, items)
            {
                result.append(item.token);

                //chk.append("[");
                //chk.append(item.token);
                //chk.append("]");
            }
            //diag(chk.toLocal8Bit().constData());

            err(qPrintable(tr("Unable to remove enough decimal places for command (will be truncated): %s")), line.toLocal8Bit().constData());

            QString msg;
            if (failRemoveSufficientDecimals)
                msg = QString(tr("Error, insufficent reduction '%1'")).arg(result);
            else
                msg = QString(tr("Precision reduced '%1'")).arg(result);

            emit addList(msg);
            emit sendMsg(msg);
        }
    }

    return result;
}

bool GCode::isGCommandValid(float value, bool& toEndOfLine)
{
    toEndOfLine = false;
    // supported values obtained from https://github.com/grbl/grbl/wiki
    const static float supported[] =
    {
        0,    1,    2,    3,    4,
        10,    17,    18,    19,    20,    21,    28,    28.1,    30,    30.1,
        53,    54,    55,    56,    57,    58,    59,
        80,    90,    91,    92,    92.1,    93,    94
    };

    int len = sizeof(supported) / sizeof(float);
    for (int i = 0; i < len; i++)
    {
        if (value == supported[i])
            return true;
    }

    if (value == 43 || value == 44)
    {
        toEndOfLine = true;
    }
    return false;
}

bool GCode::isMCommandValid(float value)
{
    // supported values obtained from https://github.com/grbl/grbl/wiki

    // NOTE: M2 and M30 are supported but they cause occasional grbl lockups
    // and thus have been removed from the supported list. No harm is caused
    // by their removal.
    const static float supported[] =
    {
        0,    3,    4,    5,    8,    9
    };

    int len = sizeof(supported) / sizeof(float);
    for (int i = 0; i < len; i++)
    {
        if (value == supported[i])
            return true;
    }
    return false;
}

QStringList GCode::doZRateLimit(QString inputLine, QString& msg, bool& xyRateSet)
{
    // i.e.
    //G00 Z1 => G01 Z1 F100
    //G01 Z1 F260 => G01 Z1 F100
    //G01 Z1 F30 => G01 Z1 F30
    //G01 X1 Y1 Z1 F200 -> G01 X1 Y1 & G01 Z1 F100
    QStringList list;
    QString line = inputLine.toUpper();

    if (line.count("Z") > 0)
    {
        QStringList components = line.split(" ", QString::SkipEmptyParts);
        QString s;
        bool foundFeed = false;
        bool didLimit = false;

        // We need to build one or two command strings depending on input.
        QString x, y, g, f;

        // First get all component parts
        bool inLimit = false;
        foreach (s, components)
        {
            if (s.at(0) == 'G')
            {
                g = s;
            }
            else if (s.at(0) == 'F')
            {
                f = s;

                double value = s.mid(1,-1).toDouble();
                if (value > controlParams.zRateLimitAmount)
                    inLimit = true;
            }
            else if (s.at(0) == 'X')
            {
                x = s;
            }
            else if (s.at(0) == 'Y')
            {
                y = s;
            }
        }

        // Determine whether we want to have one or two command lins
        // 1 string: Have !G0 and F but not in limit
        // 1 string: Have Z only
        // 2 strings: All other conditions
        QString line1;
        QString line2;
        if ((g != "G0" && f.size() > 0 && !inLimit)
                || (x.size() == 0 && y.size() == 0))
        {
            foreach (s, components)
            {
                if (s.at(0) == 'G')
                {
                    int value = s.mid(1,-1).toInt();
                    if (value == 0)
                        line1.append("G1");
                    else
                        line1.append(s);
                }
                else if (s.at(0) == 'F')
                {
                    double value = s.mid(1,-1).toDouble();
                    if (value > controlParams.zRateLimitAmount)
                    {
                        line1.append("F").append(QString::number(controlParams.zRateLimitAmount));
                        didLimit = true;
                    }
                    else
                        line1.append(s);

                    foundFeed = true;
                }
                else
                {
                    line1.append(s);
                }
                line1.append(" ");
            }
        }
        else
        {
            // two lines
            foreach (s, components)
            {
                if (s.at(0) == 'G')
                {
                    int value = s.mid(1,-1).toInt();
                    if (value != 1)
                        line1.append("G1").append(" ");
                    else
                        line1.append(s).append(" ");

                    line2.append(s).append(" ");
                }
                else if (s.at(0) == 'F')
                {
                    double value = s.mid(1,-1).toDouble();
                    if (value > controlParams.zRateLimitAmount)
                    {
                        line1.append("F").append(QString::number(controlParams.zRateLimitAmount));
                        didLimit = true;
                    }
                    else
                        line1.append(s).append(" ");

                    line2.append(s).append(" ");

                    foundFeed = true;
                }
                else if (s.at(0) == 'Z')
                {
                    line1.append(s).append(" ");
                }
                else
                {
                    line2.append(s).append(" ");
                }
            }
        }

        if (!foundFeed)
        {
            line1.append("F").append(QString::number(controlParams.zRateLimitAmount));
            didLimit = true;
        }

        line1 = line1.trimmed();
        line2 = line2.trimmed();

        if (didLimit)
        {
            if (line2.size() == 0)
            {
                msg = QString(tr("Z-Rate Limit: [%1]=>[%2]")).arg(inputLine).arg(line1);
                xyRateSet = true;
            }
            else
            {
                msg = QString(tr("Z-Rate Limit: [%1]=>[%2,%3]")).arg(inputLine).arg(line1).arg(line2);
                line2.append(QString(" F").append(QString::number(controlParams.xyRateAmount)));
            }
        }

        list.append(line1);
        if (line2.size() > 0)
            list.append(line2);
        return list;
    }
    else if (xyRateSet)
    {
        QStringList components = line.split(" ", QString::SkipEmptyParts);
        QString s;

        bool addRateG = false;
        bool addRateXY = false;
        bool gotF = false;
        foreach (s, components)
        {
            if (s.at(0) == 'G')
            {
                int value = s.mid(1,-1).toInt();
                if (value != 0)
                {
                    addRateG = true;
                }
            }
            else if (s.at(0) == 'F')
            {
                gotF = true;
            }
            else
            if (s.at(0) == 'X' || s.at(0) == 'Y' || s.at(0) == 'A' || s.at(0) == 'B' || s.at(0) == 'C')
            {
                addRateXY = true;
            }
        }

        if (addRateG && addRateXY)
        {
            if (!gotF)
            {
                QString line = inputLine;
                line.append(QString(" F").append(QString::number(controlParams.xyRateAmount)));
                msg = QString(tr("XY-Rate Limit FIX: [%1]=>[%2]")).arg(inputLine).arg(line);
                list.append(line);
            }
            else
            {
                list.append(inputLine);
            }

            xyRateSet = false;

            return list;
        }
    }

    list.append(inputLine);
    return list;

}

void GCode::gotoXYZFourth(QString line)
{
    bool queryPos = GrblInterface::checkForGetPosStr(line);
    if (!queryPos && controlParams.usePositionRequest
            && controlParams.positionRequestType == PREQ_ALWAYS)
        pollPosWaitForIdle(false);

    if (sendGcodeLocal(false, line))
    {
        if (!queryPos)
            pollPosWaitForIdle(false);
    }
    else
    {
        QString msg(QString(tr("Bad command: %1")).arg(line));
        warn("%s", qPrintable(msg));
        emit addList(msg);
    }

    emit setCommandText("");
}


QString GCode::getMoveAmountFromString(QString prefix, QString item)
{
    int index = item.indexOf(prefix);
    if (index != -1)
        return item.mid(index + 1);

    return "";
}

void GCode::axisAdj(char axis, float coord, bool inv, bool absoluteAfterAxisAdj, int sZC)
{
    if (inv)
    {
        coord = (-coord);
    }

    QString cmd = QString("G01 ").append(axis)
            .append(QString::number(coord));

    if (axis == 'Z')
    {
        cmd.append(" F").append(QString::number(controlParams.zJogRate));
    }

    SendJog(cmd, absoluteAfterAxisAdj);

    if (axis == 'Z')
        grbl.setSliderZCount(sZC);

    emit adjustedAxis();
}

bool GCode::SendJog(QString cmd, bool absoluteAfterAxisAdj)
{
    //pollPosWaitForIdle(false);

    // G91 = distance relative to previous
    bool ret = sendGcodeLocal(false, "G91\r");

    bool result = ret && sendGcodeLocal(false, cmd.append("\r"));

    if (result)
    {
        //pollPosWaitForIdle(false);
    }

    if (absoluteAfterAxisAdj)
        sendGcodeLocal(false, "G90\r");

    return result;
}

// settings change calls here
void GCode::setResponseWait(ControlParams controlParamsIn)
{
    bool oldMm = controlParams.useMm;

    controlParams = controlParamsIn;

    grbl.setParams(controlParamsIn);

    controlParams.useMm = oldMm;

    if ((oldMm != controlParamsIn.useMm) && grbl.isPortOpen() && grbl.isDoubleDollarFormat())
    {
        if (controlParamsIn.useMm)
        {
            setConfigureMmMode(true);
        }
        else
        {
            setConfigureInchesMode(true);
        }
    }

    controlParams.useMm = controlParamsIn.useMm;
    grbl.setNumAxis(controlParams.useFourAxis ? MAX_AXIS_COUNT : DEFAULT_AXIS_COUNT);

    setUnitsTypeDisplay(controlParams.useMm);
}

// 0.8c and above only!
void GCode::checkAndSetCorrectMeasurementUnits()
{
    sendGcodeLocal(true, REQUEST_PARSER_STATE_V08c, false);

    if (grbl.isIncorrectMeasurementUnits())
    {
        if (controlParams.useMm)
        {
            emit addList(tr("Options specify use mm but Grbl parser set for inches. Fixing."));
            setConfigureMmMode(true);
        }
        else
        {
            emit addList(tr("Options specify use inches but Grbl parser set for mm. Fixing.") );
            setConfigureInchesMode(true);
        }
        grbl.clearIncorrectMeasurementUnits();// hope this is ok to do here
        positionUpdate(true);
    }
    else
    {
        sendGcodeLocal(true, SETTINGS_COMMAND_V08c);

        if (grbl.isIncorrectLcdDisplayUnits())
        {
            if (controlParams.useMm)
            {
                emit addList(tr("Options specify use mm but Grbl reporting set for inches. Fixing."));
                setConfigureMmMode(false);
            }
            else
            {
                emit addList(tr("Options specify use inches but Grbl reporting set for mm. Fixing."));
                setConfigureInchesMode(false);
            }
        }
        grbl.clearIncorrectLcdDisplayUnits();
    }
}

void GCode::setOldFormatMeasurementUnitControl()
{
    if (controlParams.useMm)
        sendGcodeLocal(false, "G21");
    else
        sendGcodeLocal(false, "G20");
}

void GCode::setConfigureMmMode(bool setGrblUnits)
{
    sendGcodeLocal(false, "$13=0");
    if (setGrblUnits)
        sendGcodeLocal(false, "G21");
    positionUpdate(true);
}

void GCode::setConfigureInchesMode(bool setGrblUnits)
{
    sendGcodeLocal(false, "$13=1");
    if (setGrblUnits)
        sendGcodeLocal(false, "G20");
    positionUpdate(true);
}

void GCode::setUnitsTypeDisplay(bool millimeters)
{
    if (millimeters)
    {
        emit setUnitsWork(tr("(mm)"));
        emit setUnitsMachine(tr("(mm)"));
    }
    else
    {
        emit setUnitsWork(tr("(in)"));
        emit setUnitsMachine(tr("(in)"));
    }
}

GCode::PosReqStatus GCode::positionUpdate(bool forceIfEnabled /* = false */)
{
    if (controlParams.usePositionRequest)
    {
        if (forceIfEnabled)
        {
            return sendGcodeLocal(true, REQUEST_CURRENT_POS) ? POS_REQ_RESULT_OK : POS_REQ_RESULT_ERROR;
        }
        else
        {
            int ms = pollPosTimer.elapsed();
            if (ms >= controlParams.postionRequestTimeMilliSec)
            {
                pollPosTimer.restart();
                return sendGcodeLocal(true, REQUEST_CURRENT_POS) ? POS_REQ_RESULT_OK : POS_REQ_RESULT_ERROR;
            }
            else
            {
                return POS_REQ_RESULT_TIMER_SKIP;
            }
        }
    }
    return POS_REQ_RESULT_UNAVAILABLE;
}

