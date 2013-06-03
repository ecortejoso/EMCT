#include "emct_main.h"
#include "connections_window.h"

using namespace std;

/*
****************************************************************************************************************************

setReadInfo:

This methods set the parameters: user, pswd, dpath and e_count for the connectionsWindow class
as defined in connections_window.h. This is general information provided by the configuration file.


****************************************************************************************************************************
*/

void connectionsWindow::setReadInfo(std::string u, std::string p, std::string d, std::string m, int e, int i){
    user = u;
    pswd = p;
    dpath = d;
    mpath = m;
    e_count = e;
    i_count = i;
}


/*
****************************************************************************************************************************

setOptionNum:

This methods sets the optionNum paramter of the connectionsWindow class as defined in connections_window.h. This parameter
defines the execution option selected by the user.


****************************************************************************************************************************
*/


void connectionsWindow::setOptionNum(int option){
    optionNum = option;
}


/*
****************************************************************************************************************************

setElist:

This methods sets the elist paramter of the connectionsWindow class as defined in connections_window.h. This holds
information regarding the various product machines


****************************************************************************************************************************
*/


void connectionsWindow::setElist(vector<E_machine> list){
    elist = list;
}


/*
****************************************************************************************************************************

setIlist:

This methods sets the ilist paramter of the connectionsWindow class as defined in connections_window.h. This holds
information regarding the various product machines' if2s.


****************************************************************************************************************************
*/


void connectionsWindow::setIlist(vector<E_machine> list){
    ilist = list;
}


/*
****************************************************************************************************************************

setProductInfo:

This methods sets the list_products and pro_num paramters of the connectionsWindow class as defined in connections_window.h.
These work to store information regarding different product information.


****************************************************************************************************************************
*/


void connectionsWindow::setProductInfo(vector<producto> prolist, int num){
    list_products = prolist;
    pro_num = num;
}


/*
****************************************************************************************************************************

setLeu:

This methods sets the leu paramter of the connectionsWindow class as defined in connections_window.h. This stores the LEU
to be updated should the user choose to Update a LEU.


****************************************************************************************************************************
*/


void connectionsWindow::setLeu(std::string l){
    leu = l;
}


/*
****************************************************************************************************************************

elimPath:

This methods simply takes a string designed to be a path for a file and returns the file name by eliminating the
prefixed file path.


****************************************************************************************************************************
*/


string connectionsWindow::elimPath(string s){
    size_t position;
    position = s.rfind("/");
    s.erase(0, position+1);
    return s;
}


/*
****************************************************************************************************************************

connectionsWindow:

This is the constructor method for the class connectionsWindow. It establishes the UI and connections neccesary for usability.
The connections Window is setup to dynamically add buttons and labels based on the configuration file provided by the user,
thus it relies less on the UI design.


****************************************************************************************************************************
*/


