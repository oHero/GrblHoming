/****************************************************************************
** Meta object code from reading C++ file 'logger.h'
**
** Created: Mon 19. May 20:33:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "logger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Log4Qt__Logger[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      20,   15, 0x01095103,
      37,   31, 0x0009510b,
      61,   43, 0x00095009,
      86,   78, 0x0a095001,
      99,   91, 0x00095009,

       0        // eod
};

static const char qt_meta_stringdata_Log4Qt__Logger[] = {
    "Log4Qt::Logger\0bool\0additivity\0Level\0"
    "level\0LoggerRepository*\0loggerRepository\0"
    "QString\0name\0Logger*\0parentLogger\0"
};

const QMetaObject Log4Qt::Logger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Log4Qt__Logger,
      qt_meta_data_Log4Qt__Logger, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Log4Qt::Logger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Log4Qt::Logger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Log4Qt::Logger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__Logger))
        return static_cast<void*>(const_cast< Logger*>(this));
    return QObject::qt_metacast(_clname);
}

int Log4Qt::Logger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = additivity(); break;
        case 1: *reinterpret_cast< Level*>(_v) = level(); break;
        case 2: *reinterpret_cast< LoggerRepository**>(_v) = loggerRepository(); break;
        case 3: *reinterpret_cast< QString*>(_v) = name(); break;
        case 4: *reinterpret_cast< Logger**>(_v) = parentLogger(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setAdditivity(*reinterpret_cast< bool*>(_v)); break;
        case 1: setLevel(*reinterpret_cast< Level*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
