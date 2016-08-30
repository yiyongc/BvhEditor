/****************************************************************************
** Meta object code from reading C++ file 'StartupDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/StartupDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StartupDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cacani__ui__StartupDialog_t {
    QByteArrayData data[8];
    char stringdata[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cacani__ui__StartupDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cacani__ui__StartupDialog_t qt_meta_stringdata_cacani__ui__StartupDialog = {
    {
QT_MOC_LITERAL(0, 0, 25),
QT_MOC_LITERAL(1, 26, 12),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 14),
QT_MOC_LITERAL(4, 55, 13),
QT_MOC_LITERAL(5, 69, 13),
QT_MOC_LITERAL(6, 83, 11),
QT_MOC_LITERAL(7, 95, 8)
    },
    "cacani::ui::StartupDialog\0file_updated\0"
    "\0confirmClicked\0cancelClicked\0"
    "browseClicked\0fileUpdated\0fileName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cacani__ui__StartupDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   42,    2, 0x08 /* Private */,
       4,    0,   43,    2, 0x08 /* Private */,
       5,    0,   44,    2, 0x08 /* Private */,
       6,    1,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void cacani::ui::StartupDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        StartupDialog *_t = static_cast<StartupDialog *>(_o);
        switch (_id) {
        case 0: _t->file_updated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->confirmClicked(); break;
        case 2: _t->cancelClicked(); break;
        case 3: _t->browseClicked(); break;
        case 4: _t->fileUpdated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (StartupDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&StartupDialog::file_updated)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject cacani::ui::StartupDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_cacani__ui__StartupDialog.data,
      qt_meta_data_cacani__ui__StartupDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *cacani::ui::StartupDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cacani::ui::StartupDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cacani__ui__StartupDialog.stringdata))
        return static_cast<void*>(const_cast< StartupDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int cacani::ui::StartupDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void cacani::ui::StartupDialog::file_updated(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
