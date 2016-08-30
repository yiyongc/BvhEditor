/****************************************************************************
** Meta object code from reading C++ file 'MeshOptionDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../OpenToonz/MeshOptionDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MeshOptionDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MeshOptionDialog_t {
    QByteArrayData data[10];
    char stringdata[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MeshOptionDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MeshOptionDialog_t qt_meta_stringdata_MeshOptionDialog = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 14),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 14),
QT_MOC_LITERAL(4, 48, 13),
QT_MOC_LITERAL(5, 62, 19),
QT_MOC_LITERAL(6, 82, 3),
QT_MOC_LITERAL(7, 86, 18),
QT_MOC_LITERAL(8, 105, 23),
QT_MOC_LITERAL(9, 129, 22)
    },
    "MeshOptionDialog\0defaultClicked\0\0"
    "confirmClicked\0cancelClicked\0"
    "updateMarginSpinBox\0val\0updateMarginSlider\0"
    "updateEdgeLengthSpinBox\0updateEdgeLengthSlider"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MeshOptionDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    1,   52,    2, 0x08 /* Private */,
       7,    1,   55,    2, 0x08 /* Private */,
       8,    0,   58,    2, 0x08 /* Private */,
       9,    0,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MeshOptionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MeshOptionDialog *_t = static_cast<MeshOptionDialog *>(_o);
        switch (_id) {
        case 0: _t->defaultClicked(); break;
        case 1: _t->confirmClicked(); break;
        case 2: _t->cancelClicked(); break;
        case 3: _t->updateMarginSpinBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->updateMarginSlider((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->updateEdgeLengthSpinBox(); break;
        case 6: _t->updateEdgeLengthSlider(); break;
        default: ;
        }
    }
}

const QMetaObject MeshOptionDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MeshOptionDialog.data,
      qt_meta_data_MeshOptionDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *MeshOptionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeshOptionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MeshOptionDialog.stringdata))
        return static_cast<void*>(const_cast< MeshOptionDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int MeshOptionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
