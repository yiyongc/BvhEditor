/****************************************************************************
** Meta object code from reading C++ file 'FrameWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/FrameWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrameWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cacani__ui__FrameWidget_t {
    QByteArrayData data[16];
    char stringdata[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cacani__ui__FrameWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cacani__ui__FrameWidget_t qt_meta_stringdata_cacani__ui__FrameWidget = {
    {
QT_MOC_LITERAL(0, 0, 23),
QT_MOC_LITERAL(1, 24, 14),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 5),
QT_MOC_LITERAL(4, 46, 13),
QT_MOC_LITERAL(5, 60, 15),
QT_MOC_LITERAL(6, 76, 12),
QT_MOC_LITERAL(7, 89, 11),
QT_MOC_LITERAL(8, 101, 1),
QT_MOC_LITERAL(9, 103, 1),
QT_MOC_LITERAL(10, 105, 9),
QT_MOC_LITERAL(11, 115, 9),
QT_MOC_LITERAL(12, 125, 12),
QT_MOC_LITERAL(13, 138, 19),
QT_MOC_LITERAL(14, 158, 20),
QT_MOC_LITERAL(15, 179, 10)
    },
    "cacani::ui::FrameWidget\0sliderBarRight\0"
    "\0value\0sliderBarLeft\0sliderBarBottom\0"
    "sliderBarTop\0framePanned\0x\0y\0smallGrid\0"
    "smallMode\0setSmallGrid\0showDuplicateDialog\0"
    "removeSelectedFrames\0refreshFPS"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cacani__ui__FrameWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       5,    1,   70,    2, 0x06 /* Public */,
       6,    1,   73,    2, 0x06 /* Public */,
       7,    2,   76,    2, 0x06 /* Public */,
      10,    1,   81,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,   84,    2, 0x08 /* Private */,
      13,    0,   87,    2, 0x08 /* Private */,
      14,    0,   88,    2, 0x08 /* Private */,
      15,    0,   89,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::Bool,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void cacani::ui::FrameWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FrameWidget *_t = static_cast<FrameWidget *>(_o);
        switch (_id) {
        case 0: _t->sliderBarRight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sliderBarLeft((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sliderBarBottom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sliderBarTop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->framePanned((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->smallGrid((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setSmallGrid((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->showDuplicateDialog(); break;
        case 8: _t->removeSelectedFrames(); break;
        case 9: _t->refreshFPS(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FrameWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FrameWidget::sliderBarRight)) {
                *result = 0;
            }
        }
        {
            typedef void (FrameWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FrameWidget::sliderBarLeft)) {
                *result = 1;
            }
        }
        {
            typedef void (FrameWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FrameWidget::sliderBarBottom)) {
                *result = 2;
            }
        }
        {
            typedef void (FrameWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FrameWidget::sliderBarTop)) {
                *result = 3;
            }
        }
        {
            typedef void (FrameWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FrameWidget::framePanned)) {
                *result = 4;
            }
        }
        {
            typedef void (FrameWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FrameWidget::smallGrid)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject cacani::ui::FrameWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_cacani__ui__FrameWidget.data,
      qt_meta_data_cacani__ui__FrameWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *cacani::ui::FrameWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cacani::ui::FrameWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cacani__ui__FrameWidget.stringdata))
        return static_cast<void*>(const_cast< FrameWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int cacani::ui::FrameWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void cacani::ui::FrameWidget::sliderBarRight(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void cacani::ui::FrameWidget::sliderBarLeft(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void cacani::ui::FrameWidget::sliderBarBottom(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void cacani::ui::FrameWidget::sliderBarTop(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void cacani::ui::FrameWidget::framePanned(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void cacani::ui::FrameWidget::smallGrid(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
