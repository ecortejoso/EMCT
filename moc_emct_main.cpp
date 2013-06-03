/****************************************************************************
** Meta object code from reading C++ file 'emct_main.h'
**
** Created: Thu Jul 7 11:55:29 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "emct_main.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'emct_main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_emct_main[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      29,   23,   10,   10, 0x0a,
      54,   10,   10,   10, 0x0a,
      64,   10,   10,   10, 0x0a,
      80,   10,   10,   10, 0x0a,
      90,   10,   10,   10, 0x0a,
     104,   10,  100,   10, 0x0a,
     126,  121,   10,   10, 0x0a,
     144,   10,   10,   10, 0x0a,
     161,   10,   10,   10, 0x0a,
     179,  177,   10,   10, 0x0a,
     206,   10,  100,   10, 0x0a,
     229,  224,   10,   10, 0x0a,
     248,  224,   10,   10, 0x0a,
     272,  224,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_emct_main[] = {
    "emct_main\0\0exitChild()\0event\0"
    "closeEvent(QCloseEvent*)\0getPath()\0"
    "executeOption()\0saveLog()\0helpPdf()\0"
    "int\0readConfigFile()\0widg\0openCon(QWidget*)\0"
    "disableButtons()\0enableButtons()\0s\0"
    "changeEStatus(std::string)\0readVersionFile()\0"
    "text\0logAppend(QString)\0logAppendError(QString)\0"
    "logAppendOp(QString)\0"
};

const QMetaObject emct_main::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_emct_main,
      qt_meta_data_emct_main, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &emct_main::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *emct_main::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *emct_main::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_emct_main))
        return static_cast<void*>(const_cast< emct_main*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int emct_main::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: exitChild(); break;
        case 1: closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 2: getPath(); break;
        case 3: executeOption(); break;
        case 4: saveLog(); break;
        case 5: helpPdf(); break;
        case 6: { int _r = readConfigFile();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: openCon((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 8: disableButtons(); break;
        case 9: enableButtons(); break;
        case 10: changeEStatus((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 11: { int _r = readVersionFile();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: logAppend((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: logAppendError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: logAppendOp((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void emct_main::exitChild()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
