/****************************************************************************
** Meta object code from reading C++ file 'stringmatchfilter.h'
**
** Created: Mon 19. May 20:33:56 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stringmatchfilter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stringmatchfilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Log4Qt__StringMatchFilter[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      31,   26, 0x01095103,
      53,   45, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_Log4Qt__StringMatchFilter[] = {
    "Log4Qt::StringMatchFilter\0bool\0"
    "acceptOnMatch\0QString\0stringToMatch\0"
};

const QMetaObject Log4Qt::StringMatchFilter::staticMetaObject = {
    { &Filter::staticMetaObject, qt_meta_stringdata_Log4Qt__StringMatchFilter,
      qt_meta_data_Log4Qt__StringMatchFilter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Log4Qt::StringMatchFilter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Log4Qt::StringMatchFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Log4Qt::StringMatchFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__StringMatchFilter))
        return static_cast<void*>(const_cast< StringMatchFilter*>(this));
    return Filter::qt_metacast(_clname);
}

int Log4Qt::StringMatchFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Filter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = acceptOnMatch(); break;
        case 1: *reinterpret_cast< QString*>(_v) = stringToMatch(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setAcceptOnMatch(*reinterpret_cast< bool*>(_v)); break;
        case 1: setStringToMatch(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
