/****************************************************************************
** Meta object code from reading C++ file 'writerappender.h'
**
** Created: Mon 19. May 20:33:56 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "writerappender.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'writerappender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Log4Qt__WriterAppender[] = {

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
      35,   23, 0x0009510b,
      57,   44, 0x0009510b,
      69,   64, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_Log4Qt__WriterAppender[] = {
    "Log4Qt::WriterAppender\0QTextCodec*\0"
    "encoding\0QTextStream*\0writer\0bool\0"
    "immediateFlush\0"
};

const QMetaObject Log4Qt::WriterAppender::staticMetaObject = {
    { &AppenderSkeleton::staticMetaObject, qt_meta_stringdata_Log4Qt__WriterAppender,
      qt_meta_data_Log4Qt__WriterAppender, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Log4Qt::WriterAppender::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Log4Qt::WriterAppender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Log4Qt::WriterAppender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__WriterAppender))
        return static_cast<void*>(const_cast< WriterAppender*>(this));
    return AppenderSkeleton::qt_metacast(_clname);
}

int Log4Qt::WriterAppender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppenderSkeleton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QTextCodec**>(_v) = encoding(); break;
        case 1: *reinterpret_cast< QTextStream**>(_v) = writer(); break;
        case 2: *reinterpret_cast< bool*>(_v) = immediateFlush(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setEncoding(*reinterpret_cast< QTextCodec**>(_v)); break;
        case 1: setWriter(*reinterpret_cast< QTextStream**>(_v)); break;
        case 2: setImmediateFlush(*reinterpret_cast< bool*>(_v)); break;
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
