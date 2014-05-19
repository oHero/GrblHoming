/****************************************************************************
** Meta object code from reading C++ file 'ttcclayout.h'
**
** Created: Mon 19. May 20:33:55 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ttcclayout.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ttcclayout.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Log4Qt__TTCCLayout[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       4,   14, // properties
       1,   26, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      24,   19, 0x01095103,
      42,   19, 0x01095103,
      66,   58, 0x0a095103,
      77,   19, 0x01095103,

 // enums: name, flags, count, data
      92, 0x0,    5,   30,

 // enum data: key, value
     103, uint(Log4Qt::TTCCLayout::NONE),
     108, uint(Log4Qt::TTCCLayout::ISO8601),
     116, uint(Log4Qt::TTCCLayout::ABSOLUTE),
     125, uint(Log4Qt::TTCCLayout::DATE),
     130, uint(Log4Qt::TTCCLayout::RELATIVE),

       0        // eod
};

static const char qt_meta_stringdata_Log4Qt__TTCCLayout[] = {
    "Log4Qt::TTCCLayout\0bool\0categoryPrefixing\0"
    "contextPrinting\0QString\0dateFormat\0"
    "threadPrinting\0DateFormat\0NONE\0ISO8601\0"
    "ABSOLUTE\0DATE\0RELATIVE\0"
};

const QMetaObject Log4Qt::TTCCLayout::staticMetaObject = {
    { &Layout::staticMetaObject, qt_meta_stringdata_Log4Qt__TTCCLayout,
      qt_meta_data_Log4Qt__TTCCLayout, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Log4Qt::TTCCLayout::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Log4Qt::TTCCLayout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Log4Qt::TTCCLayout::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__TTCCLayout))
        return static_cast<void*>(const_cast< TTCCLayout*>(this));
    return Layout::qt_metacast(_clname);
}

int Log4Qt::TTCCLayout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Layout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = categoryPrefixing(); break;
        case 1: *reinterpret_cast< bool*>(_v) = contextPrinting(); break;
        case 2: *reinterpret_cast< QString*>(_v) = dateFormat(); break;
        case 3: *reinterpret_cast< bool*>(_v) = threadPrinting(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setCategoryPrefixing(*reinterpret_cast< bool*>(_v)); break;
        case 1: setContextPrinting(*reinterpret_cast< bool*>(_v)); break;
        case 2: setDateFormat(*reinterpret_cast< QString*>(_v)); break;
        case 3: setThreadPrinting(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
