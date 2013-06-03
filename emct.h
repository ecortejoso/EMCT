#ifndef EMCT_H
#define EMCT_H

#include <QtGui>
#include <QApplication>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <dirent.h>
#include <csignal>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

using namespace std;

struct plo_r{
    string ip;
    string user;
    string pswd;
    string data_path;
};

struct E_machine {
    string id;
    string ip_64;
    string ip_32;
    string m64;
    string m32;
    string name;
    string data_path;
    string exe_path;
    QLabel *imageE;
    vector<string> colIds;
    int col_num;
    plo_r plo;
};


struct md5pair{
    string num;
    string path;
};

struct producto{
    string id;
    string version;
    string name;
    vector <md5pair> pro_ids;
};


string getCurrentPath();

#endif
