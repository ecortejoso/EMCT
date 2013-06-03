#include <QApplication>
#include "emct.h"
#include "emct_main.h"
#include "connections_window.h"

string getCurrentPath(){
    std::string cwd;
    char buff[PATH_MAX];
    if ( getcwd(buff, PATH_MAX) != 0){
        cwd = string( buff );
    } else {
        cwd = "Error";
    }
    return cwd;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    emct_main *dialog = new emct_main;
    dialog->show();
    return app.exec();
}

