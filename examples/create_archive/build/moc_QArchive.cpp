/****************************************************************************
** Meta object code from reading C++ file 'QArchive.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../QArchive.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QArchive.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QArchive__Extractor_t {
    QByteArrayData data[7];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QArchive__Extractor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QArchive__Extractor_t qt_meta_stringdata_QArchive__Extractor = {
    {
QT_MOC_LITERAL(0, 0, 19), // "QArchive::Extractor"
QT_MOC_LITERAL(1, 20, 8), // "finished"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 9), // "extracted"
QT_MOC_LITERAL(4, 40, 10), // "extracting"
QT_MOC_LITERAL(5, 51, 6), // "status"
QT_MOC_LITERAL(6, 58, 5) // "error"

    },
    "QArchive::Extractor\0finished\0\0extracted\0"
    "extracting\0status\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QArchive__Extractor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       4,    1,   43,    2, 0x06 /* Public */,
       5,    2,   46,    2, 0x06 /* Public */,
       6,    2,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Short, QMetaType::QString,    2,    2,

       0        // eod
};

void QArchive::Extractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Extractor *_t = static_cast<Extractor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->extracted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->extracting((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->status((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->error((*reinterpret_cast< short(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Extractor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Extractor::finished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Extractor::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Extractor::extracted)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Extractor::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Extractor::extracting)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Extractor::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Extractor::status)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Extractor::*_t)(short , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Extractor::error)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject QArchive::Extractor::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QArchive__Extractor.data,
      qt_meta_data_QArchive__Extractor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QArchive::Extractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QArchive::Extractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QArchive__Extractor.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int QArchive::Extractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void QArchive::Extractor::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QArchive::Extractor::extracted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QArchive::Extractor::extracting(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QArchive::Extractor::status(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QArchive::Extractor::error(short _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
struct qt_meta_stringdata_QArchive__Compressor_t {
    QByteArrayData data[6];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QArchive__Compressor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QArchive__Compressor_t qt_meta_stringdata_QArchive__Compressor = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QArchive::Compressor"
QT_MOC_LITERAL(1, 21, 8), // "finished"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "compressing"
QT_MOC_LITERAL(4, 43, 10), // "compressed"
QT_MOC_LITERAL(5, 54, 5) // "error"

    },
    "QArchive::Compressor\0finished\0\0"
    "compressing\0compressed\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QArchive__Compressor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,
       4,    1,   38,    2, 0x06 /* Public */,
       5,    2,   41,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Short, QMetaType::QString,    2,    2,

       0        // eod
};

void QArchive::Compressor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Compressor *_t = static_cast<Compressor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->compressing((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->compressed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->error((*reinterpret_cast< short(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Compressor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Compressor::finished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Compressor::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Compressor::compressing)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Compressor::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Compressor::compressed)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Compressor::*_t)(short , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Compressor::error)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject QArchive::Compressor::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QArchive__Compressor.data,
      qt_meta_data_QArchive__Compressor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QArchive::Compressor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QArchive::Compressor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QArchive__Compressor.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int QArchive::Compressor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void QArchive::Compressor::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QArchive::Compressor::compressing(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QArchive::Compressor::compressed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QArchive::Compressor::error(short _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