connectionsWindow::connectionsWindow(emct_main *parent){

    // This uses the layout .ui file to create the basis of the window. Refer to connectios_window.h.
    setupUi(this);

    // Initialize of QImage variables to "success" and "failure" images
    QImage imageGreen("CheckMark.png");
    QImage imageRed("Minus.png");

    // Declaration of variables
    QSignalMapper *sm;
    QSignalMapper *sm2;
    QVBoxLayout *mainLayout;
    string s;
    QString qstringaux;
    QGroupBox *gbox;
    QVBoxLayout *top;
    QVBoxLayout *lLayout1;
    QVBoxLayout *mlLayout1;
    QVBoxLayout *mrLayout1;
    QVBoxLayout *rLayout1;
    QHBoxLayout *rightLayout1;
    QHBoxLayout *lineLayout1;
    QPushButton *execb1;
    QPushButton *execAll;
    QLabel *ip64;
    QLabel *ip32;
    QLabel *res;
    QLabel *machID;
    QLabel *image1;
    QLabel *image2;
    QLabel *imageExe1;
    QLabel *statusLabel;
    string command;
    string line;
    string mes;
    string found_id;
    string current_path;
    vector<string> vec_ids;
    E_machine run_emachine;
    E_machine run_imachine;
    ifstream file;
    QString Mes;
    int j;
    bool string_in;
    vector<string> vec_aux;

    // Resize images
    imageGreen = imageGreen.scaled((imageGreen.size()*0.2), Qt::KeepAspectRatio);
    imageRed = imageRed.scaled((imageRed.size()*0.2), Qt::KeepAspectRatio);

    // Initializations
    sm = new QSignalMapper(this);
    sm2 = new QSignalMapper(this);
    mainLayout = new QVBoxLayout;
    setReadInfo(parent->getUser(), parent->getPswd(), parent->getDPath(), parent->getMPath(), parent->getEcount(), parent->getIcount());
    setOptionNum(parent->getOptionNum());
    setElist(parent->getEList());
    setIlist(parent->getIList());
    setProductInfo(parent->getPList(), parent->getProductNum());
    setLeu(parent->getLeu());
    statusLabel = new QLabel("Ready...");

    // Set status on this window too "Ready.."
    QMainWindow::statusBar()->addWidget(statusLabel);

    // Connects a custom signal to be able to disable buttons on the main window
    connect( this, SIGNAL( sigEnable() ), parent, SLOT( enableButtons() ) );
    // Connects a custom signals to transmit messages into the main window's log.
    connect( this, SIGNAL(textResult(const QString &)), parent, SLOT(logAppend(const QString &)));
    connect( this, SIGNAL(textError(const QString &)), parent, SLOT(logAppendError(const QString &)));
    connect( this, SIGNAL(textOp(const QString &)), parent, SLOT(logAppendOp(const QString &)));

    // Set status on the main window too "Testing connections.."
    parent->changeEStatus("Testing connections...");



    if(optionNum == 1){

        command = "find "+dpath+"/* -name \"*"+leu+"*\" >> temp.txt";
        if(system(command.c_str()) == 0){
            // Open temporary file
            current_path = getCurrentPath();
            current_path = current_path+"/temp.txt";
            file.open(current_path.c_str());
            while(file.good()){
                file >> line;
                if(line.substr(0,1) == "."){
                    found_id = line.substr(dpath.size()+1, 8);
                    if(vec_ids.empty()){
                        vec_ids.push_back(found_id);
                    } else {
                        string_in = false;
                        for(int i=0; i<(int)vec_ids.size(); i++){
                            if(vec_ids[i] == found_id){
                                string_in = true;
                            }
                        }
                        if(!string_in){
                            vec_ids.push_back(found_id);
                        }
                    }
                }
            }
        }
        command = "rm temp.txt";
        if(system(command.c_str()) == 0){

        }



        for(int i=0; i<(int)vec_ids.size(); i++){
            for(int k=0; k<e_count; k++){
                if(vec_ids[i] == elist[k].id){
                    // Gets the list of colateral machines of the current ERTMS Manager
                    vec_aux = elist[k].colIds;
                    // Loop that runs through colateral list to execute
                    for(int a=0; a<elist[k].col_num; a++){
                        for(int b=0; b<e_count; b++){
                            if(vec_aux[a] == elist[b].id){

                                // Initialize title for the ERTMS
                                s = "ERTMS Machine";
                                qstringaux = s.c_str();
                                gbox = new QGroupBox(qstringaux);

                                // Initialize layouts
                                top = new QVBoxLayout;
                                lLayout1 = new QVBoxLayout;
                                mlLayout1 = new QVBoxLayout;
                                mrLayout1 = new QVBoxLayout;
                                rLayout1 = new QVBoxLayout;
                                rightLayout1 = new QHBoxLayout;
                                lineLayout1 = new QHBoxLayout;

                                // Initialize Labels that display text information
                                s = "IP64";
                                qstringaux = s.c_str();
                                ip64 = new QLabel(qstringaux);
                                s = "IP32";
                                qstringaux = s.c_str();
                                ip32 = new QLabel(qstringaux);
                                s = "Result";
                                qstringaux = s.c_str();
                                res = new QLabel(qstringaux);
                                s = "Machine ID: "+elist[b].id;
                                qstringaux = s.c_str();
                                machID = new QLabel(qstringaux);

                                //Initialize images in red, they will stay red if connection fails
                                image1 = new QLabel;
                                image2 = new QLabel;
                                imageExe1 = new QLabel;
                                image1->setPixmap(QPixmap::fromImage(imageRed));
                                imageExe1->setPixmap(QPixmap::fromImage(imageRed));
                                image2->setPixmap(QPixmap::fromImage(imageRed));
                                elist[b].imageE = imageExe1;

                                command = "plink -l "+user+" -pw "+pswd+" "+elist[b].ip_64+" cd .";
                                if(system(command.c_str()) == 0){ // Successfull connection
                                    // Set image of the corresponding 64-bit machine to green
                                    image1->setPixmap(QPixmap::fromImage(imageGreen));
                                    // Send connection message to main window log
                                    mes = "Connection to "+elist[b].ip_64+" successfull.\n";
                                    Mes = mes.c_str();
                                    emit textResult(Mes);
                                } else{ // Failed connection
                                    mes = "Connection to "+elist[b].ip_64+" failed.\n";
                                    Mes = mes.c_str();
                                    emit textError(Mes);
                                }
                                // Execute a simple operation on the machine to check connection on it's 32-bit machine
                                command = "plink -l "+user+" -pw "+pswd+" "+elist[b].ip_32+" cd .";
                                if(system(command.c_str()) == 0){ // Successfull connection
                                    // Set image of the corresponding 32-bit machine to green
                                    image2->setPixmap(QPixmap::fromImage(imageGreen));
                                    // Send connection message to main window log
                                    mes = "Connection to "+elist[b].ip_32+" successfull.\n";
                                    Mes = mes.c_str();
                                    emit textResult(Mes);
                                } else{ // Failed connection
                                    mes = "Connection to "+elist[b].ip_32+" failed.\n";
                                    Mes = mes.c_str();
                                    emit textError(Mes);
                                }


                                // Window formatting and layout procedures
                                rLayout1->setSpacing(0);
                                mrLayout1->setSpacing(0);
                                mlLayout1->setSpacing(0);
                                mlLayout1->addWidget(ip64);
                                mlLayout1->addWidget(image1);
                                mrLayout1->addWidget(ip32);
                                mrLayout1->addWidget(image2);
                                rLayout1->addWidget(res);
                                rLayout1->addWidget(imageExe1);
                                rightLayout1->addLayout(mlLayout1);
                                rightLayout1->addLayout(mrLayout1);
                                rightLayout1->addLayout(rLayout1);
                                lineLayout1->addWidget(machID);
                                lineLayout1->addStretch();
                                lineLayout1->addLayout(rightLayout1);

                                // Final layout procedures for this particular instance of ERTMS within the loop
                                top->addLayout(lineLayout1);
                                gbox->setLayout(top);
                                mainLayout->addWidget(gbox);
                            }
                        }
                    }
                }
            }
        }

        execAll = new QPushButton("Update LEU on all relevant machines", this);
        connect( execAll, SIGNAL( clicked() ), this, SLOT( executeAll() ) );
        mainLayout->addWidget(execAll);

        scrollSpace->setLayout(mainLayout);

    } else if(optionNum == 2){

        command = "find "+dpath+"/* -name \"*"+leu+"*\" >> temp.txt";
        if(system(command.c_str()) == 0){
            // Open temporary file
            current_path = getCurrentPath();
            current_path = current_path+"/temp.txt";
            file.open(current_path.c_str());
            while(file.good()){
                file >> line;
                if(line.substr(0,1) == "."){
                    found_id = line.substr(dpath.size()+1, 8);
                    if(vec_ids.empty()){
                        vec_ids.push_back(found_id);
                    } else {
                        string_in = false;
                        for(int i=0; i<(int)vec_ids.size(); i++){
                            if(vec_ids[i] == found_id){
                                string_in = true;
                            }
                        }
                        if(!string_in){
                            vec_ids.push_back(found_id);
                        }
                    }
                }
            }
        }
        command = "rm temp.txt";
        if(system(command.c_str()) == 0){

        }


        for(int k=0; k<i_count; k++){
            if(ilist[k].id == leu){

                // Initialize title for the ERTMS
                s = "IF2 Machine";
                qstringaux = s.c_str();
                gbox = new QGroupBox(qstringaux);

                // Initialize layouts
                top = new QVBoxLayout;
                lLayout1 = new QVBoxLayout;
                mlLayout1 = new QVBoxLayout;
                mrLayout1 = new QVBoxLayout;
                rLayout1 = new QVBoxLayout;
                rightLayout1 = new QHBoxLayout;
                lineLayout1 = new QHBoxLayout;


                // Initialize Labels that display text information
                s = "IP64";
                qstringaux = s.c_str();
                ip64 = new QLabel(qstringaux);
                s = "IP32";
                qstringaux = s.c_str();
                ip32 = new QLabel(qstringaux);
                s = "Result";
                qstringaux = s.c_str();
                res = new QLabel(qstringaux);
                s = "Machine ID: "+ilist[k].id;
                qstringaux = s.c_str();
                machID = new QLabel(qstringaux);

                //Initialize images in red, they will stay red if connection fails
                image1 = new QLabel;
                image2 = new QLabel;
                imageExe1 = new QLabel;
                image1->setPixmap(QPixmap::fromImage(imageRed));
                imageExe1->setPixmap(QPixmap::fromImage(imageRed));
                image2->setPixmap(QPixmap::fromImage(imageRed));
                ilist[k].imageE = imageExe1;

                command = "plink -l "+user+" -pw "+pswd+" "+ilist[k].ip_64+" cd .";
                if(system(command.c_str()) == 0){ // Successfull connection
                    // Set image of the corresponding 64-bit machine to green
                    image1->setPixmap(QPixmap::fromImage(imageGreen));
                    // Send connection message to main window log
                    mes = "Connection to "+ilist[k].ip_64+" successfull.\n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                } else{ // Failed connection
                    mes = "Connection to "+ilist[k].ip_64+" failed.\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }
                // Execute a simple operation on the machine to check connection on it's 32-bit machine
                command = "plink -l "+user+" -pw "+pswd+" "+ilist[k].ip_32+" cd .";
                if(system(command.c_str()) == 0){ // Successfull connection
                    // Set image of the corresponding 32-bit machine to green
                    image2->setPixmap(QPixmap::fromImage(imageGreen));
                    // Send connection message to main window log
                    mes = "Connection to "+ilist[k].ip_32+" successfull.\n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                } else{ // Failed connection
                    mes = "Connection to "+ilist[k].ip_32+" failed.\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }


                // Window formatting and layout procedures
                rLayout1->setSpacing(0);
                mrLayout1->setSpacing(0);
                mlLayout1->setSpacing(0);
                mlLayout1->addWidget(ip64);
                mlLayout1->addWidget(image1);
                mrLayout1->addWidget(ip32);
                mrLayout1->addWidget(image2);
                rLayout1->addWidget(res);
                rLayout1->addWidget(imageExe1);
                rightLayout1->addLayout(mlLayout1);
                rightLayout1->addLayout(mrLayout1);
                rightLayout1->addLayout(rLayout1);
                lineLayout1->addWidget(machID);
                lineLayout1->addStretch();
                lineLayout1->addLayout(rightLayout1);
                // Final layout procedures for this particular instance of ERTMS within the loop
                top->addLayout(lineLayout1);
                gbox->setLayout(top);
                mainLayout->addWidget(gbox);

            }
        }



        for(int i=0; i<(int)vec_ids.size(); i++){
            for(int k=0; k<e_count; k++){
                if(vec_ids[i] == elist[k].id){
                    // Gets the list of colateral machines of the current ERTMS Manager


                    // Initialize title for the ERTMS
                    s = "ERTMS Machine";
                    qstringaux = s.c_str();
                    gbox = new QGroupBox(qstringaux);

                    // Initialize layouts
                    top = new QVBoxLayout;
                    lLayout1 = new QVBoxLayout;
                    mlLayout1 = new QVBoxLayout;
                    mrLayout1 = new QVBoxLayout;
                    rLayout1 = new QVBoxLayout;
                    rightLayout1 = new QHBoxLayout;
                    lineLayout1 = new QHBoxLayout;

                    // Initialize Labels that display text information
                    s = "IP64";
                    qstringaux = s.c_str();
                    ip64 = new QLabel(qstringaux);
                    s = "IP32";
                    qstringaux = s.c_str();
                    ip32 = new QLabel(qstringaux);
                    s = "Result";
                    qstringaux = s.c_str();
                    res = new QLabel(qstringaux);
                    s = "Machine ID: "+elist[k].id;
                    qstringaux = s.c_str();
                    machID = new QLabel(qstringaux);

                    //Initialize images in red, they will stay red if connection fails
                    image1 = new QLabel;
                    image2 = new QLabel;
                    imageExe1 = new QLabel;
                    image1->setPixmap(QPixmap::fromImage(imageRed));
                    imageExe1->setPixmap(QPixmap::fromImage(imageRed));
                    image2->setPixmap(QPixmap::fromImage(imageRed));
                    elist[k].imageE = imageExe1;

                    command = "plink -l "+user+" -pw "+pswd+" "+elist[k].ip_64+" cd .";
                    if(system(command.c_str()) == 0){ // Successfull connection
                        // Set image of the corresponding 64-bit machine to green
                        image1->setPixmap(QPixmap::fromImage(imageGreen));
                        // Send connection message to main window log
                        mes = "Connection to "+elist[k].ip_64+" successfull.\n";
                        Mes = mes.c_str();
                        emit textResult(Mes);
                    } else{ // Failed connection
                        mes = "Connection to "+elist[k].ip_64+" failed.\n";
                        Mes = mes.c_str();
                        emit textError(Mes);
                    }
                    // Execute a simple operation on the machine to check connection on it's 32-bit machine
                    command = "plink -l "+user+" -pw "+pswd+" "+elist[k].ip_32+" cd .";
                    if(system(command.c_str()) == 0){ // Successfull connection
                        // Set image of the corresponding 32-bit machine to green
                        image2->setPixmap(QPixmap::fromImage(imageGreen));
                        // Send connection message to main window log
                        mes = "Connection to "+elist[k].ip_32+" successfull.\n";
                        Mes = mes.c_str();
                        emit textResult(Mes);
                    } else{ // Failed connection
                        mes = "Connection to "+elist[k].ip_32+" failed.\n";
                        Mes = mes.c_str();
                        emit textError(Mes);
                    }


                    // Window formatting and layout procedures
                    rLayout1->setSpacing(0);
                    mrLayout1->setSpacing(0);
                    mlLayout1->setSpacing(0);
                    mlLayout1->addWidget(ip64);
                    mlLayout1->addWidget(image1);
                    mrLayout1->addWidget(ip32);
                    mrLayout1->addWidget(image2);
                    rLayout1->addWidget(res);
                    rLayout1->addWidget(imageExe1);
                    rightLayout1->addLayout(mlLayout1);
                    rightLayout1->addLayout(mrLayout1);
                    rightLayout1->addLayout(rLayout1);
                    lineLayout1->addWidget(machID);
                    lineLayout1->addStretch();
                    lineLayout1->addLayout(rightLayout1);

                    // Final layout procedures for this particular instance of ERTMS within the loop
                    top->addLayout(lineLayout1);
                    gbox->setLayout(top);
                    mainLayout->addWidget(gbox);

                }
            }
        }

        execAll = new QPushButton("Update RBC on all relevant machines", this);
        connect( execAll, SIGNAL( clicked() ), this, SLOT( executeAll() ) );
        mainLayout->addWidget(execAll);

        scrollSpace->setLayout(mainLayout);

    } else {

        // Loop that creates information pertaining to each ERTMS
        j=0;
        while(j<e_count){

            // Initialize title for the ERTMS
            s = "ERTMS Machine";
            qstringaux = s.c_str();
            gbox = new QGroupBox(qstringaux);

            // Initialize layouts
            top = new QVBoxLayout;
            lLayout1 = new QVBoxLayout;
            mlLayout1 = new QVBoxLayout;
            mrLayout1 = new QVBoxLayout;
            rLayout1 = new QVBoxLayout;
            rightLayout1 = new QHBoxLayout;
            lineLayout1 = new QHBoxLayout;

            // Initialize button to execute
            execb1 = new QPushButton("Execute", this);

            // Initialize Labels that display text information
            s = "IP64";
            qstringaux = s.c_str();
            ip64 = new QLabel(qstringaux);
            s = "IP32";
            qstringaux = s.c_str();
            ip32 = new QLabel(qstringaux);
            s = "Result";
            qstringaux = s.c_str();
            res = new QLabel(qstringaux);
            s = "Machine ID: "+elist[j].id;
            qstringaux = s.c_str();
            machID = new QLabel(qstringaux);

            //Initialize images in red, they will stay red if connection fails
            image1 = new QLabel;
            image2 = new QLabel;
            imageExe1 = new QLabel;
            image1->setPixmap(QPixmap::fromImage(imageRed));
            imageExe1->setPixmap(QPixmap::fromImage(imageRed));
            image2->setPixmap(QPixmap::fromImage(imageRed));
            elist[j].imageE = imageExe1;


            // Execute a simple operation on the machine to check connection on it's 64-bit machine
            command = "plink -l "+user+" -pw "+pswd+" "+elist[j].ip_64+" cd .";
            if(system(command.c_str()) == 0){ // Successfull connection
                // Set image of the corresponding 64-bit machine to green
                image1->setPixmap(QPixmap::fromImage(imageGreen));
                // Send connection message to main window log
                mes = "Connection to "+elist[j].ip_64+" successfull.\n";
                Mes = mes.c_str();
                emit textResult(Mes);
            } else{ // Failed connection
                // Deactivate execution button pertaining to the ERTMS
                execb1->setEnabled(false);
                mes = "Connection to "+elist[j].ip_64+" failed.\n";
                Mes = mes.c_str();
                emit textError(Mes);
            }

            // Execute a simple operation on the machine to check connection on it's 32-bit machine
            command = "plink -l "+user+" -pw "+pswd+" "+elist[j].ip_32+" cd .";
            if(system(command.c_str()) == 0){ // Successfull connection
                // Set image of the corresponding 32-bit machine to green
                image2->setPixmap(QPixmap::fromImage(imageGreen));
                // Send connection message to main window log
                mes = "Connection to "+elist[j].ip_32+" successfull.\n";
                Mes = mes.c_str();
                emit textResult(Mes);
            } else{ // Failed connection
                // Deactivate execution button pertaining to the ERTMS
                execb1->setEnabled(false);
                // Send connection message to main window log
                mes = "Connection to "+elist[j].ip_32+" failed.\n";
                Mes = mes.c_str();
                emit textError(Mes);
            }

            // Window formatting and layout procedures
            rLayout1->setSpacing(0);
            lLayout1->setSpacing(0);
            mrLayout1->setSpacing(0);
            mlLayout1->setSpacing(0);
            mlLayout1->addWidget(ip64);
            mlLayout1->addWidget(image1);
            mrLayout1->addWidget(ip32);
            mrLayout1->addWidget(image2);
            rLayout1->addWidget(res);
            rLayout1->addWidget(imageExe1);
            lLayout1->addWidget(execb1);
            rightLayout1->addLayout(lLayout1);
            rightLayout1->addLayout(mlLayout1);
            rightLayout1->addLayout(mrLayout1);
            rightLayout1->addLayout(rLayout1);
            lineLayout1->addWidget(machID);
            lineLayout1->addStretch();
            lineLayout1->addLayout(rightLayout1);

            // Sets up the connection to execute the chosen option when the button is clicked
            // A mapping is needed to send a parameter (an integer) via this connection
            sm->setMapping(execb1, (j));
            connect( execb1, SIGNAL( clicked() ), sm, SLOT( map() ) );


            // Final layout procedures for this particular instance of ERTMS within the loop
            top->addLayout(lineLayout1);
            gbox->setLayout(top);
            mainLayout->addWidget(gbox);
            j++;

        }


        if(optionNum != 0){
            j=0;
            while(j<i_count){

                // Initialize title for the ERTMS
                s = "IF2 Machine";
                qstringaux = s.c_str();
                gbox = new QGroupBox(qstringaux);

                // Initialize layouts
                top = new QVBoxLayout;
                lLayout1 = new QVBoxLayout;
                mlLayout1 = new QVBoxLayout;
                mrLayout1 = new QVBoxLayout;
                rLayout1 = new QVBoxLayout;
                rightLayout1 = new QHBoxLayout;
                lineLayout1 = new QHBoxLayout;

                // Initialize button to execute
                execb1 = new QPushButton("Execute", this);

                // Initialize Labels that display text information
                s = "IP64";
                qstringaux = s.c_str();
                ip64 = new QLabel(qstringaux);
                s = "IP32";
                qstringaux = s.c_str();
                ip32 = new QLabel(qstringaux);
                s = "Result";
                qstringaux = s.c_str();
                res = new QLabel(qstringaux);
                s = "IF2 ID: "+ilist[j].id;
                qstringaux = s.c_str();
                machID = new QLabel(qstringaux);

                //Initialize images in red, they will stay red if connection fails
                image1 = new QLabel;
                image2 = new QLabel;
                imageExe1 = new QLabel;
                image1->setPixmap(QPixmap::fromImage(imageRed));
                imageExe1->setPixmap(QPixmap::fromImage(imageRed));
                image2->setPixmap(QPixmap::fromImage(imageRed));
                ilist[j].imageE = imageExe1;


                // Execute a simple operation on the machine to check connection on it's 64-bit machine
                command = "plink -l "+user+" -pw "+pswd+" "+ilist[j].ip_64+" cd .";
                if(system(command.c_str()) == 0){ // Successfull connection
                    // Set image of the corresponding 64-bit machine to green
                    image1->setPixmap(QPixmap::fromImage(imageGreen));
                    // Send connection message to main window log
                    mes = "Connection to "+ilist[j].ip_64+" successfull.\n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                } else{ // Failed connection
                    // Deactivate execution button pertaining to the ERTMS
                    execb1->setEnabled(false);
                    mes = "Connection to "+ilist[j].ip_64+" failed.\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }

                // Execute a simple operation on the machine to check connection on it's 32-bit machine
                command = "plink -l "+user+" -pw "+pswd+" "+ilist[j].ip_32+" cd .";
                if(system(command.c_str()) == 0){ // Successfull connection
                    // Set image of the corresponding 32-bit machine to green
                    image2->setPixmap(QPixmap::fromImage(imageGreen));
                    // Send connection message to main window log
                    mes = "Connection to "+ilist[j].ip_32+" successfull.\n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                } else{ // Failed connection
                    // Deactivate execution button pertaining to the ERTMS
                    execb1->setEnabled(false);
                    // Send connection message to main window log
                    mes = "Connection to "+ilist[j].ip_32+" failed.\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }

                // Window formatting and layout procedures
                rLayout1->setSpacing(0);
                lLayout1->setSpacing(0);
                mrLayout1->setSpacing(0);
                mlLayout1->setSpacing(0);
                mlLayout1->addWidget(ip64);
                mlLayout1->addWidget(image1);
                mrLayout1->addWidget(ip32);
                mrLayout1->addWidget(image2);
                rLayout1->addWidget(res);
                rLayout1->addWidget(imageExe1);
                lLayout1->addWidget(execb1);
                rightLayout1->addLayout(lLayout1);
                rightLayout1->addLayout(mlLayout1);
                rightLayout1->addLayout(mrLayout1);
                rightLayout1->addLayout(rLayout1);
                lineLayout1->addWidget(machID);
                lineLayout1->addStretch();
                lineLayout1->addLayout(rightLayout1);

                // Sets up the connection to execute the chosen option when the button is clicked
                // A mapping is needed to send a parameter (an integer) via this connection
                sm2->setMapping(execb1, (j));
                connect( execb1, SIGNAL( clicked() ), sm2, SLOT( map() ) );

                // Final layout procedures for this particular instance of ERTMS within the loop
                top->addLayout(lineLayout1);
                gbox->setLayout(top);
                mainLayout->addWidget(gbox);
                j++;
            }

        connect(sm2, SIGNAL(mapped(int)), this, SLOT(prepareExecutionI(int)));

        }


        // Create and connect a final button, that on click will execute the chosen option
        // On all machines.
        execAll = new QPushButton("Execute on all machines", this);
        connect( execAll, SIGNAL( clicked() ), this, SLOT( executeAll() ) );
        mainLayout->addWidget(execAll);

        // Second part that connects all buttons to their particular instance and sends their respective functions
        connect(sm, SIGNAL(mapped(int)), this, SLOT(prepareExecutionE(int)));


        // Sets the main Layout
        scrollSpace->setLayout(mainLayout);

    }



}


/*
****************************************************************************************************************************

closeEvent:

This rewrites the handeling of the event of closing the window via the close() function. It erases current values so that
the tool may preform further tasks without hinderance.


****************************************************************************************************************************
*/


void connectionsWindow::closeEvent(QCloseEvent *event){
    elist.clear();
    e_count = 0;
    event->accept();
    emit sigEnable();

}


/*
****************************************************************************************************************************

reboot:

This simply reboots the ERTMS 32-bit machine using ERTMS commands sent through the established connection.


****************************************************************************************************************************
*/


int connectionsWindow::reboot(string ipAdd){

    // Variable Declaration
    string command;
    string mes;
    QString Mes;

    // Run ERTMS Manager stop command on the 32-bit machine
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd+" gr stop";
    if(system(command.c_str()) == 0){
        // Run ERTMS Manager "clear TSR" command on the 32-bit machine
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd+" gr clear_tsr";
        if(system(command.c_str()) == 0){
            // Run ERTMS Manager start command on the 32-bit machine
            command = "plink -l "+user+" -pw "+pswd+" "+ipAdd+" gr start &";
            if(system(command.c_str()) == 0){
                // Successfull calls handling
                mes = "System Reboot at "+ipAdd+" successfull.\n";
                Mes = mes.c_str();
                emit textResult(Mes);
                return 0;
            } else {
                // Start failure
                mes = "System start operation at "+ipAdd+" unsuccessfull.\n";
                Mes = mes.c_str();
                emit textError(Mes);
                return 1;
            }
        } else {
            // Clear TSR failure
            mes = "Clear TSR operation at "+ipAdd+" unsuccessfull.\n";
            Mes = mes.c_str();
            emit textError(Mes);
            return 1;

        }
    } else {
        // Stop failure
        mes = "Stop operation at "+ipAdd+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return 1;
    }

}

/*
****************************************************************************************************************************

shutdown:

This simply shuts down the ERTMS 32-bit machine using ERTMS commands sent through the established connection.


****************************************************************************************************************************
*/


int connectionsWindow::shutdown(string ipAdd){

    // Variable Declaration
    string command;
    string mes;
    QString Mes;

    // Run ERTMS Manager stop command on the 32-bit machine
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd+" gr stop";
    if(system(command.c_str()) == 0){
        // Run ERTMS Manager "clear TSR" command on the 32-bit machine
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd+" gr clear_tsr";
        if(system(command.c_str()) == 0){
            // Successfull calls handling
            mes = "System shutdown at "+ipAdd+" successfull.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
            return 0;
        } else {
            // Clear TSR failure
            mes = "Clear TSR operation at "+ipAdd+" unsuccessfull.\n";
            Mes = mes.c_str();
            emit textError(Mes);
            return 1;
        }
    } else {
        // Stop failure
        mes = "Stop operation at "+ipAdd+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return 1;
    }
}


/*
****************************************************************************************************************************

updateDC:

This function updates DATACORE files relating to the selected ERTMS Manager machine.


****************************************************************************************************************************
*/


int connectionsWindow::updateDC(string ipAdd64, string ipAdd32, string id, string name, string m_dpath){

    // Variable Declaration
    vector<md5pair> md5s;
    vector<md5pair> md5s32;
    vector<md5pair> md5s64;
    vector<string> vec_aux;
    ifstream file;
    ifstream file1;
    ifstream file2;
    string line;
    string command;
    string mes;
    string current_path;
    QString Mes;
    bool not_vec;
    plo_r ertms_plo;

    // Send Operation message to log
    mes = "******************************************************************************\n\n          Update Datacore for "+name+" \n\n******************************************************************************\n";
    Mes = mes.c_str();
    emit textOp(Mes);


    // Run ERTMS Manager stop command on the 32-bit machine
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr stop";
    if(system(command.c_str()) == 0){
        // Run ERTMS Manager "clear TSR" command on the 32-bit machine
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr clear_tsr";
        if(system(command.c_str()) == 0){
            // Successfull calls handling
            mes = "System shutdown at "+ipAdd32+" successfull.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
        } else {
            // Clear TSR failure
            mes = "Clear TSR operation at "+ipAdd32+" unsuccessfull.\n";
            Mes = mes.c_str();
            emit textError(Mes);
            return 1;
        }
    } else {
        // Stop failure
        mes = "Stop operation at "+ipAdd32+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return 1;
    }

    // Calculate MD5 for all Datacore files that will be uploaded to the ERTMS Manager
    // and put results in a temporary file
    for(int i=0; i<e_count; i++){
        if (elist[i].id == id){
            // Gets the list of colateral machines of the current ERTMS Manager
            vec_aux = elist[i].colIds;
            // Loop that runs through colateral list to execute
            for(int j=0; j<elist[i].col_num; j++){
                command = "find "+dpath+"/"+vec_aux[j]+"/* -exec md5sum {} \\; >> temp.txt";
                if(system(command.c_str()) == 0){
                    //Do Nothing
                }
            }
        }
    }

    // Open temporary file
    current_path = getCurrentPath();
    current_path = current_path+"/temp.txt";
    file.open(current_path.c_str());
    if (file.is_open()){
        // While the file is open, copy each line into the specified vector using the provided md5pair structure
        while(file.good()){
            md5pair newmd5;
            file >> line;
            newmd5.num = line;
            file >> line;
            newmd5.path = line;
            md5s.push_back(newmd5);
        }
        file.close();
    } else {
        // Do Nothing
    }

    // Immediately delete the temporary file
    command = "rm temp.txt";
    if(system(command.c_str()) != 0){
        return -1;
    } else {
        // Do Nothing
    }


    // Create backup directories. Will fail if they already exist.
    command = "mkdir ./BACKUP/"+name+"/";
    if(system(command.c_str()) == 0){
        command = "mkdir ./BACKUP/"+name+"/64/";
        if(system(command.c_str()) == 0){
            command = "mkdir ./BACKUP/"+name+"/32/";
            if(system(command.c_str()) == 0){
                // Do Nothing
            } else {
                // Do Nothing
            }
        } else {
            // Do Nothing
        }
    } else {
        // Do Nothing
    }


    // Backs up files from the 64-bit machine it it's corresponding folder
    command = "pscp -l "+user+" -pw "+pswd+" -r "+user+"@"+ipAdd64+":"+m_dpath+"/* ./BACKUP/"+name+"/64";
    if(system(command.c_str()) == 0){
        // Backs up files from the 32-bit machine it it's corresponding folder
        command = "pscp -l "+user+" -pw "+pswd+" -r "+user+"@"+ipAdd32+":"+m_dpath+"/* ./BACKUP/"+name+"/32";
        if(system(command.c_str()) != 0){
            mes = "Error occurred backing up DATACORE files of the ERTMS machine at "+ipAdd32+".\n";
            Mes = mes.c_str();
            emit textError(Mes);
            return -1;
        } else {
            // Do Nothing
        }
    } else {
        // Error Message sent to main window log
        mes = "Error occurred backing up DATACORE files of the ERTMS machine at "+ipAdd64+".\n";
        Mes = mes.c_str();
        emit textError(Mes);
    }

    //Erase all DATACORE files from the 32-bit machine
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'rm -r "+m_dpath+"/*/*\'";
    if(system(command.c_str()) == 0){
        // Message sent to main window log
        mes = "DATACORE files erased at "+ipAdd32+"\n";
        Mes = mes.c_str();
        emit textResult(Mes);
        //Erase all DATACORE files from the 64-bit machine
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'rm -r "+m_dpath+"/*/*\'";
        if(system(command.c_str()) == 0){
            // Message sent to main window log
            mes = "DATACORE files erased at "+ipAdd64+"\n";
            Mes = mes.c_str();
            emit textResult(Mes);
        } else {
            // Error Message sent to main window log
            mes = "Error occurred erasing DATACORE files of the ERTMS machine at "+ipAdd64+".\n";
            Mes = mes.c_str();
            emit textError(Mes);
        }
    } else {
        // Error Message sent to main window log
        mes = "Error occurred erasing DATACORE files of the ERTMS machine at "+ipAdd32+".\n";
        Mes = mes.c_str();
        emit textError(Mes);
    }


    // Loop that sends local updated DATACORE files to the ERTMS machine and it's collaterals
    for(int i=0; i<e_count; i++){
        if (elist[i].id == id){
            // Gets the list of colateral machines of the current ERTMS Manager
            vec_aux = elist[i].colIds;
            ertms_plo = elist[i].plo;
            // Loop that runs through colateral list to execute
            for(int j=0; j<elist[i].col_num; j++){
                // Send local updated DATACORE files to the 64-bit machine
                command = "pscp -l "+user+" -pw "+pswd+" -r "+dpath+"/"+vec_aux[j]+"/* "+user+"@"+ipAdd64+":"+m_dpath+"/"+vec_aux[j]+"/";
                if(system(command.c_str()) == 0){
                    // Message sent to main window log
                    mes = "DATACORE files updated at "+ipAdd64+" for machine ID: "+vec_aux[j]+".\n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                    // Send local updated DATACORE files to the 32-bit machine
                    command = "pscp -l "+user+" -pw "+pswd+" -r "+dpath+"/"+vec_aux[j]+"/* "+user+"@"+ipAdd32+":"+m_dpath+"/"+vec_aux[j]+"/";
                    if(system(command.c_str()) == 0){
                        // Message sent to main window log
                        mes = "DATACORE files updated at "+ipAdd32+" for machine ID: "+vec_aux[j]+".\n";
                        Mes = mes.c_str();
                        emit textResult(Mes);
                    } else{
                        // Error Message sent to main window log
                        mes = "Error occurred updating DATACORE files of the ERTMS machine at "+ipAdd32+" for machine ID:"+vec_aux[j]+".\n";
                        Mes = mes.c_str();
                        emit textError(Mes);
                    }
                } else {
                    // Error Message sent to main window log
                    mes = "Error occurred updating DATACORE files of the ERTMS machine at "+ipAdd64+" for machine ID:"+vec_aux[j]+".\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }

                // Mounts the corresponding PLO machine to the local machine (Needs to be done through root)
                command = "mount -t cifs //"+ertms_plo.ip+"/C$ /mnt -o username="+ertms_plo.user+",password="+ertms_plo.pswd;
                if(system(command.c_str()) == 0){
                    // Erases everything in the collaterals directory
                    command = "rm -r /mnt"+ertms_plo.data_path+"/"+vec_aux[j]+"/*";
                    if(system(command.c_str()) == 0){
                        // Copies specified files to collateral directory
                        command = "cp -r "+dpath+"/"+vec_aux[j]+"/* /mnt"+ertms_plo.data_path+"/"+vec_aux[j]+"/";
                        if(system(command.c_str()) == 0){
                            mes = "PLO_R files updated for "+id+".\n";
                            Mes = mes.c_str();
                            emit textResult(Mes);
                            // Unmounts directory
                            command = "umount /mnt";
                            if(system(command.c_str()) != 0){
                                mes = "Error occurred unmounting the PLO_R for "+id+".\n";
                                Mes = mes.c_str();
                                emit textError(Mes);
                            } else {
                                // Do nothing
                            }
                        } else{
                            mes = "Error updating files to the PLO_R for "+id+".\n";
                            Mes = mes.c_str();
                            emit textError(Mes);
                        }
                    } else {
                        mes = "Error occurred deleting outdated files of the PLO_R for "+id+".\n";
                        Mes = mes.c_str();
                        emit textError(Mes);
                    }
                } else {
                    mes = "Error occurred mounting the PLO_R for "+id+".\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }
            }
        }
    }

    // Attempt to start the ERTMS Manager
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr start &";
    if(system(command.c_str()) != 0){
        mes = "System start operation at "+ipAdd32+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    } else {
        mes = "System start operation at "+ipAdd32+" successfull.\n";
        Mes = mes.c_str();
        emit textResult(Mes);
    }


    // Calculate new MD5 of files located on the 64-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'find "+m_dpath+"/*/* -exec md5sum {} \\; >> temp1.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd64+":./temp1.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/temp1.txt";
            file1.open(current_path.c_str());
            if (file1.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file1.good()){
                    md5pair newmd5;
                    file1 >> line;
                    newmd5.num = line;
                    file1 >> line;
                    line.erase(0, 1);
                    newmd5.path = line;
                    md5s64.push_back(newmd5);
                }
                file1.close();
            } else {
                // Do Nothing
            }
        } else {
            // Do Nothing
        }
    } else {
        mes = "Error occurred calculating the MD5 of files of the ERTMS machine at "+ipAdd64+".\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }

    // Calculate new MD5 of files located on the 32-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'find "+m_dpath+"/*/* -exec md5sum {} \\; >> temp2.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd32+":./temp2.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/temp2.txt";
            file2.open(current_path.c_str());
            if (file2.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file2.good()){
                    md5pair newmd5;
                    file2 >> line;
                    newmd5.num = line;
                    file2 >> line;
                    line.erase(0, 1);
                    newmd5.path = line;
                    md5s32.push_back(newmd5);
                }
                file2.close();
            } else {
                // Do Nothing
            }

        } else {
            // Do Nothing
        }
    } else {
        mes = "Error occurred calculating the MD5 of files of the ERTMS machine at "+ipAdd32+".\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }


    // Deletes all temporary files
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" rm temp1.txt";
    if(system(command.c_str()) == 0){
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" rm temp2.txt";
        if(system(command.c_str()) == 0){
            command = "rm temp1.txt";
            if(system(command.c_str()) == 0){
                command = "rm temp2.txt";
                if(system(command.c_str()) != 0){
                    return -1;
                } else {
                    // Do Nothing
                }
            } else {
                mes = "Error occurred attempting to delete temporary files.\n";
                Mes = mes.c_str();
                emit textError(Mes);
                return -1;
            }
        } else {
            mes = "Error occurred attempting to delete temporary files.\n";
            Mes = mes.c_str();
            emit textError(Mes);
            return -1;
        }
    } else {
        mes = "Error occurred attempting to delete temporary files.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }

    // Loop to compare machines MD5 results with local files that were transfered
    for(int i=0; i<(int)md5s.size()-1; i++){
        not_vec = false;
        // Loop to run through results of the 32-bit machine for comparison
        for(int j=0; j<(int)md5s32.size()-1; j++){
            // Checks if two file names are equal
            if(md5s[i].path == md5s32[j].path){
                not_vec = true;
                // Should the names be equal, their number should be as well
                if(md5s[i].num != md5s32[j].num){
                    // Error Message sent to main window log
                    mes = "MD5 check unsuccessfull, "+md5s[i].path+" (local file) has a different MD5 Hash than"+ md5s32[j].path+" on"+ipAdd32+". \n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                    return -1;
                }
            }

        }
        // Checks if a local file was found or not on the machine (not_vec is false if the file WASNT found)
        if(!not_vec){
            // Error Message sent to main window log
            mes = "MD5 check unsuccessfull, found a DATACORE file not matching the specified files for update.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
            return -1;
        }
        not_vec = false;
        // Loop to run through results of the 64-bit machine for comparison
        for(int k=0; k<(int)md5s64.size()-1; k++){
            // Checks if two file names are equal
            if(md5s[i].path == md5s64[k].path){
                not_vec = true;
                // Should the names be equal, their number should be as well
                if(md5s[i].num != md5s64[k].num){
                    // Error Message sent to main window log
                    mes = "MD5 check unsuccessfull, "+md5s[i].path+" (local file) has a different MD5 Hash than"+ md5s64[k].path+" on"+ipAdd64+". \n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                    return -1;
                }
            }
        }
        if(!not_vec){
            // Error Message sent to main window log
            mes = "MD5 check unsuccessfull, found a DATACORE file not matching the specified files for update.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
            return -1;
        }

    }

    // Message sent to main window log
    mes = "MD5 Check successfull. \n\nDatacores updated at "+ipAdd32+" successfully.\n";
    Mes = mes.c_str();
    emit textResult(Mes);



    return 0;

}


/*
****************************************************************************************************************************

updateLEUaux:

This function simply backs up and erases a specified LEU file.


****************************************************************************************************************************
*/


void connectionsWindow::updateLEUaux(string ip, string filename){

    // Variable Declaration
    string command;
    string mes;
    QString Mes;

    // Create backup directories should they not exist yet. Ignores outcome of call because directory may already exist.
    command = "mkdir ./BACKUP/LEU/";
    if(system(command.c_str()) == 0){
        command = "mkdir ./BACKUP/LEU/"+leu+"/";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            // Do Nothing
        }
    } else {
        command = "mkdir ./BACKUP/LEU/"+leu+"/";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            // Do Nothing
        }
    }

    // Backup LEU file in question
    command = "pscp -l "+user+" -pw "+pswd+" -r "+user+"@"+ip+":"+filename+" ./BACKUP/LEU/"+leu+"/";
    if(system(command.c_str()) == 0){
        // Erase "outdated: copy on Manager machine
        command = "plink -l "+user+" -pw "+pswd+" "+ip+" rm "+filename;
        if(system(command.c_str()) == 0){
            mes = "Obsolete LEU removal of "+filename+" at "+ip+" successfull.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
        } else {
            mes = "Obsolete LEU removal of "+filename+" at "+ip+" successfull.\n";
            Mes = mes.c_str();
            emit textError(Mes);
        }
    } else {
        mes = "LEU Backup of "+filename+" at "+ip+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
    }

}


/*
****************************************************************************************************************************

updateLEU:

This function updates all files associated to a specified LEU.


****************************************************************************************************************************
*/


int connectionsWindow::updateLEU(string ipAdd64, string ipAdd32, string id, string name, string m_dpath){

    // Variable Declaration
    string aux_path;
    vector<md5pair> md5s;
    vector<md5pair> md5s32;
    vector<md5pair> md5s64;
    ifstream file;
    ifstream file1;
    ifstream file2;
    string line;
    string command;
    string mes;
    string found_id;
    string current_path;
    string last_line;
    vector<string> vec_ids;
    vector<string> vec_aux;
    QString Mes;
    bool not_vec;
    plo_r ertms_plo;

    mes = "******************************************************************************\n\n          Update LEU "+leu+" for "+name+" \n\n******************************************************************************\n";
    Mes = mes.c_str();
    emit textOp(Mes);

    // Shutdown system to be modified
    if(shutdown(ipAdd32) == 0){
        // Do nothing
    } else {
        mes = "Shutdown at "+ipAdd32+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }

    // Retrieve PLO information from the specified machine
    for(int i=0; i<e_count; i++){
        if(elist[i].id == id){
            ertms_plo = elist[i].plo;
        }
    }

    // Calculate MD5 for all Datacore files that will be uploaded to the ERTMS Manager
    // and put results in a temporary file
    command = "find "+dpath+"/*/* -name \"*"+leu+"*\" -exec md5sum {} \\; >> temp.txt";
    if(system(command.c_str()) == 0){
        // Open temporary file
        current_path = getCurrentPath();
        current_path = current_path+"/temp.txt";
        file.open(current_path.c_str());
        if (file.is_open()){
            // While the file is open, copy each line into the specified vector using the provided md5pair structure
            while(file.good()){
                md5pair newmd5;
                file >> line;
                newmd5.num = line;
                file >> line;
                newmd5.path = line;
                if(line != newmd5.num){
                    md5s.push_back(newmd5);
                }
            }
            file.close();
        } else {
            // Do Nothing
        }
    } else {
        // Do Nothing
    }

    // Immediately delete the temporary file
    command = "rm temp.txt";
    if(system(command.c_str()) != 0){
        return -1;
    } else {
        // Do Nothing
    }

    // Mount PLO machine to local machine with specified information
    command = "mount -t cifs //"+ertms_plo.ip+"/C$ /mnt -o username="+ertms_plo.user+",password="+ertms_plo.pswd;
    if(system(command.c_str()) == 0){
        // Finds all files associated with specified LEU and puts them in a temporary file
        command = "find /mnt"+ertms_plo.data_path+"/* -name \"*"+leu+"*\" >> temp.txt";
        if(system(command.c_str()) == 0){
            // Open temporary file
            current_path = getCurrentPath();
            current_path = current_path+"/temp.txt";
            file.open(current_path.c_str());
            // Loop through file, elminating the files specified by it
            last_line = " ";
            while(file.good()){
                file >> line;
                if(last_line != line){
                    last_line = line;
                    command = "rm "+line;
                    if(system(command.c_str()) == 0){
                    }
                }
            }
        }
        // Unmount the PLO
        command = "umount /mnt/";
        if(system(command.c_str()) == 0){
        }
    }


    // List all files in the DATACORE directory on the 64-bit machine
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'find "+m_dpath+"/* -name \"*"+leu+"*\">> temp.txt\'";
    if(system(command.c_str()) == 0){
        // Move file to local machine
        command = "pscp -l "+user+" -pw "+pswd+" "+user+"@"+ipAdd64+":./temp.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/temp.txt";
            file.open(current_path.c_str());
            //While the file is open, find requested LEU files
            last_line = "";
            while(file.good()){
                file >> line;
                if(line != last_line ){
                    // Backup and erase LEU files
                    last_line = line;
                    updateLEUaux(ipAdd64, line);
                }
            }
            file.close();
        } else {
            // Do Nothing
        }
    }
    // Immediately remove temporary files on local and 64-bit machine
    command = "rm temp.txt";
    if(system(command.c_str()) == 0){
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" rm temp.txt";
        if(system(command.c_str()) == 0){

        }
    }

    // List all files in the DATACORE directory on the 32-bit machine
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'find "+m_dpath+"/* -name \"*"+leu+"*\">> temp.txt\'";
    if(system(command.c_str()) == 0){
        // Move file to local machine
        command = "pscp -l "+user+" -pw "+pswd+" "+user+"@"+ipAdd32+":./temp.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/temp.txt";
            file.open(current_path.c_str());
            //While the file is open, find requested LEU files
            last_line = "";
            while(file.good()){
                file >> line;
                if(line != last_line ){
                    // Backup and erase LEU files
                    last_line = line;
                    updateLEUaux(ipAdd32, line);
                }
            }
            file.close();
        } else {
            // Do Nothing
        }
    }
    // Immediately remove temporary files on local and 64-bit machine
    command = "rm temp.txt";
    if(system(command.c_str()) == 0){
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" rm temp.txt";
        if(system(command.c_str()) == 0){

        }
    }


    // Find all files associated to specified LEU on the local machine and store in a temporary file
    command = "find "+dpath+"/*/* -name \"*"+leu+"*\" >> temp.txt";
    if(system(command.c_str()) == 0){
        // Open temporary file
        current_path = getCurrentPath();
        current_path = current_path+"/temp.txt";
        file.open(current_path.c_str());
        // While the file is open, update files on specified machines
        while(file.good()){
            file >> line;
            // Extract machine ID from the machine folders path
            found_id = line.substr(dpath.size()+1, 8);
            // Move local file to 64-bit machine at the specified machine folder (found_id)
            command = "pscp -l "+user+" -pw "+pswd+" "+line+" "+user+"@"+ipAdd64+":"+m_dpath+"/"+found_id+"/";
            if(system(command.c_str()) == 0){
                mes = "Updated to "+line+" in "+m_dpath+"/"+found_id+" at "+ipAdd64+".\n";
                Mes = mes.c_str();
                emit textResult(Mes);
                // Move local file to 32-bit machine at the specified machine folder (found_id)
                command = "pscp -l "+user+" -pw "+pswd+" "+line+" "+user+"@"+ipAdd32+":"+m_dpath+"/"+found_id+"/";
                if(system(command.c_str()) == 0){
                    mes = "Updated to "+line+" in "+m_dpath+"/"+found_id+" at "+ipAdd32+".\n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                } else {
                    mes = "Update to "+line+" in "+m_dpath+"/"+found_id+" at "+ipAdd32+" failed.\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }
            } else {
                mes = "Update to "+line+" in "+m_dpath+"/"+found_id+" at "+ipAdd64+" failed.\n";
                Mes = mes.c_str();
                emit textError(Mes);
            }

            // Mount associated PLO
            command = "mount -t cifs //"+ertms_plo.ip+"/C$ /mnt -o username="+ertms_plo.user+",password="+ertms_plo.pswd;
            if(system(command.c_str()) == 0){
                // Copy local files to specified machine folder (found_id)
                command = "cp "+line+" /mnt"+ertms_plo.data_path+"/"+found_id+"/";
                if(system(command.c_str()) == 0){
                    mes = "Updated to "+line+" at PLO for "+found_id+".\n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                }
                // Unmount PLO
                command = "umount /mnt/";
                if(system(command.c_str()) == 0){
                } else {
                    mes = "Error unmounting PLO for "+found_id+".\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }
            } else {
                mes = "Error mounting PLO for "+found_id+".\n";
                Mes = mes.c_str();
                emit textError(Mes);
            }


        }
        file.close();
        // Remove temporary file
        command = "rm temp.txt";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            // Do Nothing
        }

    } else {
        mes = "Search for specified LEU files failed on the local machine";
        Mes = mes.c_str();
        emit textError(Mes);
        return 1;
    }


    // Restart system
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr start &";
    if(system(command.c_str()) == 0){
        // Do nothing
    } else {
        mes = "System restart at "+ipAdd32+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }



    // Calculate new MD5 of files located on the 64-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'find "+m_dpath+"/*/* -name \"*"+leu+"*\" -exec md5sum {} \\; >> temp1.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd64+":./temp1.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/temp1.txt";
            file1.open(current_path.c_str());
            if (file1.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file1.good()){
                    md5pair newmd5;
                    file1 >> line;
                    newmd5.num = line;
                    file1 >> line;
                    line.erase(0, 1);
                    newmd5.path = line;
                    if(line != newmd5.num){
                        md5s64.push_back(newmd5);
                    }
                }
                file1.close();
            } else {
                // Do Nothing
            }
        } else {
            // Do Nothing
        }
    } else {
        mes = "Error occurred calculating the MD5 of files of the ERTMS machine at "+ipAdd64+".\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }

    // Calculate new MD5 of files located on the 32-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'find "+m_dpath+"/*/* -name \"*"+leu+"*\" -exec md5sum {} \\; >> temp2.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd32+":./temp2.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/temp2.txt";
            file2.open(current_path.c_str());
            if (file2.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file2.good()){
                    md5pair newmd5;
                    file2 >> line;
                    newmd5.num = line;
                    file2 >> line;
                    line.erase(0, 1);
                    newmd5.path = line;
                    if(line != newmd5.num){
                        md5s32.push_back(newmd5);
                    }
                }
                file2.close();
            } else {
                // Do Nothing
            }

        } else {
            // Do Nothing
        }
    } else {
        mes = "Error occurred calculating the MD5 of files of the ERTMS machine at "+ipAdd32+".\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }


    // Deletes all temporary files
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" rm temp1.txt";
    if(system(command.c_str()) == 0){
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" rm temp2.txt";
        if(system(command.c_str()) == 0){
            command = "rm temp1.txt";
            if(system(command.c_str()) == 0){
                command = "rm temp2.txt";
                if(system(command.c_str()) != 0){
                    return -1;
                } else {
                    // Do Nothing
                }
            } else {
                mes = "Error occurred attempting to delete temporary files.\n";
                Mes = mes.c_str();
                emit textError(Mes);
                return -1;
            }
        } else {
            mes = "Error occurred attempting to delete temporary files.\n";
            Mes = mes.c_str();
            emit textError(Mes);
            return -1;
        }
    } else {
        mes = "Error occurred attempting to delete temporary files.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }

    // Loop to compare machines MD5 results with local files that were transfered
    for(int i=0; i<(int)md5s.size(); i++){
        not_vec = false;
        // Loop to run through results of the 32-bit machine for comparison
        for(int j=0; j<(int)md5s32.size(); j++){
            // Checks if two file names are equal
            if(md5s[i].path == md5s32[j].path){
                not_vec = true;
                // Should the names be equal, their number should be as well
                if(md5s[i].num != md5s32[j].num){
                    // Error Message sent to main window log
                    mes = "MD5 check unsuccessfull, "+md5s[i].path+" (local file) has a different MD5 Hash than"+ md5s32[j].path+" on"+ipAdd32+". \n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                    return -1;
                }
            }

        }
        // Checks if a local file was found or not on the machine (not_vec is false if the file WASNT found)
        if(!not_vec){
            // Error Message sent to main window log
            mes = "MD5 check unsuccessfull, found a DATACORE file not matching the specified files for update.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
            return -1;
        }
        not_vec = false;
        // Loop to run through results of the 64-bit machine for comparison
        for(int k=0; k<(int)md5s64.size(); k++){
            // Checks if two file names are equal
            if(md5s[i].path == md5s64[k].path){
                not_vec = true;
                // Should the names be equal, their number should be as well
                if(md5s[i].num != md5s64[k].num){
                    // Error Message sent to main window log
                    mes = "MD5 check unsuccessfull, "+md5s[i].path+" (local file) has a different MD5 Hash than"+ md5s64[k].path+" on"+ipAdd64+". \n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                    return -1;
                }
            }
        }
        if(!not_vec){
            // Error Message sent to main window log
            mes = "MD5 check unsuccessfull, found a DATACORE file not matching the specified files for update.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
            return -1;
        }

    }

    // Message sent to main window log
    mes = "MD5 Check successfull. \n\nRequested LEU updated successfully.\n";
    Mes = mes.c_str();
    emit textResult(Mes);


    return 0;

}




/*
****************************************************************************************************************************

updateRBC:

This function updates all files associated to a specified RBC.


****************************************************************************************************************************
*/


int connectionsWindow::updateRBC(string ipAdd64, string ipAdd32, string name, string m_dpath){

    // Variable Declaration
    string aux_path;
    vector<md5pair> md5s;
    vector<md5pair> md5s32;
    vector<md5pair> md5s64;
    vector<string> vec_ids;
    vector<string> vec_aux;
    ifstream file;
    ifstream file1;
    ifstream file2;
    string line;
    string command;
    string found_id;
    string mes;
    string cut_leu;
    string current_path;
    string last_line;
    QString Mes;
    plo_r ertms_plo;
    bool not_vec;
    bool string_in;

    // cut_leu represents the LEU ID number leaving out the first 2 digits which are "null". This will come into play when updating dc2 files.
    cut_leu = leu.substr(2,6);

    mes = "******************************************************************************\n\n          Update RBC "+leu+" for "+name+" \n\n******************************************************************************\n";
    Mes = mes.c_str();
    emit textOp(Mes);

    // Find all files associated with the specified LEU id
    command = "find "+dpath+"/* -name \"*"+leu+"*\" >> temp.txt";
    if(system(command.c_str()) == 0){
        // Open temporary file
        current_path = getCurrentPath();
        current_path = current_path+"/temp.txt";
        file.open(current_path.c_str());
        // While the file is open, retrieve the IDs of the machines the files are associated with
        while(file.good()){
            file >> line;
            // Should always start with .
            if(line.substr(0,1) == "."){
                // retrieve machine ID by grabbing the ID number associated to the folder in which the file was found, ignoring the path before that
                found_id = line.substr(dpath.size()+1, 8);
                // Checks to see if the id is already in the vector, to avoid repeat instances
                if(vec_ids.empty()){
                    vec_ids.push_back(found_id);
                } else {
                    string_in = false;
                    for(int i=0; i<(int)vec_ids.size(); i++){
                        if(vec_ids[i] == found_id){
                            string_in = true;
                        }
                    }
                    if(!string_in){
                        // Should the id not be in the vector, we add it
                        vec_ids.push_back(found_id);
                    }
                }
            }
        }
    }

    // Immediately remove temporary file.

    command = "rm temp.txt";
    if(system(command.c_str()) == 0){
    }

    // Calculate MD5 for all Datacore files that will be uploaded to the ERTMS Manager
    // and put results in a temporary file
    command = "find "+dpath+"/*/* -name \"*"+cut_leu+"*\" -exec md5sum {} \\; >> temp.txt";
    if(system(command.c_str()) == 0){
        // Open temporary file
        current_path = getCurrentPath();
        current_path = current_path+"/temp.txt";
        file.open(current_path.c_str());
        if (file.is_open()){
            // While the file is open, copy each line into the specified vector using the provided md5pair structure
            while(file.good()){
                md5pair newmd5;
                file >> line;
                newmd5.num = line;
                file >> line;
                newmd5.path = line;
                if(line != newmd5.num){
                    md5s.push_back(newmd5);
                }
            }
            file.close();
        } else {
            // Do Nothing
        }
    } else {
        // Do Nothing
    }

    // Immediately delete the temporary file
    command = "rm temp.txt";
    if(system(command.c_str()) != 0){
        return -1;
    } else {
        // Do Nothing
    }

    // Run ERTMS Manager stop command on the 32-bit machine
    if(shutdown(ipAdd32) == 0){
        // Do nothing
    } else {
        mes = "Shutdown at "+ipAdd32+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }

    // Loop to find Manager in question
    for (int i=0; i<i_count; i++){
        if(ilist[i].id == leu){
            // List all rbc files in the RBCs DATACORE directory on the 64-bit machine
            command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'find "+m_dpath+"/"+leu+"/* -name \"*"+cut_leu+"*\" >> temp.txt\'";
            if(system(command.c_str()) == 0){
                // Move file to local machine
                command = "pscp -l "+user+" -pw "+pswd+" "+user+"@"+ipAdd64+":./temp.txt ./";
                if(system(command.c_str()) == 0){
                    current_path = getCurrentPath();
                    current_path = current_path+"/temp.txt";
                    file.open(current_path.c_str());
                    //While the file is open, find requested LEU files
                    last_line = "";
                    while(file.good()){
                        file >> line;
                        if(last_line != line){
                            last_line = line;
                            // Backup and erase all leu files found on remote machine
                            updateLEUaux(ipAdd64, line);
                        }
                    }

                    file.close();
                } else {
                    // Do Nothing
                }
            } else {
                // Do Nothing
            }

            // Immediately remove temporary files on local and 64-bit machine
            command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" rm temp.txt";
            if(system(command.c_str()) == 0){
                command = "rm temp.txt";
                if(system(command.c_str()) == 0){
                    // Do Nothing
                } else {
                    // Do Nothing
                }
            }

            // List all rbc files in the DATACORE directory on the 32-bit machine
            command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'find "+m_dpath+"/"+leu+"/* -name \"*"+cut_leu+"*\" >> temp.txt\'";
            if(system(command.c_str()) == 0){
                // Move file to local machine
                command = "pscp -l "+user+" -pw "+pswd+" "+user+"@"+ipAdd32+":./temp.txt ./";
                if(system(command.c_str()) == 0){
                    current_path = getCurrentPath();
                    current_path = current_path+"/temp.txt";
                    file.open(current_path.c_str());
                    //While the file is open, find requested LEU files
                    last_line = "";
                    while(file.good()){
                        file >> line;
                        if(last_line != line){
                            last_line = line;
                            // Backup and erase all leu files found on remote machine
                            updateLEUaux(ipAdd32, line);
                        }
                    }

                    file.close();
                } else {
                    // Do Nothing
                }
            } else {
                // Do Nothing
            }

            // Immediately remove temporary files on local and 32-bit machine
            command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" rm temp.txt";
            if(system(command.c_str()) == 0){
                command = "rm temp.txt";
                if(system(command.c_str()) == 0){
                    // Do Nothing
                } else {
                    // Do Nothing
                }
            }

        } else {
            // Do Nothing
        }
    }

    for (int i=0; i<e_count; i++){
        for (int j=0; j<(int)vec_ids.size(); j++){
            if(elist[i].id == vec_ids[j]){
                ertms_plo = elist[i].plo;
                if(shutdown(elist[i].ip_32) == 0){
                    // Do nothing
                } else {
                    mes = "Shutdown at "+elist[i].ip_32+" unsuccessfull.\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                return -1;
                }
                // List all rbc files in the DATACORE directory on the 64-bit machine
                command = "plink -l "+user+" -pw "+pswd+" "+elist[i].ip_64+" \'find "+elist[i].data_path+"/*/* -name \"*"+cut_leu+"*\" >> temp.txt\'";
                if(system(command.c_str()) == 0){
                    // Move file to local machine
                    command = "pscp -l "+user+" -pw "+pswd+" "+user+"@"+elist[i].ip_64+":./temp.txt ./";
                    if(system(command.c_str()) == 0){
                        current_path = getCurrentPath();
                        current_path = current_path+"/temp.txt";
                        file.open(current_path.c_str());
                        //While the file is open, find requested LEU files
                        last_line = "";
                        while(file.good()){
                            file >> line;
                            if(last_line != line){
                                last_line = line;
                                // Backup and erase all leu files found on remote machine
                                updateLEUaux(elist[i].ip_64, line);
                            }
                        }

                        file.close();
                    } else {
                        // Do Nothing
                    }
                } else {
                    // Do Nothing
                }

                // Immediately remove temporary files on local and 64-bit machine
                command = "plink -l "+user+" -pw "+pswd+" "+elist[i].ip_64+" rm temp.txt";
                if(system(command.c_str()) == 0){
                    command = "rm temp.txt";
                    if(system(command.c_str()) == 0){
                        // Do Nothing
                    } else {
                        // Do Nothing
                    }
                }

                // List all rbc files in the DATACORE directory on the 32-bit machine
                command = "plink -l "+user+" -pw "+pswd+" "+elist[i].ip_32+" \'find "+elist[i].data_path+"/*/* -name \"*"+cut_leu+"*\" >> temp.txt\'";
                if(system(command.c_str()) == 0){
                    // Move file to local machine
                    command = "pscp -l "+user+" -pw "+pswd+" "+user+"@"+elist[i].ip_32+":./temp.txt ./";
                    if(system(command.c_str()) == 0){
                        current_path = getCurrentPath();
                        current_path = current_path+"/temp.txt";
                        file.open(current_path.c_str());
                        //While the file is open, find requested LEU files
                        last_line = "";
                        while(file.good()){
                            file >> line;
                            if(last_line != line){
                                last_line = line;
                                // Backup and erase all leu files found on remote machine
                                updateLEUaux(elist[i].ip_32, line);
                            }
                        }

                        file.close();
                    } else {
                        // Do Nothing
                    }
                } else {
                    // Do Nothing
                }

                // Immediately remove temporary files on local and 32-bit machine
                command = "plink -l "+user+" -pw "+pswd+" "+elist[i].ip_32+" rm temp.txt";
                if(system(command.c_str()) == 0){
                    command = "rm temp.txt";
                    if(system(command.c_str()) == 0){
                        // Do Nothing
                    } else {
                        // Do Nothing
                    }
                }

                command = "mount -t cifs //"+ertms_plo.ip+"/C$ /mnt -o username="+ertms_plo.user+",password="+ertms_plo.pswd;
                if(system(command.c_str()) == 0){
                    // Copy local files to specified machine folder (found_id)
                    command = "find /mnt"+ertms_plo.data_path+"/* -name \"*"+cut_leu+"*\" >> temp.txt";
                    if(system(command.c_str()) == 0){
                        current_path = getCurrentPath();
                        current_path = current_path+"/temp.txt";
                        file.open(current_path.c_str());
                        while(file.good()){
                            file >> line;
                            if(last_line != line){
                                last_line = line;
                                // Backup and erase all leu files found on remote machine
                                command = "rm "+line;
                                if(system(command.c_str()) == 0){
                                    mes = "Removed "+line+" at PLO for "+found_id+".\n";
                                    Mes = mes.c_str();
                                    emit textResult(Mes);
                                } else {
                                    mes = "Failed removing "+line+" at PLO for "+found_id+".\n";
                                    Mes = mes.c_str();
                                    emit textError(Mes);
                                }
                            }
                        }
                    } else {
                        // Do Nothing
                    }
                    // Unmount PLO
                    command = "umount /mnt/";
                    if(system(command.c_str()) == 0){
                        // Do Nothing
                    } else {
                        mes = "Error unmounting PLO for "+found_id+".\n";
                        Mes = mes.c_str();
                        emit textError(Mes);
                    }
                } else {
                    mes = "Error mounting PLO for "+found_id+".\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                }
            }
        }
    }


    // Iteration to search for specified IF2 machine
    for (int i=0; i<i_count; i++){
        if(ilist[i].id == leu){
            // Find all files containing the RBC's ID. This ignores the first two digits (always "00") in it's search
            command = "find "+dpath+"/*/* -name \"*"+cut_leu+"*\" >> temp.txt";
            if(system(command.c_str()) == 0){
                current_path = getCurrentPath();
                current_path = current_path+"/temp.txt";
                file.close();
                file.open(current_path.c_str());
                // While the file is open, look for requested LEU files to update
                last_line = "";
                while(file.good()){
                    file >> line;
                    if(last_line != line){
                        last_line = line;
                        // Extracts the ertms machines ID number from the file path
                        found_id = line.substr(dpath.size()+1, 8);
                        if(found_id == leu){
                            // Copy local file to remote machine (64bit)
                            command = "pscp -l "+user+" -pw "+pswd+" "+line+" "+user+"@"+ipAdd64+":"+m_dpath+"/"+found_id+"/";
                            if(system(command.c_str()) == 0){
                                mes = "Updated to "+line+" in "+m_dpath+"/"+found_id+" at "+ipAdd64+".\n";
                                Mes = mes.c_str();
                                emit textResult(Mes);
                                command = "pscp -l "+user+" -pw "+pswd+" "+line+" "+user+"@"+ipAdd32+":"+m_dpath+"/"+found_id+"/";
                                if(system(command.c_str()) == 0){
                                    // Copy local file to remote machine (64bit)
                                    mes = "Updated to "+line+" in "+m_dpath+"/"+found_id+" at "+ipAdd32+".\n";
                                    Mes = mes.c_str();
                                    emit textResult(Mes);
                                } else {
                                    mes = "Update to "+line+" in "+m_dpath+"/"+found_id+" at "+ipAdd32+" failed.\n";
                                    Mes = mes.c_str();
                                    emit textError(Mes);
                                }
                            } else{
                                mes = "Update to "+line+" in "+m_dpath+"/"+found_id+" at "+ipAdd64+" failed.\n";
                                Mes = mes.c_str();
                                emit textError(Mes);
                            }
                        }
                        else {
                            // Looks for machines associated to this RBC
                            for (int k=0; k<e_count; k++){
                                // Iteration over previously calculated associated machines vector
                                for (int j=0; j<(int)vec_ids.size(); j++){
                                    if(elist[k].id == vec_ids[j]){
                                        ertms_plo = elist[k].plo;
                                        // Copy local file to specified location in the specified 64-bit machine associated to the RBC
                                        command = "pscp -l "+user+" -pw "+pswd+" "+line+" "+user+"@"+elist[k].ip_64+":"+elist[k].data_path+"/"+found_id+"/";
                                        if(system(command.c_str()) == 0){
                                            mes = "Updated to "+line+" in "+elist[k].data_path+"/"+found_id+" at "+elist[k].ip_64+".\n";
                                            Mes = mes.c_str();
                                            emit textResult(Mes);
                                            // Copy local file to specified location in the specified 32-bit machine associated to the RBC
                                            command = "pscp -l "+user+" -pw "+pswd+" "+line+" "+user+"@"+elist[k].ip_32+":"+elist[k].data_path+"/"+found_id+"/";
                                            if(system(command.c_str()) == 0){
                                                mes = "Updated to "+line+" in "+elist[k].data_path+"/"+found_id+" at "+elist[k].ip_32+".\n";
                                                Mes = mes.c_str();
                                                emit textResult(Mes);
                                            } else {
                                                mes = "Update to "+line+" in "+elist[k].data_path+"/"+found_id+" at "+elist[k].ip_32+" failed.\n";
                                                Mes = mes.c_str();
                                                emit textError(Mes);
                                            }
                                        } else {
                                            mes = "Update to "+line+" in "+elist[k].data_path+"/"+found_id+" at "+elist[k].ip_64+" failed.\n";
                                            Mes = mes.c_str();
                                            emit textError(Mes);
                                        }

                                        // Mount associated PLO
                                        command = "mount -t cifs //"+ertms_plo.ip+"/C$ /mnt -o username="+ertms_plo.user+",password="+ertms_plo.pswd;
                                        if(system(command.c_str()) == 0){
                                            // Copy local files to specified machine folder (found_id)
                                            command = "cp "+line+" /mnt"+ertms_plo.data_path+"/"+found_id+"/";
                                            if(system(command.c_str()) == 0){
                                                mes = "Updated to "+line+" at PLO for "+found_id+".\n";
                                                Mes = mes.c_str();
                                                emit textResult(Mes);
                                            }
                                            // Unmount PLO
                                            command = "umount /mnt/";
                                            if(system(command.c_str()) == 0){
                                            } else {
                                                mes = "Error unmounting PLO for "+found_id+".\n";
                                                Mes = mes.c_str();
                                                emit textError(Mes);
                                            }
                                        } else {
                                            mes = "Error mounting PLO for "+found_id+".\n";
                                            Mes = mes.c_str();
                                            emit textError(Mes);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                file.close();
                // Immediately remove temporary file on local machine
                command = "rm temp.txt";
                if(system(command.c_str()) == 0){
                    // Do Nothing
                } else {
                    // Do Nothing
                }

            }
        }
    }

    // Attempt to start the RBC machine
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr start &";
    if(system(command.c_str()) != 0){
        mes = "System start operation at "+ipAdd32+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    } else {
        mes = "System start operation at "+ipAdd32+" successfull.\n";
        Mes = mes.c_str();
        emit textResult(Mes);
    }

    // Iteration to find RBCs associated machines
    for (int k=0; k<e_count; k++){
        // Iteration over previously calculated associated machines vector
        for (int j=0; j<(int)vec_ids.size(); j++){
            if(elist[k].id == vec_ids[j]){
                // Attempt to start the RBC machine
                command = "plink -l "+user+" -pw "+pswd+" "+elist[k].ip_32+" gr start &";
                if(system(command.c_str()) != 0){
                    mes = "System start operation at "+elist[k].ip_32+" unsuccessfull.\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                    return -1;
                } else {
                    mes = "System start operation at "+elist[k].ip_32+" successfull.\n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                }
            }
        }
    }


    // Calculate new MD5 of files located on the 64-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'find "+m_dpath+"/*/* -name \"*"+cut_leu+"*\" -exec md5sum {} \\; >> temp1.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd64+":./temp1.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/temp1.txt";
            file1.open(current_path.c_str());
            if (file1.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file1.good()){
                    md5pair newmd5;
                    file1 >> line;
                    newmd5.num = line;
                    file1 >> line;
                    line.erase(0, 1);
                    newmd5.path = line;
                    md5s64.push_back(newmd5);
                }
                file1.close();
                command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" rm temp1.txt";
                if(system(command.c_str()) == 0){
                    command = "rm temp1.txt";
                    if(system(command.c_str()) == 0){
                        // Do Nothing
                    } else {
                        // Do Nothing
                    }
                }

            } else {
                // Do Nothing
            }

        } else {
            // Do Nothing
        }
    } else {
        mes = "Error occurred calculating the MD5 of files of the ERTMS machine at "+ipAdd64+".\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }

    // Iteration over machines associated to the RBC
    for (int k=0; k<e_count; k++){
        for (int j=0; j<(int)vec_ids.size(); j++){
            if(elist[k].id == vec_ids[j]){
                // Calculate new MD5 of files located on the associated 64-bit machine and writes results in a temporary file
                command = "plink -l "+user+" -pw "+pswd+" "+elist[k].ip_64+" \'find "+elist[k].data_path+"/*/* -name \"*"+cut_leu+"*\" -exec md5sum {} \\; >> temp1.txt\'";
                if(system(command.c_str()) == 0){
                    // Pass the output file to local machine
                    command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+elist[k].ip_64+":./temp1.txt ./";
                    if(system(command.c_str()) == 0){
                        current_path = getCurrentPath();
                        current_path = current_path+"/temp1.txt";
                        file1.open(current_path.c_str());
                        if (file1.is_open()){
                            // While the file is open, copy each line into the specified vector using the provided md5pair structure
                            while(file1.good()){
                                md5pair newmd5;
                                file1 >> line;
                                newmd5.num = line;
                                file1 >> line;
                                line.erase(0, 1);
                                newmd5.path = line;
                                md5s64.push_back(newmd5);
                            }
                            file1.close();
                            command = "plink -l "+user+" -pw "+pswd+" "+elist[k].ip_64+" rm temp1.txt";
                            if(system(command.c_str()) == 0){
                                command = "rm temp1.txt";
                                if(system(command.c_str()) == 0){
                                    // Do Nothing
                                } else {
                                    // Do Nothing
                                }
                            }
                        } else {
                            // Do Nothing
                        }

                    } else {
                        // Do Nothing
                    }
                } else {
                    mes = "Error occurred calculating the MD5 of files of the ERTMS machine at "+elist[k].ip_64+".\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                    return -1;
                }
            }
        }
    }


    // Calculate new MD5 of files located on the 32-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'find "+m_dpath+"/*/* -name \"*"+cut_leu+"*\" -exec md5sum {} \\; >> temp1.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd32+":./temp1.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/temp1.txt";
            file1.open(current_path.c_str());
            if (file1.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file1.good()){
                    md5pair newmd5;
                    file1 >> line;
                    newmd5.num = line;
                    file1 >> line;
                    line.erase(0, 1);
                    newmd5.path = line;
                    md5s32.push_back(newmd5);
                }
                file1.close();
                command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" rm temp1.txt";
                if(system(command.c_str()) == 0){
                    command = "rm temp1.txt";
                    if(system(command.c_str()) == 0){
                        // Do Nothing
                    } else {
                        // Do Nothing
                    }
                }

            } else {
                // Do Nothing
            }

        } else {
            // Do Nothing
        }
    } else {
        mes = "Error occurred calculating the MD5 of files of the ERTMS machine at "+ipAdd32+".\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    }

    // Iteration over machines associated to the RBC
    for (int k=0; k<e_count; k++){
        for (int j=0; j<(int)vec_ids.size(); j++){
            if(elist[k].id == vec_ids[j]){
                // Calculate new MD5 of files located on the associated 32-bit machine and writes results in a temporary file
                command = "plink -l "+user+" -pw "+pswd+" "+elist[k].ip_32+" \'find "+elist[k].data_path+"/*/* -name \"*"+cut_leu+"*\" -exec md5sum {} \\; >> temp1.txt\'";
                if(system(command.c_str()) == 0){
                    // Pass the output file to local machine
                    command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+elist[k].ip_32+":./temp1.txt ./";
                    if(system(command.c_str()) == 0){
                        current_path = getCurrentPath();
                        current_path = current_path+"/temp1.txt";
                        file1.open(current_path.c_str());
                        if (file1.is_open()){
                            // While the file is open, copy each line into the specified vector using the provided md5pair structure
                            while(file1.good()){
                                md5pair newmd5;
                                file1 >> line;
                                newmd5.num = line;
                                file1 >> line;
                                line.erase(0, 1);
                                newmd5.path = line;
                                md5s32.push_back(newmd5);
                            }
                            file1.close();
                            command = "plink -l "+user+" -pw "+pswd+" "+elist[k].ip_32+" rm temp1.txt";
                            if(system(command.c_str()) == 0){
                                command = "rm temp1.txt";
                                if(system(command.c_str()) == 0){
                                    // Do Nothing
                                } else {
                                    // Do Nothing
                                }
                            }
                        } else {
                            // Do Nothing
                        }

                    } else {
                        // Do Nothing
                    }
                } else {
                    mes = "Error occurred calculating the MD5 of files of the ERTMS machine at "+elist[k].ip_32+".\n";
                    Mes = mes.c_str();
                    emit textError(Mes);
                    return -1;
                }
            }
        }
    }


    // Loop to compare machines MD5 results with local files that were transfered
    for(int i=0; i<(int)md5s.size()-1; i++){
        not_vec = false;
        // Loop to run through results of the 32-bit machine for comparison
        for(int j=0; j<(int)md5s32.size()-1; j++){
            // Checks if two file names are equal
            if(md5s[i].path == md5s32[j].path){
                not_vec = true;
                // Should the names be equal, their number should be as well
                if(md5s[i].num != md5s32[j].num){
                    // Error Message sent to main window log
                    mes = "MD5 check unsuccessfull, "+md5s[i].path+" (local file) has a different MD5 Hash than"+ md5s32[j].path+" on"+ipAdd32+". \n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                    return -1;
                }
            }

        }
        // Checks if a local file was found or not on the machine (not_vec is false if the file WASNT found)
        if(!not_vec){
            // Error Message sent to main window log
            mes = "MD5 check unsuccessfull, found a DATACORE file not matching the specified files for update.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
            return -1;
        }
        not_vec = false;
        // Loop to run through results of the 64-bit machine for comparison
        for(int k=0; k<(int)md5s64.size()-1; k++){
            // Checks if two file names are equal
            if(md5s[i].path == md5s64[k].path){
                not_vec = true;
                // Should the names be equal, their number should be as well
                if(md5s[i].num != md5s64[k].num){
                    // Error Message sent to main window log
                    mes = "MD5 check unsuccessfull, "+md5s[i].path+" (local file) has a different MD5 Hash than"+ md5s64[k].path+" on"+ipAdd64+". \n";
                    Mes = mes.c_str();
                    emit textResult(Mes);
                    return -1;
                }
            }
        }
        if(!not_vec){
            // Error Message sent to main window log
            mes = "MD5 check unsuccessfull, found a DATACORE file not matching the specified files for update.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
            return -1;
        }

    }

    // Message sent to main window log
    mes = "MD5 Check successfull. \n\nRequested LEU updated successfully.\n";
    Mes = mes.c_str();
    emit textResult(Mes);

    return 0;

}





/*
****************************************************************************************************************************

displayVersion:

This function displays the ERTMS Managers executable files version and the local machines version by comparing them
to the versions.vrs file defined by the user.


****************************************************************************************************************************
*/


int connectionsWindow::displayVersion(string ipAdd64, string ipAdd32, string name, string m_mpath){

    // Declare Variables
    QWidget *displayWindow = new QWidget;
    QLabel *label;
    QVBoxLayout *versLayout;
    QTextBrowser *textWindow;
    string command;
    string line;
    string s1;
    string s2;
    string outstring;
    string machine;
    string current_path;
    vector<md5pair> pairs1;
    vector<md5pair> pairs2;
    vector<md5pair> pairs3;
    ifstream file1;
    ifstream file2;
    ifstream file3;
    vector<md5pair> aux_vec;
    string mes;
    QString Mes;
    bool return_cond;



    mes = "******************************************************************************\n\n          Displaying Version Information for "+name+" \n\n******************************************************************************\n";
    Mes = mes.c_str();
    emit textOp(Mes);

    return_cond = false;

    label = new QLabel("Version information:");
    versLayout = new QVBoxLayout;
    textWindow = new QTextBrowser;

    // Calculate MD5 of executables located on the 32-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'find "+m_mpath+"/*/* -name \"*linux.??\" -exec md5sum {} \\; >> aux2.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd32+":./aux2.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/aux2.txt";
            file2.open(current_path.c_str());
            if (file2.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file2.good()){
                    md5pair pair;
                    file2 >> line;
                    pair.num = line;
                    file2 >> line;
                    pair.path = "32"+line;
                    if(line != pair.num){
                        pairs2.push_back(pair);
                    }
                }
            } else {
                return_cond = true;
            }
            file2.close();
        } else{
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Immediately remove local and Manager files
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" rm aux2.txt";
    if(system(command.c_str()) == 0){
        command = "rm aux2.txt";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Calculate MD5 of executables located on the 32-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'find "+m_mpath+"/*/* -name \"*linux.??\" -exec md5sum {} \\; >> aux3.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd64+":./aux3.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/aux3.txt";
            file3.open(current_path.c_str());
            if (file3.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file3.good()){
                    md5pair pair;
                    file3 >> line;
                    pair.num = line;
                    file3 >> line;
                    pair.path = "64"+line;
                    if(line != pair.num){
                        pairs2.push_back(pair);
                    }
                }
            } else {
                return_cond = true;
            }
            file3.close();
        } else{
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Immediately remove local and Manager files
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" rm aux3.txt";
    if(system(command.c_str()) == 0){
        command = "rm aux3.txt";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            return_cond = true;
        }
    } else {
        return_cond = true;
    }


    // Calculate MD5 of executables located on the local machine and writes results in a temporary file
    command = "find "+mpath+"/*/* -name \"*linux*\" -exec md5sum {} \\; >> aux1.txt";
    if(system(command.c_str()) == 0){
        current_path = getCurrentPath();
        current_path = current_path+"/aux1.txt";
        file1.open(current_path.c_str());
        if (file1.is_open()){
            // While the file is open, copy each line into the specified vector using the provided md5pair structure
            while(file1.good()){
                md5pair pair;
                file1 >> line;
                pair.num = line;
                file1 >> line;
                pair.path = line;
                if(line != pair.num){
                    pairs1.push_back(pair);
                }
            }
        } else {
            return_cond = true;
        }
        file1.close();
    } else {
        return_cond = true;
    }

    // Immediately remove local temp file
    command = "rm aux1.txt";
    if(system(command.c_str()) == 0){
        // Do Nothing
    } else {
        return_cond = true;
    }

    // Conditional if there was a failure calculating MD5s
    if(return_cond){
        mes = "Error pertaining to the use of temporary files for calculating MD5s";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    } else {
        // Do nothing
    }

    // Create a "header" for following Local Machine version information
    textWindow->append(QString("Local Machine:"));
    textWindow->append(QString(" "));

    // Loop through local MD5 information
    for(int i=0; i<(int)pairs1.size()-1; i++){
        s1 = elimPath(pairs1[i].path);
        // Loop though versions.vrs product information for comparison
        for(int j=0; j<(int)list_products.size(); j++){
            aux_vec = list_products[j].pro_ids;
            // Loops through ids of a product
            for(int k=0; k<(int)aux_vec.size(); k++){
                s2 = elimPath(aux_vec[k].path);
                // Checks if filenames are the same
                if(s1 == s2){
                    // Should filenames match, checks if the MD5 number matches
                    if(aux_vec[k].num == pairs1[i].num){
                        outstring = "The version for "+pairs1[i].path+" is: "+list_products[j].name+" v."+list_products[j].version;
                        textWindow->append(QString(outstring.c_str()));
                    } else {
                        outstring = "The version for "+pairs1[i].path+" is unknown.";
                        textWindow->append(QString(outstring.c_str()));
                    }
                } else {
                    // Do Nothing
                }
            }


        }

    }

    // Create a "header" for following Local Machine version information
    textWindow->append(QString(" "));
    textWindow->append(QString("ERTMS Machines:"));
    textWindow->append(QString(" "));

    // Loop through Manager MD5 information
    for(int i=0; i<(int)pairs2.size()-1; i++){
        s1 = elimPath(pairs2[i].path);
        // Quickly retrieves which machine the current loops is referring to
        machine = (pairs2[i].path).substr(0, 2);
        (pairs2[i].path).erase(0,2);
        // Loop though versions.vrs product information for comparison
        for(int j=0; j<(int)list_products.size(); j++){
            aux_vec = list_products[j].pro_ids;
            // Loops through ids of a product
            for(int k=0; k<(int)aux_vec.size(); k++){
                s2 = elimPath(aux_vec[k].path);
                // Checks if filenames are the same
                if(s1 == s2){
                    // Should filenames match, checks if the MD5 number matches
                    if(aux_vec[k].num == pairs2[i].num){
                        outstring = "The version for "+pairs2[i].path+" is: "+list_products[j].name+" v."+list_products[j].version+" on the "+machine+"-bit machine.";
                        textWindow->append(QString(outstring.c_str()));
                    } else {
                        outstring = "The version for "+pairs2[i].path+" is unknown on the "+machine+"-bit machine.";
                        textWindow->append(QString(outstring.c_str()));
                    }
                } else {
                    // Do Nothing
                }
            }


        }

    }

    // Display procedures
    versLayout->addWidget(label);
    versLayout->addWidget(textWindow);
    displayWindow->setLayout(versLayout);
    displayWindow->resize(800, 1000);
    displayWindow->setWindowTitle("Version Information");
    displayWindow->show();

    mes = "Succesfully displyed version information";
    Mes = mes.c_str();
    emit textResult(Mes);

    return 0;


}


/*
****************************************************************************************************************************

updateExecutables:

This function updates the ERTMS Managers executable files with the local machines version and subsequently updates the system.


****************************************************************************************************************************
*/


int connectionsWindow::updateExecutables(string ipAdd64, string ipAdd32, string name, string m_mpath){

    // Declare Variables
    string command;
    string line;
    string s1;
    string s2;
    string machine;
    string ipcon;
    string temp_path;
    string current_path;
    string temp_aux;
    vector<md5pair> pairs1;
    vector<md5pair> pairs2;
    vector<md5pair> pairs3;
    ifstream file1;
    ifstream file2;
    ifstream file3;
    size_t position;
    string mes;
    QString Mes;
    bool return_cond;


    mes = "******************************************************************************\n\n          Updating Executable Files for "+name+" \n\n******************************************************************************\n";
    Mes = mes.c_str();
    emit textOp(Mes);

    return_cond = false;

    // Run ERTMS Manager stop command on the 32-bit machine
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr stop";
    if(system(command.c_str()) == 0){
        // Run ERTMS Manager "clear TSR" command on the 32-bit machine
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr clear_tsr";
        if(system(command.c_str()) == 0){
            // Successfull calls handling
            mes = "System shutdown at "+ipAdd32+" successfull.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
        } else {
            // Clear TSR failure
            mes = "Clear TSR operation at "+ipAdd32+" unsuccessfull.\n";
            Mes = mes.c_str();
            emit textError(Mes);
            return 1;
        }
    } else {
        // Stop failure
        mes = "Stop operation at "+ipAdd32+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return 1;
    }

    // Calculate MD5 of executables located on the local machine and writes results in a temporary file
    command = "find "+mpath+"/*/ -name \"*linux*\" -exec md5sum {} \\; >> aux1.txt";
    if(system(command.c_str()) == 0){
        current_path = getCurrentPath();
        current_path = current_path+"/aux1.txt";
        file1.open(current_path.c_str());
        if (file1.is_open()){
            // While the file is open, copy each line into the specified vector using the provided md5pair structure
            while(file1.good()){
                md5pair pair;
                file1 >> line;
                pair.num = line;
                file1 >> line;
                pair.path = line;
                if(line != pair.num){
                    pairs1.push_back(pair);
                }
            }
        } else {
            return_cond = true;
        }
        file1.close();
    } else {
        return_cond = true;
    }

    // Immediately remove local temp file
    command = "rm aux1.txt";
    if(system(command.c_str()) == 0){
        // Do Nothing
    } else {
        return_cond = true;
    }



    // Calculate MD5 of executables located on the 32-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'find "+m_mpath+"/*/ -name \"*linux.??\" -exec md5sum {} \\; >> aux2.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd32+":./aux2.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/aux2.txt";
            file2.open(current_path.c_str());
            if (file2.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file2.good()){
                    md5pair pair;
                    file2 >> line;
                    pair.num = line;
                    file2 >> line;
                    // Add 32 to the beginning of the line for later use
                    pair.path = "32"+line;
                    if(line != pair.num){
                        pairs2.push_back(pair);
                    }
                }
            } else {
                return_cond = true;
            }
            file2.close();
        } else{
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Immediately remove local and Manager files
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" rm aux2.txt";
    if(system(command.c_str()) == 0){
        command = "rm aux2.txt";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Calculate MD5 of executables located on the 64-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'find "+m_mpath+"/*/ -name \"*linux.??\" -exec md5sum {} \\; >> aux3.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd64+":./aux3.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/aux3.txt";
            file3.open(current_path.c_str());
            if (file3.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file3.good()){
                    md5pair pair;
                    file3 >> line;
                    pair.num = line;
                    file3 >> line;
                    // Add 64 to the beginning of the line for later use
                    pair.path = "64"+line;
                    if(line != pair.num){
                        pairs2.push_back(pair);
                    }
                }
            } else {
                return_cond = true;
            }
            file3.close();
        } else{
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Immediately remove local and Manager files
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" rm aux3.txt";
    if(system(command.c_str()) == 0){
        command = "rm aux3.txt";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            return_cond = true;
        }
    } else {
        return_cond = true;
    }


    // Conditional if there was a failure calculating MD5s
    if(return_cond){
        mes = "Error pertaining to the use of temporary files for calculating MD5s";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    } else {
        // Do nothing
    }

    return_cond = false;

    // Iteration over the list of machine (32 and 64 bit) module files and their MD5 hashes
    for(int i=0; i<(int)pairs2.size()-1; i++){
        // Eliminates path before the file name and stores it in a string
        s1 = elimPath(pairs2[i].path);
        // Extract "64" or "32" from the first two characters, depending on what was appended above (from 32 bit machine or 64 bit)
        machine = (pairs2[i].path).substr(0, 2);
        // Erase first two characters for future purposes
        (pairs2[i].path).erase(0,2);
        // Iteration over the list of module files on the local machine
        for(int j=0; j<(int)pairs1.size()-1; j++){
            // Eliminates path before the file name and stores it in a string
            s2 = elimPath(pairs1[j].path);
            // Defines ip Adress to be used in susbsequent commands
            if(machine == "32"){
                ipcon = ipAdd32;
            } else {
                ipcon = ipAdd64;
            }
            // Compares file names (without preceding paths) of local and non-local machines
            if(s1 == s2){

                // Checks for potential TMP directory
                temp_aux = (pairs2[i].path).substr(6,3);
                if (temp_aux == "TMP"){
                    command = "pscp -l "+user+" -pw "+pswd+" "+user+"@"+ipcon+":"+(pairs2[i].path)+" ./GR_BACKUP/TMP/";
                } else {
                    command = "pscp -l "+user+" -pw "+pswd+" "+user+"@"+ipcon+":"+(pairs2[i].path)+" ./GR_BACKUP/";
                }

                // Calls previously set command, backing up the directory in question
                if(system(command.c_str()) == 0){
                    // Remove file at specified path at the specified machine
                    command = "plink -l "+user+" -pw "+pswd+" "+ipcon+" rm "+pairs2[i].path;
                    if(system(command.c_str()) == 0){
                        // If the file was removed successfully
                        mes = "Erased "+pairs2[i].path+" at "+ipcon+"\n";
                        Mes = mes.c_str();
                        emit textResult(Mes);
                        // Set up path to place file. Looks for last "/" in the line so that the file will be added to the last directory found in the path
                        position = (pairs2[i].path).rfind("/");
                        temp_path = (pairs2[i].path).erase(position+1, (pairs2[i].path).length());
                        // Copy local file unto specified directory on the remote machine
                        command = "pscp -l "+user+" -pw "+pswd+" "+pairs1[j].path+" "+user+"@"+ipcon+":"+temp_path;
                        if(system(command.c_str()) != 0){
                            // If the file was updated successfully
                            mes = "Update to "+pairs1[j].path+" in the path: "+temp_path+" at "+ipcon+" failed.\n";
                            Mes = mes.c_str();
                            emit textError(Mes);
                            return -1;
                        } else {
                            // If the file was updated successfully
                            mes = "Updated to "+pairs1[j].path+" in the path: "+temp_path+" at "+ipcon+"\n";
                            Mes = mes.c_str();
                            emit textResult(Mes);
                        }
                    } else {
                        // If the file wasn't removed successfully
                        mes = "Failed erasing "+pairs2[i].path+" at "+ipcon+"\n";
                        Mes = mes.c_str();
                        emit textError(Mes);
                    }
                }
            }
        }
    }

    // Calculate MD5 of executables located on the 32-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" \'find "+m_mpath+"/*/ -name \"*linux.??\" -exec md5sum {} \\; >> aux2.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd32+":./aux2.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/aux2.txt";
            file2.open(current_path.c_str());
            if (file2.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file2.good()){
                    md5pair pair;
                    file2 >> line;
                    pair.num = line;
                    file2 >> line;
                    pair.path = "32"+line;
                    if(line != pair.num){
                        pairs3.push_back(pair);
                    }
                }
            } else {
                return_cond = true;
            }
            file2.close();
        } else{
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Immediately remove local and Manager files
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" rm aux2.txt";
    if(system(command.c_str()) == 0){
        command = "rm aux2.txt";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Calculate MD5 of executables located on the 32-bit machine and writes results in a temporary file
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" \'find "+m_mpath+"/*/ -name \"*linux.??\" -exec md5sum {} \\; >> aux3.txt\'";
    if(system(command.c_str()) == 0){
        // Pass the output file to local machine
        command = "pscp -l "+user+ " -pw "  +pswd+" "+user+"@"+ipAdd64+":./aux3.txt ./";
        if(system(command.c_str()) == 0){
            current_path = getCurrentPath();
            current_path = current_path+"/aux3.txt";
            file3.open(current_path.c_str());
            if (file3.is_open()){
                // While the file is open, copy each line into the specified vector using the provided md5pair structure
                while(file3.good()){
                    md5pair pair;
                    file3 >> line;
                    pair.num = line;
                    file3 >> line;
                    pair.path = "64"+line;
                    if(line != pair.num){
                        pairs3.push_back(pair);
                    }
                }
            } else {
                return_cond = true;
            }
            file3.close();
        } else{
            return_cond = true;
        }
    } else {
        return_cond = true;
    }

    // Immediately remove local and Manager files
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd64+" rm aux3.txt";
    if(system(command.c_str()) == 0){
        command = "rm aux3.txt";
        if(system(command.c_str()) == 0){
            // Do Nothing
        } else {
            return_cond = true;
        }
    } else {
        return_cond = true;
    }


    // Conditional if there was a failure calculating MD5s
    if(return_cond){
        mes = "Error pertaining to the use of temporary files for calculating MD5s.";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    } else {
        // Do nothing
    }

    // Loop through new updated MD5 numbers
    for(int i=0; i<(int)pairs3.size()-1; i++){
        s1 = elimPath(pairs3[i].path);
        // Loop through new local MD5 numbers
        for(int j=0; j<(int)pairs1.size()-1; j++){
            s2 = elimPath(pairs1[j].path);
            // Checks if the filename matches
            if(s1 == s2){
                // Should a filename match and the MD5 not, an error occurs
                if (pairs1[j].num != pairs3[i].num){
                    mes = "MD5 Check Failed.";
                    Mes = mes.c_str();
                    emit textError(Mes);
                    return -1;
                }
            }
        }
    }


    // Attempt to start the ERTMS Manager
    command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr start &";
    if(system(command.c_str()) != 0){
        mes = "System start operation at "+ipAdd32+" unsuccessfull.\n";
        Mes = mes.c_str();
        emit textError(Mes);
        return -1;
    } else {
        mes = "System start operation at "+ipAdd32+" successfull.\n";
        Mes = mes.c_str();
        emit textResult(Mes);
        // Attempt to update the ERTMS Manager
        command = "plink -l "+user+" -pw "+pswd+" "+ipAdd32+" gr update";
        if(system(command.c_str()) != 0){
            mes = "System start operation at "+ipAdd32+" unsuccessfull.\n";
            Mes = mes.c_str();
            emit textError(Mes);
            return -1;
        } else {
            mes = "System update operation at "+ipAdd32+" successfull.\n";
            Mes = mes.c_str();
            emit textResult(Mes);
        }
    }

    // Final approval message
    mes = "Executables updated successfully.\n";
    Mes = mes.c_str();
    emit textResult(Mes);


    return 0;
}


/*
****************************************************************************************************************************

executeAll:

This function executes the chosen option on all machines described in the configuration file.


****************************************************************************************************************************
*/


void connectionsWindow::executeAll(){

    string command;
    string line;
    string found_id;
    string cut_leu;
    string notices;
    string current_path;
    QString noticeMes;
    vector<string> vec_ids;
    vector<string> vec_aux;
    ifstream file;
    bool string_in;


    if(optionNum == 1){

        // StatusBar update and a quick information message to be confirmed by the user to continue
        QMainWindow::statusBar()->showMessage(tr("Updating LEU..."));
        notices = "This may take a few minutes, please wait until the operation is finished. \n\nClick \"OK\" or close this window to continue.\n";
        noticeMes = notices.c_str();
        QMessageBox::information(this, "Notice", noticeMes);

        // Find all files associated with the specified LEU id
        command = "find "+dpath+"/* -name \"*"+leu+"*\" >> temp.txt";
        if(system(command.c_str()) == 0){
            // Open temporary file
            current_path = getCurrentPath();
            current_path = current_path+"/temp.txt";
            file.open(current_path.c_str());
            // While the file is open, retrieve the IDs of the machines the files are associated with
            while(file.good()){
                file >> line;
                // Should always start with .
                if(line.substr(0,1) == "."){
                    // retrieve machine ID by grabbing the ID number associated to the folder in which the file was found, ignoring the path before that
                    found_id = line.substr(dpath.size()+1, 8);
                    // Checks to see if the id is already in the vector, to avoid repeat instances
                    if(vec_ids.empty()){
                        vec_ids.push_back(found_id);
                    } else {
                        string_in = false;
                        for(int i=0; i<(int)vec_ids.size(); i++){
                            if(vec_ids[i] == found_id){
                                string_in = true;
                            }
                        }
                        if(!string_in){
                            // Should the id not be in the vector, we add it
                            vec_ids.push_back(found_id);
                        }
                    }
                }
            }
        }

        // Immediately remove temporary file.
        command = "rm temp.txt";
        if(system(command.c_str()) == 0){

        }


        // Iteration through the ids associated with the LEU
        for(int i=0; i<(int)vec_ids.size(); i++){
            // Iteration through the list of ERTMS machines to find the associated machine
            for(int k=0; k<e_count; k++){
                if(vec_ids[i] == elist[k].id){
                    // Retieve collaterals
                    vec_aux = elist[k].colIds;
                    // Run option on all collaterals (itself included)
                    for(int b=0; b<elist[k].col_num; b++){
                        for(int a=0; a<e_count; a++){
                            if(elist[a].id == vec_aux[b]){
                                execeOption(elist[a]);
                            }
                        }
                    }
                }
            }
        }

    } else if(optionNum == 2){

        // StatusBar update and a quick information message to be confirmed by the user to continue
        QMainWindow::statusBar()->showMessage(tr("Updating RBC..."));
        notices = "This may take a few minutes, please wait until the operation is finished. \n\nClick \"OK\" or close this window to continue.\n";
        noticeMes = notices.c_str();
        QMessageBox::information(this, "Notice", noticeMes);

        // Find the IF2 id an run specifically from that machine.
        for(int i=0; i<i_count;i++){
            if(ilist[i].id == leu){
                execeOption(ilist[i]);
            }
        }


    } else {
        // Run on all ERTMS machines
        for(int i=0; i<e_count; i++){
            execeOption(elist[i]);
        }
        // Run on all IF2 machines, provided we aren't updating DATACOREs
        if(optionNum != 0){
            for(int i=0; i<i_count; i++){
                execeOption(ilist[i]);
            }
        }
    }
}


/*
****************************************************************************************************************************

prepareExecutionE:

This function simply recieves the mapped integer sent by a button and uses it to execute the desired option from the
correct ERTMS machine.


****************************************************************************************************************************
*/


void connectionsWindow::prepareExecutionE(int num){
    execeOption(elist[num]);
}

/*
****************************************************************************************************************************

prepareExecutionI:

This function simply recieves the mapped integer sent by a button and uses it to execute the desired option from the
correct IF2 machine.


****************************************************************************************************************************
*/

void connectionsWindow::prepareExecutionI(int num){
    execeOption(ilist[num]);
}


/*
****************************************************************************************************************************

executeOption:

This function executes the chosen option on one particular machine described in the configuration file, as requested
by the user.


****************************************************************************************************************************
*/
void connectionsWindow::execeOption(E_machine machine){

    // Declare Variables
    QImage imageGreen("CheckMark.png");
    imageGreen = imageGreen.scaled((imageGreen.size()*0.2), Qt::KeepAspectRatio);
    QLabel *colorImg;
    string ipAdd64;
    string ipAdd32;
    string id;
    string name;
    string m_dpath;
    string m_mpath;
    string errors;
    vector<string> vec_aux;
    QString errorMes;
    string notices;
    QString noticeMes;

    // Initialize variables pertaining to the Manager
    ipAdd64 = machine.ip_64;
    ipAdd32 = machine.ip_32;
    colorImg = machine.imageE;
    id = machine.id;
    name = machine.name;
    m_dpath =  machine.data_path;
    m_mpath = machine.exe_path;

    // List possible options
    if(optionNum == 0){

        // StatusBar update and a quick information message to be confirmed by the user to continue
        QMainWindow::statusBar()->showMessage(tr("Updating datacores..."));
        notices = "This may take a few minutes, please wait until the operation is finished. \n\nClick \"OK\" or close this window to continue.\n";
        noticeMes = notices.c_str();
        QMessageBox::information(this, "Notice", noticeMes);
        // Check to see if the provided ID is a Level 2 machine (RBC) or not using previously calculated boolean
        // Execute the Update Datacore option
        if(updateDC(ipAdd64, ipAdd32, id, name, m_dpath) !=0){
            errors = "Datacore update at \n"+ id +"\n was unsuccessfull\n";
            errorMes = errors.c_str();
            QMessageBox::critical(this, "Connection Error", errorMes);
        } else {
            colorImg->setPixmap(QPixmap::fromImage(imageGreen));
            QMainWindow::statusBar()->showMessage(tr("Done updating datacores..."));
        }


    } else if(optionNum == 1){

        // Execute the Update LEU option
        if(updateLEU(ipAdd64, ipAdd32, id, name, m_dpath) !=0){
            errors = "LEU Update was unsuccessfull\n";
            errorMes = errors.c_str();
            QMessageBox::critical(this, "Connection Error", errorMes);
        } else {
            colorImg->setPixmap(QPixmap::fromImage(imageGreen));
            QMainWindow::statusBar()->showMessage(tr("Done updating LEU..."));
        }


    } else if(optionNum == 2){

        // Execute the Update LEU option
        if(updateRBC(ipAdd64, ipAdd32, name, m_dpath) !=0){
            errors = "LEU Update was unsuccessfull\n";
            errorMes = errors.c_str();
            QMessageBox::critical(this, "Connection Error", errorMes);
        } else {
            colorImg->setPixmap(QPixmap::fromImage(imageGreen));
            // Iteration that updates the
            for(int i=0; i<i_count; i++){
                if(ilist[i].id == leu){
                    vec_aux = ilist[i].colIds;
                    for(int j=0; j<e_count; j++){
                        for(int k=0; k<ilist[i].col_num; k++){
                            if(elist[j].id == vec_aux[k]){
                                elist[j].imageE->setPixmap(QPixmap::fromImage(imageGreen));
                            }
                        }
                    }
                }
            }
            QMainWindow::statusBar()->showMessage(tr("Done updating LEU..."));
        }


    } else if(optionNum == 3){

        // StatusBar update and a quick information message to be confirmed by the user to continue
        QMainWindow::statusBar()->showMessage(tr("Updating executable files..."));
        notices = "This may take a few minutes, please wait until the operation is finished. \n\nClick \"OK\" or close this window to continue.\n";
        noticeMes = notices.c_str();
        QMessageBox::information(this, "Notice", noticeMes);

        // Execute the Update Executables option
        if(updateExecutables(ipAdd64, ipAdd32, name, m_mpath) !=0){
            errors = "Executables update was unsuccessfull\n";
            errorMes = errors.c_str();
            QMessageBox::critical(this, "Connection Error", errorMes);
        } else {
            colorImg->setPixmap(QPixmap::fromImage(imageGreen));
            QMainWindow::statusBar()->showMessage(tr("Done updating executable files..."));
        }


    } else if(optionNum == 4){

        // StatusBar update and a quick information message to be confirmed by the user to continue
        QMainWindow::statusBar()->showMessage(tr("Generating version information..."));
        notices = "This may take a few minutes, please wait until the operation is finished. \n\nClick \"OK\" or close this window to continue.\n";
        noticeMes = notices.c_str();
        QMessageBox::information(this, "Notice", noticeMes);

        // Execute the Display Version option
        if(displayVersion(ipAdd64, ipAdd32, name, m_mpath) !=0){
            errors = "Version Display was unsuccessfull\n";
            errorMes = errors.c_str();
            QMessageBox::critical(this, "Connection Error", errorMes);
        } else {
            colorImg->setPixmap(QPixmap::fromImage(imageGreen));
            QMainWindow::statusBar()->showMessage(tr("Version information generated..."));
        }


    } else if(optionNum == 5){

        // StatusBar update and a quick information message to be confirmed by the user to continue
        QMainWindow::statusBar()->showMessage(tr("Shutting down systems..."));
        notices = "This may take a few minutes, please wait until the operation is finished. \n\nClick \"OK\" or close this window to continue.\n";
        noticeMes = notices.c_str();
        QMessageBox::information(this, "Notice", noticeMes);

        // Execute the Shutdown option
        if(shutdown(ipAdd32) != 0){
            errors = "ERTMS Shutdown at \n"+ id +"\n was unsuccessfull\n";
            errorMes = errors.c_str();
            QMessageBox::critical(this, "Connection Error", errorMes);
        } else {
            colorImg->setPixmap(QPixmap::fromImage(imageGreen));
            QMainWindow::statusBar()->showMessage(tr("Done shutting down systems..."));
        }
    } else if(optionNum == 6){

        // StatusBar update and a quick information message to be confirmed by the user to continue
        QMainWindow::statusBar()->showMessage(tr("Restarting systems..."));
        notices = "This may take a few minutes, please wait until the operation is finished. \n\nClick \"OK\" or close this window to continue.\n";
        noticeMes = notices.c_str();
        QMessageBox::information(this, "Notice", noticeMes);

        QMainWindow::statusBar()->showMessage("Preforming System Reboot...");

        // Execute the Reboot option
        if(reboot(ipAdd32) != 0){
            errors = "ERTMS Reboot at \n"+ id +"\n was unsuccessfull\n";
            errorMes = errors.c_str();
            QMessageBox::critical(this, "Connection Error", errorMes);
        } else {
            colorImg->setPixmap(QPixmap::fromImage(imageGreen));
            QMainWindow::statusBar()->showMessage(tr("Done rebooting systems..."));
        }


    } else {
        // Option out of range
        errors = "Option unavailable \n";
        errorMes = errors.c_str();
        QMessageBox::critical(this, "Connection Error", errorMes);
    }

}

