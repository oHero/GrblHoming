/****************************************************************************
** Meta object code from reading C++ file 'levelrangefilter.h'
**
** Created: Mon 19. May 20:33:55 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "levelrangefilter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'levelrangefilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Log4Qt__LevelRangeFilter[] = {

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
      30,   25, 0x01095103,
      50,   44, 0x0009510b,
      59,   44, 0x0009510b,

       0        // eod
};

static const char qt_meta_stringdata_Log4Qt__LevelRangeFilter[] = {
    "Log4Qt::LevelRangeFilter\0bool\0"
    "acceptOnMatch\0Level\0levelMax\0levelMin\0"
};

const QMetaObject Log4Qt::LevelRangeFilter::staticMetaObject = {
    { &Filter::staticMetaObject, qt_meta_stringdata_Log4Qt__LevelRangeFilter,
      qt_meta_data_Log4Qt__LevelRangeFilter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Log4Qt::LevelRangeFilter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Log4Qt::LevelRangeFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Log4Qt::LevelRangeFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__LevelRangeFilter))
        return static_cast<void*>(const_cast< LevelRangeFilter*>(this));
    return Filter::qt_metacast(_clname);
}

int Log4Qt::LevelRangeFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Filter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = acceptOnMatch(); break;
        case 1: *reinterpret_cast< Level*>(_v) = levelMax(); break;
        case 2: *reinterpret_cast< Level*>(_v) = levelMin(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setAcceptOnMatch(*reinterpret_cast< bool*>(_v)); break;
        case 1: setLevelMax(*reinterpret_cast< Level*>(_v)); break;
        case 2: setLevelMin(*reinterpret_cast< Level*>(_v)); break;
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
