#ifndef CONNECTIONS_WINDOW_H
#define CONNECTIONS_WINDOW_H

#include "ui_Connection_UI.h"
#include "emct_main.h"
#include "emct.h"

using namespace std;

class connectionsWindow : public QMainWindow, private Ui::ConnectMain {
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

public:
    connectionsWindow(emct_main *parent = 0);
    void setReadInfo(std::string u, std::string p, std::string d, std::string m, int e, int i);
    void setOptionNum(int option);
    void setElist(vector<E_machine> list);
    void setIlist(vector<E_machine> list);
    void setProductInfo(vector<producto> prolist, int num);
    void setLeu(std::string l);

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
    void execeOption(E_machine m);
    void executeAll();
    void prepareExecutionE(int num);
    void prepareExecutionI(int num);
    int updateDC(std::string ipAdd64, std::string ipAdd32, std::string id, std::string name, std::string m_dpath);
    int reboot(std::string s);
    int shutdown(std::string s);
    int updateLEU(std::string ipAdd64, std::string ipAdd32, std::string id, std::string name, std::string m_dpath);
    int updateRBC(std::string ipAdd64, std::string ipAdd32, std::string name, std::string m_dpath);
    void updateLEUaux(std::string ip, std::string filename);
    int displayVersion(std::string ipAdd64, std::string ipAdd32, std::string name, std::string m_mpath);
    int updateExecutables(string ipAdd64, string ipAdd32, std::string name, std::string m_mpath);
    std::string elimPath(std::string s);

signals:
    void sigEnable();
    void textResult(const QString &text);
    void textError(const QString &text);
    void textOp(const QString &text);

};

#endif
