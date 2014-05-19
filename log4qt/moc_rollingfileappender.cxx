/****************************************************************************
** Meta object code from reading C++ file 'rollingfileappender.h'
**
** Created: Mon 19. May 20:33:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "rollingfileappender.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rollingfileappender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Log4Qt__RollingFileAppender[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       3,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      32,   28, 0x02095103,
      54,   47, 0x04095103,
      78,   70, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_Log4Qt__RollingFileAppender[] = {
    "Log4Qt::RollingFileAppender\0int\0"
    "maxBackupIndex\0qint64\0maximumFileSize\0"
    "QString\0maxFileSize\0"
};

const QMetaObject Log4Qt::RollingFileAppender::staticMetaObject = {
    { &FileAppender::staticMetaObject, qt_meta_stringdata_Log4Qt__RollingFileAppender,
      qt_meta_data_Log4Qt__RollingFileAppender, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Log4Qt::RollingFileAppender::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Log4Qt::RollingFileAppender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Log4Qt::RollingFileAppender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__RollingFileAppender))
        return static_cast<void*>(const_cast< RollingFileAppender*>(this));
    return FileAppender::qt_metacast(_clname);
}

int Log4Qt::RollingFileAppender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = FileAppender::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = maxBackupIndex(); break;
        case 1: *reinterpret_cast< qint64*>(_v) = maximumFileSize(); break;
        case 2: *reinterpret_cast< QString*>(_v) = maxFileSize(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setMaxBackupIndex(*reinterpret_cast< int*>(_v)); break;
        case 1: setMaximumFileSize(*reinterpret_cast< qint64*>(_v)); break;
        case 2: setMaxFileSize(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
