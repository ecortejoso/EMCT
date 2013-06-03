#ifndef EMCT_TEST_H
#define EMCT_TEST_H

#include "ui_EMCTUI_Prop2.h"
#include "emct.h"


using namespace std;


class emct_main : public QMainWindow, private Ui::EMCT {
    Q_OBJECT
    string user;
    string pswd;
    string dpath;
    string mpath;
    string leu;
    int e_count;
    int i_count;
    int pro_num;
    int optionNum;
    vector<E_machine> elist;
    vector<E_machine> ilist;
    vector<producto> list_products;
    vector<string> leulist;

public:
    emct_main();
    std::string getUser() {return user;}
    std::string getPswd() {return pswd;}
    std::string getDPath() {return dpath;}
    std::string getMPath() {return mpath;}
    std::string getLeu() {return leu;}
    int getEcount() {return e_count;}
    int getIcount() {return i_count;}
    int getProductNum() {return pro_num;}
    int getOptionNum() {return optionNum;}
    vector<E_machine> getEList() {return elist;}
    vector<E_machine> getIList() {return ilist;}
    vector<producto> getPList() {return list_products;}

public slots:
    void closeEvent(QCloseEvent *event);
    void getPath();
    void executeOption();
    void saveLog();
    void helpPdf();
    int readConfigFile();
    void openCon(QWidget *widg);
    void disableButtons();
    void enableButtons();
    void changeEStatus(std::string s);
    int readVersionFile();
    void logAppend(const QString &text);
    void logAppendError(const QString &text);
    void logAppendOp(const QString &text);

signals:
    void exitChild();

};


#endif
