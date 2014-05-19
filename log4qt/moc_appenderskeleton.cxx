/****************************************************************************
** Meta object code from reading C++ file 'appenderskeleton.h'
**
** Created: Mon 19. May 20:33:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "appenderskeleton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'appenderskeleton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Log4Qt__AppenderSkeleton[] = {

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
      30,   25, 0x01095001,
      39,   25, 0x01095001,
      54,   48, 0x0009510b,

       0        // eod
};

static const char qt_meta_stringdata_Log4Qt__AppenderSkeleton[] = {
    "Log4Qt::AppenderSkeleton\0bool\0isActive\0"
    "isClosed\0Level\0threshold\0"
};

const QMetaObject Log4Qt::AppenderSkeleton::staticMetaObject = {
    { &Appender::staticMetaObject, qt_meta_stringdata_Log4Qt__AppenderSkeleton,
      qt_meta_data_Log4Qt__AppenderSkeleton, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Log4Qt::AppenderSkeleton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Log4Qt::AppenderSkeleton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Log4Qt::AppenderSkeleton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__AppenderSkeleton))
        return static_cast<void*>(const_cast< AppenderSkeleton*>(this));
    return Appender::qt_metacast(_clname);
}

int Log4Qt::AppenderSkeleton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Appender::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isActive(); break;
        case 1: *reinterpret_cast< bool*>(_v) = isClosed(); break;
        case 2: *reinterpret_cast< Level*>(_v) = threshold(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: setThreshold(*reinterpret_cast< Level*>(_v)); break;
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
