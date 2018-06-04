/****************************************************************************
** Meta object code from reading C++ file 'httptcpserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../lib/hobrasofthttp/httptcpserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httptcpserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HobrasoftHttpd__HttpTcpServer_t {
    QByteArrayData data[8];
    char stringdata0[123];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HobrasoftHttpd__HttpTcpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HobrasoftHttpd__HttpTcpServer_t qt_meta_stringdata_HobrasoftHttpd__HttpTcpServer = {
    {
QT_MOC_LITERAL(0, 0, 29), // "HobrasoftHttpd::HttpTcpServer"
QT_MOC_LITERAL(1, 30, 13), // "slotEncrypted"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 13), // "slotSslErrors"
QT_MOC_LITERAL(4, 59, 16), // "QList<QSslError>"
QT_MOC_LITERAL(5, 76, 19), // "slotPeerVerifyError"
QT_MOC_LITERAL(6, 96, 9), // "QSslError"
QT_MOC_LITERAL(7, 106, 16) // "slotDisconnected"

    },
    "HobrasoftHttpd::HttpTcpServer\0"
    "slotEncrypted\0\0slotSslErrors\0"
    "QList<QSslError>\0slotPeerVerifyError\0"
    "QSslError\0slotDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HobrasoftHttpd__HttpTcpServer[] = {

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
       1,    0,   34,    2, 0x08 /* Private */,
       3,    1,   35,    2, 0x08 /* Private */,
       5,    1,   38,    2, 0x08 /* Private */,
       7,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,

       0        // eod
};

void HobrasoftHttpd::HttpTcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HttpTcpServer *_t = static_cast<HttpTcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotEncrypted(); break;
        case 1: _t->slotSslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 2: _t->slotPeerVerifyError((*reinterpret_cast< const QSslError(*)>(_a[1]))); break;
        case 3: _t->slotDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HobrasoftHttpd::HttpTcpServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_HobrasoftHttpd__HttpTcpServer.data,
      qt_meta_data_HobrasoftHttpd__HttpTcpServer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HobrasoftHttpd::HttpTcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HobrasoftHttpd::HttpTcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HobrasoftHttpd__HttpTcpServer.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int HobrasoftHttpd::HttpTcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
