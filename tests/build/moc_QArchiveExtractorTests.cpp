/****************************************************************************
** Meta object code from reading C++ file 'QArchiveExtractorTests.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QArchiveExtractorTests.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QArchiveExtractorTests.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QArchiveExtractor_t {
    QByteArrayData data[15];
    char stringdata0[323];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QArchiveExtractor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QArchiveExtractor_t qt_meta_stringdata_QArchiveExtractor = {
    {
QT_MOC_LITERAL(0, 0, 17), // "QArchiveExtractor"
QT_MOC_LITERAL(1, 18, 16), // "simpleExtraction"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 16), // "usingPauseResume"
QT_MOC_LITERAL(4, 53, 11), // "usingCancel"
QT_MOC_LITERAL(5, 65, 13), // "usingPassword"
QT_MOC_LITERAL(6, 79, 54), // "usingWrongPasswordAndAfter3Tr..."
QT_MOC_LITERAL(7, 134, 32), // "cancelingPasswordRequiredSignals"
QT_MOC_LITERAL(8, 167, 22), // "testInvalidArchivePath"
QT_MOC_LITERAL(9, 190, 22), // "testInvalidDestination"
QT_MOC_LITERAL(10, 213, 27), // "testArchivePasswordNotGiven"
QT_MOC_LITERAL(11, 241, 24), // "testArchiveWrongPassword"
QT_MOC_LITERAL(12, 266, 26), // "isExtractorObjectReuseable"
QT_MOC_LITERAL(13, 293, 13), // "supportForTar"
QT_MOC_LITERAL(14, 307, 15) // "cleanupTestCase"

    },
    "QArchiveExtractor\0simpleExtraction\0\0"
    "usingPauseResume\0usingCancel\0usingPassword\0"
    "usingWrongPasswordAndAfter3TriesGiveTheCorrectPassword\0"
    "cancelingPasswordRequiredSignals\0"
    "testInvalidArchivePath\0testInvalidDestination\0"
    "testArchivePasswordNotGiven\0"
    "testArchiveWrongPassword\0"
    "isExtractorObjectReuseable\0supportForTar\0"
    "cleanupTestCase"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QArchiveExtractor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    0,   90,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QArchiveExtractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QArchiveExtractor *_t = static_cast<QArchiveExtractor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->simpleExtraction(); break;
        case 1: _t->usingPauseResume(); break;
        case 2: _t->usingCancel(); break;
        case 3: _t->usingPassword(); break;
        case 4: _t->usingWrongPasswordAndAfter3TriesGiveTheCorrectPassword(); break;
        case 5: _t->cancelingPasswordRequiredSignals(); break;
        case 6: _t->testInvalidArchivePath(); break;
        case 7: _t->testInvalidDestination(); break;
        case 8: _t->testArchivePasswordNotGiven(); break;
        case 9: _t->testArchiveWrongPassword(); break;
        case 10: _t->isExtractorObjectReuseable(); break;
        case 11: _t->supportForTar(); break;
        case 12: _t->cleanupTestCase(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QArchiveExtractor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QArchiveExtractor.data,
      qt_meta_data_QArchiveExtractor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QArchiveExtractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QArchiveExtractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QArchiveExtractor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QArchiveExtractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
