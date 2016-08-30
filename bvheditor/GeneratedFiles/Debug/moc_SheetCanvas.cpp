/****************************************************************************
** Meta object code from reading C++ file 'SheetCanvas.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/SheetCanvas.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SheetCanvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cacani__ui__SheetCanvas_t {
    QByteArrayData data[15];
    char stringdata[191];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cacani__ui__SheetCanvas_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cacani__ui__SheetCanvas_t qt_meta_stringdata_cacani__ui__SheetCanvas = {
    {
QT_MOC_LITERAL(0, 0, 23),
QT_MOC_LITERAL(1, 24, 16),
QT_MOC_LITERAL(2, 41, 0),
QT_MOC_LITERAL(3, 42, 5),
QT_MOC_LITERAL(4, 48, 16),
QT_MOC_LITERAL(5, 65, 16),
QT_MOC_LITERAL(6, 82, 12),
QT_MOC_LITERAL(7, 95, 12),
QT_MOC_LITERAL(8, 108, 12),
QT_MOC_LITERAL(9, 121, 11),
QT_MOC_LITERAL(10, 133, 7),
QT_MOC_LITERAL(11, 141, 11),
QT_MOC_LITERAL(12, 153, 7),
QT_MOC_LITERAL(13, 161, 10),
QT_MOC_LITERAL(14, 172, 18)
    },
    "cacani::ui::SheetCanvas\0xRotationChanged\0"
    "\0angle\0yRotationChanged\0zRotationChanged\0"
    "setXRotation\0setYRotation\0setZRotation\0"
    "drawMessage\0line_no\0const char*\0message\0"
    "playCanvas\0playCanvasSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cacani__ui__SheetCanvas[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       5,    1,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   68,    2, 0x0a /* Public */,
       7,    1,   71,    2, 0x0a /* Public */,
       8,    1,   74,    2, 0x0a /* Public */,
       9,    2,   77,    2, 0x0a /* Public */,
      13,    0,   82,    2, 0x08 /* Private */,
      14,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 11,   10,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void cacani::ui::SheetCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SheetCanvas *_t = static_cast<SheetCanvas *>(_o);
        switch (_id) {
        case 0: _t->xRotationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->yRotationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->zRotationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setXRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setYRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setZRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->drawMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const char*(*)>(_a[2]))); break;
        case 7: _t->playCanvas(); break;
        case 8: _t->playCanvasSelected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SheetCanvas::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SheetCanvas::xRotationChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (SheetCanvas::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SheetCanvas::yRotationChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (SheetCanvas::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SheetCanvas::zRotationChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject cacani::ui::SheetCanvas::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_cacani__ui__SheetCanvas.data,
      qt_meta_data_cacani__ui__SheetCanvas,  qt_static_metacall, 0, 0}
};


const QMetaObject *cacani::ui::SheetCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cacani::ui::SheetCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cacani__ui__SheetCanvas.stringdata))
        return static_cast<void*>(const_cast< SheetCanvas*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int cacani::ui::SheetCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void cacani::ui::SheetCanvas::xRotationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void cacani::ui::SheetCanvas::yRotationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void cacani::ui::SheetCanvas::zRotationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
