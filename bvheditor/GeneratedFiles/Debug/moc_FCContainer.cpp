/****************************************************************************
** Meta object code from reading C++ file 'FCContainer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/FCContainer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FCContainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cacani__ui__FCContainer_t {
    QByteArrayData data[17];
    char stringdata[241];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cacani__ui__FCContainer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cacani__ui__FCContainer_t qt_meta_stringdata_cacani__ui__FCContainer = {
    {
QT_MOC_LITERAL(0, 0, 23),
QT_MOC_LITERAL(1, 24, 13),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 12),
QT_MOC_LITERAL(4, 52, 17),
QT_MOC_LITERAL(5, 70, 5),
QT_MOC_LITERAL(6, 76, 17),
QT_MOC_LITERAL(7, 94, 16),
QT_MOC_LITERAL(8, 111, 12),
QT_MOC_LITERAL(9, 124, 16),
QT_MOC_LITERAL(10, 141, 18),
QT_MOC_LITERAL(11, 160, 19),
QT_MOC_LITERAL(12, 180, 17),
QT_MOC_LITERAL(13, 198, 20),
QT_MOC_LITERAL(14, 219, 17),
QT_MOC_LITERAL(15, 237, 1),
QT_MOC_LITERAL(16, 239, 1)
    },
    "cacani::ui::FCContainer\0updateDrawing\0"
    "\0adjustScroll\0setScrollForFrame\0value\0"
    "setScrollForLayer\0adjustHorzScroll\0"
    "adjustActive\0adjustVertScroll\0"
    "scrollToChangeLeft\0scrollToChangeRight\0"
    "scrollToChangeTop\0scrollToChangeBottom\0"
    "frameWidgetPanned\0x\0y"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cacani__ui__FCContainer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    1,   76,    2, 0x08 /* Private */,
       6,    1,   79,    2, 0x08 /* Private */,
       7,    1,   82,    2, 0x08 /* Private */,
       7,    0,   85,    2, 0x28 /* Private | MethodCloned */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    1,   87,    2, 0x08 /* Private */,
      11,    1,   90,    2, 0x08 /* Private */,
      12,    1,   93,    2, 0x08 /* Private */,
      13,    1,   96,    2, 0x08 /* Private */,
      14,    2,   99,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   15,   16,

       0        // eod
};

void cacani::ui::FCContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FCContainer *_t = static_cast<FCContainer *>(_o);
        switch (_id) {
        case 0: _t->updateDrawing(); break;
        case 1: _t->adjustScroll(); break;
        case 2: _t->setScrollForFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setScrollForLayer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->adjustHorzScroll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->adjustHorzScroll(); break;
        case 6: _t->adjustVertScroll(); break;
        case 7: _t->scrollToChangeLeft((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->scrollToChangeRight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->scrollToChangeTop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->scrollToChangeBottom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->frameWidgetPanned((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject cacani::ui::FCContainer::staticMetaObject = {
    { &QSplitter::staticMetaObject, qt_meta_stringdata_cacani__ui__FCContainer.data,
      qt_meta_data_cacani__ui__FCContainer,  qt_static_metacall, 0, 0}
};


const QMetaObject *cacani::ui::FCContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cacani::ui::FCContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cacani__ui__FCContainer.stringdata))
        return static_cast<void*>(const_cast< FCContainer*>(this));
    return QSplitter::qt_metacast(_clname);
}

int cacani::ui::FCContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSplitter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
