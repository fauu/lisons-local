/****************************************************************************
** Meta object code from reading C++ file 'httpconnection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../lib/hobrasofthttp/httpconnection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httpconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HobrasoftHttpd__HttpConnection_t {
    QByteArrayData data[6];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HobrasoftHttpd__HttpConnection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HobrasoftHttpd__HttpConnection_t qt_meta_stringdata_HobrasoftHttpd__HttpConnection = {
    {
QT_MOC_LITERAL(0, 0, 30), // "HobrasoftHttpd::HttpConnection"
QT_MOC_LITERAL(1, 31, 5), // "close"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 11), // "slotTimeout"
QT_MOC_LITERAL(4, 50, 8), // "slotRead"
QT_MOC_LITERAL(5, 59, 16) // "slotDisconnected"

    },
    "HobrasoftHttpd::HttpConnection\0close\0"
    "\0slotTimeout\0slotRead\0slotDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HobrasoftHttpd__HttpConnection[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HobrasoftHttpd::HttpConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HttpConnection *_t = static_cast<HttpConnection *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->close(); break;
        case 1: _t->slotTimeout(); break;
        case 2: _t->slotRead(); break;
        case 3: _t->slotDisconnected(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject HobrasoftHttpd::HttpConnection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HobrasoftHttpd__HttpConnection.data,
      qt_meta_data_HobrasoftHttpd__HttpConnection,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HobrasoftHttpd::HttpConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HobrasoftHttpd::HttpConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HobrasoftHttpd__HttpConnection.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HobrasoftHttpd::HttpConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
