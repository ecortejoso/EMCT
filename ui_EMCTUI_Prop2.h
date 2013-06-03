/********************************************************************************
** Form generated from reading UI file 'EMCTUI_Prop2.ui'
**
** Created: Thu Jul 7 11:54:50 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMCTUI_PROP2_H
#define UI_EMCTUI_PROP2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EMCT
{
public:
    QAction *quit;
    QAction *actionHelp;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *configFile;
    QPushButton *pushButton_Browse;
    QLabel *label_2;
    QPushButton *pushButton_Save;
    QTextBrowser *log;
    QComboBox *chooseOption;
    QPushButton *pushButton_execute;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EMCT)
    {
        if (EMCT->objectName().isEmpty())
            EMCT->setObjectName(QString::fromUtf8("EMCT"));
        EMCT->resize(660, 705);
        quit = new QAction(EMCT);
        quit->setObjectName(QString::fromUtf8("quit"));
        actionHelp = new QAction(EMCT);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        centralwidget = new QWidget(EMCT);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        configFile = new QLineEdit(centralwidget);
        configFile->setObjectName(QString::fromUtf8("configFile"));

        gridLayout->addWidget(configFile, 0, 1, 1, 1);

        pushButton_Browse = new QPushButton(centralwidget);
        pushButton_Browse->setObjectName(QString::fromUtf8("pushButton_Browse"));

        gridLayout->addWidget(pushButton_Browse, 0, 2, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pushButton_Save = new QPushButton(centralwidget);
        pushButton_Save->setObjectName(QString::fromUtf8("pushButton_Save"));

        gridLayout->addWidget(pushButton_Save, 4, 2, 1, 1);

        log = new QTextBrowser(centralwidget);
        log->setObjectName(QString::fromUtf8("log"));

        gridLayout->addWidget(log, 2, 0, 1, 3);

        chooseOption = new QComboBox(centralwidget);
        chooseOption->setObjectName(QString::fromUtf8("chooseOption"));

        gridLayout->addWidget(chooseOption, 1, 1, 1, 2);

        pushButton_execute = new QPushButton(centralwidget);
        pushButton_execute->setObjectName(QString::fromUtf8("pushButton_execute"));

        gridLayout->addWidget(pushButton_execute, 4, 0, 1, 2);

        EMCT->setCentralWidget(centralwidget);
        menubar = new QMenuBar(EMCT);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 660, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        EMCT->setMenuBar(menubar);
        statusBar = new QStatusBar(EMCT);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        EMCT->setStatusBar(statusBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(quit);
        menuHelp->addAction(actionHelp);

        retranslateUi(EMCT);

        QMetaObject::connectSlotsByName(EMCT);
    } // setupUi

    void retranslateUi(QMainWindow *EMCT)
    {
        EMCT->setWindowTitle(QApplication::translate("EMCT", "ERTMS Manager Configuration Tool", 0, QApplication::UnicodeUTF8));
        quit->setText(QApplication::translate("EMCT", "Quit", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("EMCT", "Help...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EMCT", "Configuration File", 0, QApplication::UnicodeUTF8));
        pushButton_Browse->setText(QApplication::translate("EMCT", "Browse...", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EMCT", "Choose Option", 0, QApplication::UnicodeUTF8));
        pushButton_Save->setText(QApplication::translate("EMCT", "Save Log", 0, QApplication::UnicodeUTF8));
        chooseOption->clear();
        chooseOption->insertItems(0, QStringList()
         << QApplication::translate("EMCT", "Update Datacore", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EMCT", "Update LEU", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EMCT", "Update RBC", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EMCT", "Update Modules", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EMCT", "Check Version", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EMCT", "Shutdown Systems", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EMCT", "Reboot Systems", 0, QApplication::UnicodeUTF8)
        );
        pushButton_execute->setText(QApplication::translate("EMCT", "Execute", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("EMCT", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("EMCT", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EMCT: public Ui_EMCT {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMCTUI_PROP2_H
