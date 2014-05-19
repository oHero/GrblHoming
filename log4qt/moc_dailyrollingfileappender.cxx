/****************************************************************************
** Meta object code from reading C++ file 'dailyrollingfileappender.h'
**
** Created: Mon 19. May 20:33:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dailyrollingfileappender.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dailyrollingfileappender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Log4Qt__DailyRollingFileAppender[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       1,   17, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      41,   33, 0x0a095103,

 // enums: name, flags, count, data
      53, 0x0,    6,   21,

 // enum data: key, value
      65, uint(Log4Qt::DailyRollingFileAppender::MINUTELY_ROLLOVER),
      83, uint(Log4Qt::DailyRollingFileAppender::HOURLY_ROLLOVER),
      99, uint(Log4Qt::DailyRollingFileAppender::HALFDAILY_ROLLOVER),
     118, uint(Log4Qt::DailyRollingFileAppender::DAILY_ROLLOVER),
     133, uint(Log4Qt::DailyRollingFileAppender::WEEKLY_ROLLOVER),
     149, uint(Log4Qt::DailyRollingFileAppender::MONTHLY_ROLLOVER),

       0        // eod
};

static const char qt_meta_stringdata_Log4Qt__DailyRollingFileAppender[] = {
    "Log4Qt::DailyRollingFileAppender\0"
    "QString\0datePattern\0DatePattern\0"
    "MINUTELY_ROLLOVER\0HOURLY_ROLLOVER\0"
    "HALFDAILY_ROLLOVER\0DAILY_ROLLOVER\0"
    "WEEKLY_ROLLOVER\0MONTHLY_ROLLOVER\0"
};

const QMetaObject Log4Qt::DailyRollingFileAppender::staticMetaObject = {
    { &FileAppender::staticMetaObject, qt_meta_stringdata_Log4Qt__DailyRollingFileAppender,
      qt_meta_data_Log4Qt__DailyRollingFileAppender, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Log4Qt::DailyRollingFileAppender::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Log4Qt::DailyRollingFileAppender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Log4Qt::DailyRollingFileAppender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__DailyRollingFileAppender))
        return static_cast<void*>(const_cast< DailyRollingFileAppender*>(this));
    return FileAppender::qt_metacast(_clname);
}

int Log4Qt::DailyRollingFileAppender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = FileAppender::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = datePattern(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setDatePattern(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
