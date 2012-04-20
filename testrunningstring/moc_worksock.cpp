/****************************************************************************
** Meta object code from reading C++ file 'worksock.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../worksock.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'worksock.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_worksock[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      24,    9,    9,    9, 0x0a,
      42,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_worksock[] = {
    "worksock\0\0receiveData()\0errorProcessing()\0"
    "connectProcessing()\0"
};

void worksock::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        worksock *_t = static_cast<worksock *>(_o);
        switch (_id) {
        case 0: _t->receiveData(); break;
        case 1: _t->errorProcessing(); break;
        case 2: _t->connectProcessing(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData worksock::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject worksock::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_worksock,
      qt_meta_data_worksock, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &worksock::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *worksock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *worksock::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_worksock))
        return static_cast<void*>(const_cast< worksock*>(this));
    return QObject::qt_metacast(_clname);
}

int worksock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
