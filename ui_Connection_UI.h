/********************************************************************************
** Form generated from reading UI file 'Connection_UI.ui'
**
** Created: Thu Jul 7 11:54:50 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTION_UI_H
#define UI_CONNECTION_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectMain
{
public:
    QWidget *ConWin;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollSpace;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ConnectMain)
    {
        if (ConnectMain->objectName().isEmpty())
            ConnectMain->setObjectName(QString::fromUtf8("ConnectMain"));
        ConnectMain->resize(555, 342);
        ConWin = new QWidget(ConnectMain);
        ConWin->setObjectName(QString::fromUtf8("ConWin"));
        verticalLayout = new QVBoxLayout(ConWin);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(ConWin);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollSpace = new QWidget();
        scrollSpace->setObjectName(QString::fromUtf8("scrollSpace"));
        scrollSpace->setGeometry(QRect(0, 0, 535, 300));
        scrollArea->setWidget(scrollSpace);

        verticalLayout->addWidget(scrollArea);

        ConnectMain->setCentralWidget(ConWin);
        statusbar = new QStatusBar(ConnectMain);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ConnectMain->setStatusBar(statusbar);

        retranslateUi(ConnectMain);

        QMetaObject::connectSlotsByName(ConnectMain);
    } // setupUi

    void retranslateUi(QMainWindow *ConnectMain)
    {
        ConnectMain->setWindowTitle(QApplication::translate("ConnectMain", "Connections", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConnectMain: public Ui_ConnectMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTION_UI_H
