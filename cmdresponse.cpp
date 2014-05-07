#include "cmdresponse.h"

CmdResponse::CmdResponse(const char *buf, int c, int l)
 : cmd(buf), count(c), line(l)
{
    waitForMe = false;
    if (buf[0] == 'M')
    {
        int value = cmd.mid(1,-1).toInt();
        if (value == 9)
            waitForMe = true;
    }
}
