#include <QString>
#include <QtTest>
#include "tst_positemtest.h"
#include "tst_preloadtest.h"
enum GC_LOG_TYPES
{
    LOG_DEBUG_TYPE = 1,
    LOG_ERROR_TYPE,
    LOG_WARN_TYPE,
    LOG_INFO_TYPE,
    LOG_STATUS_TYPE,
};
void logit(GC_LOG_TYPES type, const char *str, va_list args);
//replaces multiple QTEST_APPLESS_MAIN(PreloadTest)
int main(int argc, char** argv)
{
   int status = 0;
   {
      PreloadTest tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   {
      PositemTest tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   return status;
}

//------------------------------
void status(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_STATUS_TYPE, str, args);
    va_end(args);
}

void err(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_ERROR_TYPE, str, args);
    va_end(args);
}

void warn(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_WARN_TYPE, str, args);
    va_end(args);
}

void info(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_INFO_TYPE, str, args);
    va_end(args);
}

void diag(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_DEBUG_TYPE, str, args);
    va_end(args);
}

void logit(GC_LOG_TYPES type, const char *str, va_list args)
{
#define PRNTBUFSIZE 500
    char buf[PRNTBUFSIZE];
    buf[PRNTBUFSIZE-1] = '\0';

    vsnprintf(buf, sizeof(buf) - 1, str, args);

    int len = strlen(buf);
    if (len > 0)
    {
        if (len == 1 && (buf[0] == '\r' || buf[0] == '\n'))
        {
            buf[0] = '\0';
        }
        else if (len > 1)
        {
            if (buf[len - 2] == '\r' || buf[len - 2] == '\n')
                buf[len - 2] = '\0';
            else if (buf[len - 1] == '\r' || buf[len - 1] == '\n')
                buf[len - 1] = '\0';
        }
    }
    else if (len == 0)
        return;

    switch (type)
    {
        case LOG_STATUS_TYPE:
            printf("%s", buf);
            break;
        case LOG_DEBUG_TYPE:
            printf("%s", buf);
            break;
        case LOG_ERROR_TYPE:
            printf("%s", buf);
            break;
        case LOG_WARN_TYPE:
            printf("%s", buf);
            break;
        case LOG_INFO_TYPE:
            printf("%s", buf);
            break;
    }
}
