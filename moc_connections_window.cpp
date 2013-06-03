/****************************************************************************
** Meta object code from reading C++ file 'connections_window.h'
**
** Created: Thu Jul 7 11:55:26 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "connections_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connections_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_connectionsWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      36,   31,   18,   18, 0x05,
      56,   31,   18,   18, 0x05,
      75,   31,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      97,   91,   18,   18, 0x0a,
     124,  122,   18,   18, 0x0a,
     147,   18,   18,   18, 0x0a,
     164,  160,   18,   18, 0x0a,
     187,  160,   18,   18, 0x0a,
     246,  214,  210,   18, 0x0a,
     318,  316,  210,   18, 0x0a,
     338,  316,  210,   18, 0x0a,
     360,  214,  210,   18, 0x0a,
     460,  431,  210,   18, 0x0a,
     531,  519,   18,   18, 0x0a,
     598,  569,  210,   18, 0x0a,
     662,  569,  210,   18, 0x0a,
     731,  316,  719,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_connectionsWindow[] = {
    "connectionsWindow\0\0sigEnable()\0text\0"
    "textResult(QString)\0textError(QString)\0"
    "textOp(QString)\0event\0closeEvent(QCloseEvent*)\0"
    "m\0execeOption(E_machine)\0executeAll()\0"
    "num\0prepareExecutionE(int)\0"
    "prepareExecutionI(int)\0int\0"
    "ipAdd64,ipAdd32,id,name,m_dpath\0"
    "updateDC(std::string,std::string,std::string,std::string,std::string)\0"
    "s\0reboot(std::string)\0shutdown(std::string)\0"
    "updateLEU(std::string,std::string,std::string,std::string,std::string)\0"
    "ipAdd64,ipAdd32,name,m_dpath\0"
    "updateRBC(std::string,std::string,std::string,std::string)\0"
    "ip,filename\0updateLEUaux(std::string,std::string)\0"
    "ipAdd64,ipAdd32,name,m_mpath\0"
    "displayVersion(std::string,std::string,std::string,std::string)\0"
    "updateExecutables(string,string,std::string,std::string)\0"
    "std::string\0elimPath(std::string)\0"
};

const QMetaObject connectionsWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_connectionsWindow,
      qt_meta_data_connectionsWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &connectionsWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *connectionsWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *connectionsWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_connectionsWindow))
        return static_cast<void*>(const_cast< connectionsWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int connectionsWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigEnable(); break;
        case 1: textResult((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: textError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: textOp((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 5: execeOption((*reinterpret_cast< E_machine(*)>(_a[1]))); break;
        case 6: executeAll(); break;
        case 7: prepareExecutionE((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: prepareExecutionI((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: { int _r = updateDC((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2])),(*reinterpret_cast< std::string(*)>(_a[3])),(*reinterpret_cast< std::string(*)>(_a[4])),(*reinterpret_cast< std::string(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: { int _r = reboot((*reinterpret_cast< std::string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: { int _r = shutdown((*reinterpret_cast< std::string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: { int _r = updateLEU((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2])),(*reinterpret_cast< std::string(*)>(_a[3])),(*reinterpret_cast< std::string(*)>(_a[4])),(*reinterpret_cast< std::string(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: { int _r = updateRBC((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2])),(*reinterpret_cast< std::string(*)>(_a[3])),(*reinterpret_cast< std::string(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 14: updateLEUaux((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2]))); break;
        case 15: { int _r = displayVersion((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2])),(*reinterpret_cast< std::string(*)>(_a[3])),(*reinterpret_cast< std::string(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: { int _r = updateExecutables((*reinterpret_cast< string(*)>(_a[1])),(*reinterpret_cast< string(*)>(_a[2])),(*reinterpret_cast< std::string(*)>(_a[3])),(*reinterpret_cast< std::string(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 17: { std::string _r = elimPath((*reinterpret_cast< std::string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::string*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void connectionsWindow::sigEnable()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void connectionsWindow::textResult(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void connectionsWindow::textError(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void connectionsWindow::textOp(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
