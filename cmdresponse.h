#ifndef CMDRESPONSE_H
#define CMDRESPONSE_H

#include <QString>

class CmdResponse
{
public:
    CmdResponse(const char *buf, int c, int l);

public:
    QString cmd;
    int count;
    int line;
    bool waitForMe;
};


#endif // CMDRESPONSE_H
