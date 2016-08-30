/****************************************************************************
** Meta object code from reading C++ file 'ImportDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/ImportDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImportDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cacani__ui__ImportDialog_t {
    QByteArrayData data[6];
    char stringdata[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cacani__ui__ImportDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cacani__ui__ImportDialog_t qt_meta_stringdata_cacani__ui__ImportDialog = {
    {
QT_MOC_LITERAL(0, 0, 24),
QT_MOC_LITERAL(1, 25, 14),
QT_MOC_LITERAL(2, 40, 0),
QT_MOC_LITERAL(3, 41, 13),
QT_MOC_LITERAL(4, 55, 13),
QT_MOC_LITERAL(5, 69, 7)
    },
    "cacani::ui::ImportDialog\0confirmClicked\0"
    "\0cancelClicked\0radio2Clicked\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cacani__ui__ImportDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    1,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,

       0        // eod
};

void cacani::ui::ImportDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ImportDialog *_t = static_cast<ImportDialog *>(_o);
        switch (_id) {
        case 0: _t->confirmClicked(); break;
        case 1: _t->cancelClicked(); break;
        case 2: _t->radio2Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject cacani::ui::ImportDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_cacani__ui__ImportDialog.data,
      qt_meta_data_cacani__ui__ImportDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *cacani::ui::ImportDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cacani::ui::ImportDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cacani__ui__ImportDialog.stringdata))
        return static_cast<void*>(const_cast< ImportDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int cacani::ui::ImportDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
