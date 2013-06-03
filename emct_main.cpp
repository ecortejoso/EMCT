#include "emct_main.h"
#include "connections_window.h"
using namespace std;



/*
****************************************************************************************************************************

emct_main:

This is the constructor method for the class emct_main. It establishes the UI and connections neccesary for usability.


****************************************************************************************************************************
*/


emct_main::emct_main(){
    setupUi(this); // this sets up GUI

    // signals/slots mechanism in action
    connect( pushButton_execute, SIGNAL( clicked() ), this, SLOT( executeOption() ) );
    connect( pushButton_Browse, SIGNAL( clicked() ), this, SLOT( getPath() ) );
    connect( pushButton_Save, SIGNAL( clicked() ), this, SLOT( saveLog() ) );
    connect( quit, SIGNAL(triggered()), this, SLOT(close()));
    connect( actionHelp, SIGNAL(triggered()), this, SLOT(helpPdf()));

    QMainWindow::statusBar()->showMessage(tr("Ready"));
    setWindowIcon(QIcon("train.png"));
}


/*
****************************************************************************************************************************

logAppend:

This method appends a normal message to the main windows log.


****************************************************************************************************************************
*/


void emct_main::logAppend(const QString &text){
    log->setTextColor(QColor(0,0,0, 255));
    log->setFontUnderline(false);
    log->append(text);
}


/*
****************************************************************************************************************************

logAppendError:

This method appends an error message to the main windows log.


****************************************************************************************************************************
*/


void emct_main::logAppendError(const QString &text){
    log->setTextColor(QColor(255,0,0, 255));
    log->setFontUnderline(false);
    log->append(text);
}


/*
****************************************************************************************************************************

logAppendOp:

This method appends an operation title message to the main windows log.


****************************************************************************************************************************
*/


void emct_main::logAppendOp(const QString &text){
    log->setTextColor(QColor(0,200,56, 255));
    log->setFontUnderline(false);
    log->append(text);
}


/*
****************************************************************************************************************************

changeEStatus:

This message changes the status message at the bottom of the main Window.


****************************************************************************************************************************
*/


void emct_main::changeEStatus(string s){

    QString qsaux;
    qsaux = s.c_str();
    QMainWindow::statusBar()->showMessage(qsaux);

}


/*
****************************************************************************************************************************

disableButtons:

This function disables all relevant buttons on the main window.


****************************************************************************************************************************
*/


void emct_main::disableButtons(){
    pushButton_Browse->setEnabled(false);
    pushButton_Save->setEnabled(false);
    pushButton_execute->setEnabled(false);
}


/*
****************************************************************************************************************************

disableButtons:

This function enables all relevant buttons on the main window and sets the status to "Ready..."


****************************************************************************************************************************
*/


void emct_main::enableButtons(){
    pushButton_Browse->setEnabled(true);
    pushButton_Save->setEnabled(true);
    pushButton_execute->setEnabled(true);
    QMainWindow::statusBar()->showMessage(tr("Ready..."));
}


/*
****************************************************************************************************************************

closeEvent:

This function handles what happens when the main window is closed.


****************************************************************************************************************************
*/


void emct_main::closeEvent(QCloseEvent *event){
    emit exitChild();
    event->accept();
    exit(0);
}


/*
****************************************************************************************************************************

helpPdf:

This function accesses the help PDF file.


****************************************************************************************************************************
*/



void emct_main::helpPdf(){
    string current_path;
    current_path = getCurrentPath();
    current_path = "evince "+current_path+"/help_EMCT.pdf";
    if(system(current_path.c_str()) != 0){
        QMessageBox::critical(this, "Help Error", "There was an unexpected error opening the help file \n");
    }

}


/*
****************************************************************************************************************************

saveLog:

This function saves the text in the application window as a text file.


****************************************************************************************************************************
*/


void emct_main::saveLog(){

    // Declar Variables
    QString path;
    QByteArray bytes;
    const char * ptr;
    ofstream file;
    QString text;
    // Show new status
    QMainWindow::statusBar()->showMessage(tr("Saving"));

    // Open window to search for path
    path = QFileDialog::getSaveFileName(
        this,
        "Choose a directory to save into:",
        QString::null,
        tr("Text Documents (*.txt)"));

    // Handle path information
    bytes = path.toAscii();
    ptr = bytes.data();
    file.open(ptr);
    // Retrieves information from the log
    text = log->toPlainText();
    // Write to file.
    file << text.toStdString();
    file.close();
    QMainWindow::statusBar()->showMessage(tr("Ready"));

}

/*
****************************************************************************************************************************

getPath:

This function simply opens a file browser window to specify the path of the configuration file the user desires.


****************************************************************************************************************************
*/


void emct_main::getPath(){
    QString path;
    QMainWindow::statusBar()->showMessage(tr("Getting Path..."));
    path = QFileDialog::getOpenFileName(
        this,
        "Choose a file to open",
        QString::null,
        tr("Configuration Files (*.cnf)"));

    configFile->setText( path );
    QMainWindow::statusBar()->showMessage(tr("Ready..."));


}


/*
****************************************************************************************************************************

readConfigFile:

This function reads the configuration file specified bny the user.

If there are no errors, it returns 1, otherwise it returns 0.


****************************************************************************************************************************
*/


int emct_main::readConfigFile(){

    // Declare variables
    QString path;
    string line;
    QByteArray bytes;
    const char * ptr;
    string line_name;
    string dc_path;
    vector<string> leus_aux;
    string cs_sub;
    bool non_yet;
    string res;
    string command;
    int ertms_num;
    int if2_num;
    bool exit_loop_aux;
    int col_count;
    string colat;
    string current_path;

    // Initialize 4 "counter" ints. e_count and i_count will be the final numbers of ERTMS/IF2 machines read.
    ertms_num = 0;
    e_count = 0;
    if2_num = 0;
    i_count = 0;

    // Grab current path set on the "Configuration File" line and open the file.
    path = configFile->text();
    ifstream file;
    bytes = path.toAscii();
    ptr = bytes.data();
    file.open(ptr);
    if (file.is_open()){
        // Read "header" information from file. There should be 5 lines.
        for (int i=0; i<7; i++){
            if(file.good()){
                if(i==0){
                    getline(file, line);
                    line_name = line;
                } else {
                    file >> line;
                    // Handle if the user tries to assign the Datacore_path
                    if("Datacore_path" == line){
                        file >> line;
                        if("=" == line){
                            file >> line;
                            dpath = line;
                        } else {
                            QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring Datacore Path in your Configuration File. \n");
                            file.close();
                            return 0;
                        }
                    // Handle if the user tries to assign Module_Path
                    } else if ("Module_path" == line){
                        file >> line;
                        if("=" == line){
                            file >> line;
                            mpath = line;
                        } else {
                            QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring Datacore Path in your Configuration File. \n");
                            file.close();
                            return 0;
                        }

                    // Handle if the user tries to assign ERTMS_IDs
                    } else if ("ERTMS_IDs" == line) {
                        file >> line;
                        bool exit_loop = true;

                        if("=" == line){
                            // Run loop reading different ERTMS Manager IDs
                            while (exit_loop){
                            E_machine m;
                            file >> line;
                            ertms_num++;
                                // Case for a non-last ID on the list.
                                if(',' == line[line.length()-1]){
                                    // eliminate the ',', create a "machine" struct and add to list with ID
                                    line.erase(line.length()-1);
                                    m.id = line;
                                    elist.push_back(m);
                                // Case for the last ID in the list.
                                } else {
                                    // set loop conditional to exit, create a "machine" struct and add to list with ID
                                    exit_loop = false;
                                    m.id = line;
                                    elist.push_back(m);
                                }
                            }
                        } else {
                            QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring ERTMS IDs in your Configuration File. \n");
                            file.close();
                            return 0;
                        }

                    // Handle if the user tries to assign the IF2_IDs
                    } else if ("IF2_IDs" == line) {
                        file >> line;
                        bool exit_loop = true;

                        if("=" == line){
                            // Run loop reading different ERTMS Manager IDs
                            while (exit_loop){
                            E_machine m;
                            file >> line;
                            if2_num++;
                                // Case for a non-last ID on the list.
                                if(',' == line[line.length()-1]){
                                    // eliminate the ',', create a "machine" struct and add to list with ID
                                    line.erase(line.length()-1);
                                    m.id = line;
                                    ilist.push_back(m);
                                // Case for the last ID in the list.
                                } else {
                                    // set loop conditional to exit, create a "machine" struct and add to list with ID
                                    exit_loop = false;
                                    m.id = line;
                                    ilist.push_back(m);
                                }
                            }
                        } else {
                            QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring ERTMS IDs in your Configuration File. \n");
                            file.close();
                            return 0;
                        }

                    // Handle if the user tries to assign the user
                    } else if ("user" == line){
                        file >> line;
                        if("=" == line){
                            file >> line;
                            user = line;
                        } else {
                            QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring Datacore Path in your Configuration File. \n");
                            file.close();
                            return 0;
                        }

                    // Handle if the user tries to assign the password
                    } else if("password" == line){
                        file >> line;
                        if("=" == line){
                            file >> line;
                            pswd = line;
                        } else {
                            QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring Datacore Path in your Configuration File. \n");
                            file.close();
                            return 0;
                        }
                    // Error handle
                    } else {
                        QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring global variables in your Configuration File. \n");
                        file.close();
                        return 0;
                    }
                }
            } else {
                QMessageBox::critical(this, "Configuration File Read Error", "Could not open specified Configuration File. \n");
                file.close();
                return 0;
            }
        }

        // proceed to read information pertaining to the machines
        // Loop read a line while the file is not NULL or closed
        while(file.good()){
            file >> line;
            // If the line ends with a ']', it means it represents the id of the machine
            if(']' == line[line.length()-1]){
                // Eliminate the [  and ] and use that to compare to the ERTMS IDs saved previously.
                line.erase(line.length()-1);
                line.erase(0, 1);
                int i;
                bool main_bool = true;
                // Loop through the ERTMS machines to assign the information read
                for(i=0; i<ertms_num; i++){
                    //cout << line+"\n";
                    // See id read matches one on the list and see if the breaking condition of not declaring collaterals last fails.
                    if(line == elist[i].id && main_bool){
                        e_count++;
                        // There are, currently, 6 posible statements the user can input.
                        for(int a=0; a<9; a++){
                            file >> line;
                            // User wants to declare the Managers 64-bit machine IP address
                            if ("IP64" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring IP addresses in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                elist[i].ip_64 = line;
                            // User wants to declare the Managers 32-bit machine IP address
                            } else if ("IP32" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring IP addresses in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                elist[i].ip_32 = line;
                            // User wants to declare the Managers 64-bit machine name
                            } else if ("M_name64" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring a machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                elist[i].m64 = line;
                            // User wants to declare the Managers 32-bit machine name
                            } else if ("M_name32" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring a machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                elist[i].m32 = line;
                            // User wants to declare the Managers system Name
                            } else if ("Name" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring the main machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                elist[i].name = line;
                            // User wants to declare realtive Datacore path for this machine
                            } else if ("Datacore_path" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring the main machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                elist[i].data_path = line;
                            // User wants to declare realtive Module executables path for this machine
                            } else if ("Module_path" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring the main machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                elist[i].exe_path = line;
                            // User wants to declare the PLO associated to the machine
                            } else if ("PLO_R" == line) {
                                plo_r plo_aux;
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring a PLO in your Configuration File. \n");
                                    return 0;
                                }
                                for(int k=0; k<3; k++){
                                    file >> line;
                                    if(',' == line[line.length()-1]){
                                        line.erase(line.length()-1);
                                        if(k==0){
                                            plo_aux.ip = line;
                                        } else if (k==1){
                                            plo_aux.user = line;
                                        } else {
                                            plo_aux.pswd = line;
                                        }
                                    }
                                }
                                file >> line;
                                plo_aux.data_path = line;
                                elist[i].plo = plo_aux;
                            // User wants to declare the Managers that are collateral to this one
                            } else if ("Colaterals" == line) {
                                file >> line;
                                if (a == 8){
                                    main_bool = false;
                                }
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring colaterals for an ERTMS in your Configuration File. \n");
                                    return 0;
                                }
                                // Initialize loop specific paramaters, a conditional and a counter
                                exit_loop_aux = true;
                                col_count=0;
                                // Loop to read collaterals
                                while (exit_loop_aux){
                                    file >> line;
                                    // Case for any non-last ID in the list
                                    if(',' == line[line.length()-1]){
                                        line.erase(line.length()-1);
                                        colat = line;
                                        (elist[i].colIds).push_back(colat);
                                    // Case for any last ID in the list
                                    } else {
                                        exit_loop_aux = false;
                                        colat = line;
                                        (elist[i].colIds).push_back(colat);
                                    }
                                    col_count++;
                                }
                                // assigns number of collaterals read to the info stored for the ERTMS Manager machine
                                elist[i].col_num = col_count;
                            } else {
                                // Unexepected variable declaration error
                                QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring variables in an ERTMS in your Configuration File. \n");
                                file.close();
                                return 0;
                            }
                        }
                    } else {
                        // Do Nothing
                    }
                }

                main_bool = true;
                for(i=0; i<if2_num; i++){

                    // See id read matches one on the list and see if the breaking condition of not declaring collaterals last fails.
                    if(line == ilist[i].id && main_bool){
                        i_count++;
                        // There are, currently, 6 posible statements the user can input.
                        for(int a=0; a<8; a++){
                            file >> line;
                            // User wants to declare the IF2s 64-bit machine IP address
                            if ("IP64" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring IP addresses in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                ilist[i].ip_64 = line;
                            // User wants to declare the IF2s 32-bit machine IP address
                            } else if ("IP32" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring IP addresses in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                ilist[i].ip_32 = line;
                            // User wants to declare the IF2s 64-bit machine name
                            } else if ("M_name64" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring a machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                ilist[i].m64 = line;
                            // User wants to declare the IF2s 32-bit machine name
                            } else if ("M_name32" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring a machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                ilist[i].m32 = line;
                            // User wants to declare the IF2s system Name
                            } else if ("Name" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring the main machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                ilist[i].name = line;
                            // User wants to declare realtive Datacore path for this machine
                            } else if ("Datacore_path" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring the main machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                ilist[i].data_path = line;
                            // User wants to declare realtive Module executables path for this machine
                            } else if ("Module_path" == line) {
                                file >> line;
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring the main machine name in your Configuration File. \n");
                                    return 0;
                                }
                                file >> line;
                                ilist[i].exe_path = line;
                            // User wants to declare the IF2s that are collateral to this one
                            } else if ("Colaterals" == line) {
                                file >> line;
                                if (a == 7){
                                    main_bool = false;
                                }
                                if(line != "="){
                                    file.close();
                                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring colaterals for an ERTMS in your Configuration File. \n");
                                    return 0;
                                }
                                // Initialize loop specific paramaters, a conditional and a counter
                                exit_loop_aux = true;
                                col_count=0;
                                // Loop to read collaterals
                                while (exit_loop_aux){
                                    file >> line;
                                    // Case for any non-last ID in the list
                                    if(',' == line[line.length()-1]){
                                        line.erase(line.length()-1);
                                        colat = line;
                                        (ilist[i].colIds).push_back(colat);
                                    // Case for any last ID in the list
                                    } else {
                                        exit_loop_aux = false;
                                        colat = line;
                                        (ilist[i].colIds).push_back(colat);
                                    }
                                    col_count++;
                                }
                                // assigns number of collaterals read to the info stored for the IF2 machine
                                ilist[i].col_num = col_count;
                            } else {
                                // Unexepected variable declaration error
                                cout << line;
                                QMessageBox::critical(this, "Configuration File Read Error", "There was an unexpected error declaring variables in an IF2 in your Configuration File. \n");
                                file.close();
                                return 0;
                            }
                        }
                    } else {
                        // Do Nothing
                    }
                }


            } else {
                // A final check to ensure that the correct number of machines was read.
                if(ertms_num != e_count || if2_num != i_count){
                    QMessageBox::critical(this, "Configuration File Read Error", "There was an error declaring IP addresses in your Configuration File. \n");
                    file.close();
                    return 0;
                } else {
                    // Do nothing
                }
            }
        }
        file.close();
    }
    else {
        QMessageBox::critical(this, "Configuration File Read Error", "There was an error reading your Configuration File.\n");
        return 0;
    }


    // Reading the current list of local LEUs to be able to list should the user want to update a LEU


    // List all DATACORE files in a temp file
    command = "ls ./DATACORE/* >> leuhelp.txt";
    if(system(command.c_str()) == 0){
        // Open the temp file

        current_path = getCurrentPath();
        current_path = current_path+"/leuhelp.txt";
        file.open(current_path.c_str());
        if (file.is_open()){
            leus_aux.push_back("00000000");
            // Loop for reading subsequent files
            while(file.good()){
                file >> line;
                // If the first character is a '.', it denotes that it is a path, thus it is ignored
                if (line.substr(0,1) != "."){
                    // Run the the current list of LEUs read
                    for(int j=0; j<(int)leus_aux.size(); j++){
                        // Initialze bool meant to show if the current LEU has already been read, to avoid repetition
                        non_yet = false;

                        size_t point = line.find_first_of('.');
                        if(line.substr(point+1,3) == "dc2"){
                            non_yet = true;
                        }

                        // Checks for the 'LEU-' prefix and extracts LEU accordingly
                        if(line.substr(0,4) == "LEU-"){
                            res = line.substr(4,8);
                            // Loop to check if current LEU has been read
                            for(int k=0; k<(int)leus_aux.size(); k++){
                                if(leus_aux[k] == res){
                                    // Condition showing that LEU has already been read, thus sets the boolean as true
                                    non_yet = true;
                                } else {
                                    // Do Nothing
                                }
                            }
                        } else {
                            res = line.substr(0,8);
                            // Loop to check if current LEU has been read
                            for(int k=0; k<(int)leus_aux.size(); k++){
                                if(leus_aux[k] == res){
                                    // Condition showing that LEU has already been read, thus sets the boolean as true
                                    non_yet = true;
                                } else {
                                    // Do Nothing
                                }
                            }
                        }
                        for(int k=0; k<i_count; k++){
                            if(ilist[k].id == res){
                                non_yet = true;
                            }
                        }
                        // Should the boolean be true (LEU already read), it will not add it to the list, otherwise it adds the LEU
                        if(!non_yet){
                            leus_aux.push_back(res);
                        } else {
                            // Do Nothing
                        }
                    }
                } else {
                    // Do Nothing
                }
            }

        }
        file.close();
    }

    // Assigns final list to the main class LEU list
    leulist = leus_aux;

    // Remove the temp file
    command = "rm leuhelp.txt";
    if(system(command.c_str()) == 0){
        // Do Nothing
    } else {
        // Do Nothing
    }


    return 1;

}


/*
****************************************************************************************************************************

readVersionFile:

This function reads the version file specified bny the user.

If there are no errors, it returns 1, otherwise it returns 0.


****************************************************************************************************************************
*/


int emct_main::readVersionFile(){

    // Declare variables
    QString path;
    string line;
    string current_path;
    vector<string> pro_ids;
    ifstream file;
    bool is_in;

    // Open the version file
    current_path = getCurrentPath();
    current_path = current_path+"/versions.vrs";
    file.open(current_path.c_str());
    if (file.is_open()){
        if(file.good()){
            file >> line;
            // Assures the first line is [PRODUCTOS]
            if(line != "[PRODUCTS]"){
                return -1;
            }
            file >> line;
            // The next line should be "Number_Products" with a associated number
            if(line != "Number_Products"){
                return -1;
            } else {
                file >> line;
                if (line == "="){
                    file >> line;
                    pro_num = atoi(line.c_str());
                } else{
                    return -1;
                }
            }

            // Loop to read products and it's associated version and module information
            for (int i = 0; i<pro_num; i++){

                //Declare variable for each loop, this is intentional so that the buffers do not coincide
                stringstream ss;
                ss << i;
                string compare_line = "v"+ss.str();

                // This begins reading product version IDs, always started aith a 'v' followed by a sequential number (0,1,2,3... etc) in order
                file >> line;
                if(line != compare_line){
                    return -1;
                } else {
                    file >> line;
                    if (line == "="){
                        file >> line;
                        pro_ids.push_back(line);
                    } else{
                        return -1;
                    }
                }

            }

            // Loop to find executable files information product by product
            for(int i = 0; i<pro_num; i++){

                // Declare variables within this scope to avoid memory trouble
                vector<md5pair> l_pairs;
                producto product;

                is_in = false;
                // Read ID of product and remove brackets at each end
                file >> line;
                line.erase(line.length()-1);
                line.erase(0, 1);
                // Loop to check if the ID was declared in the previous section. If not, an error is produced using the is_in boolean
                for (int j = 0; j<pro_num; j++){
                    if (line == pro_ids[j]){
                        is_in = true;
                        product.id = line;
                    } else {
                        continue;
                    }
                }
                // Reads the "Version" parameter first
                file >> line;
                if(line == "Version" && is_in){
                    file >> line;
                    if(line != "="){
                        return -1;
                    } else {
                        file >> line;
                        //If line format is successfull, assign the version to this loops product
                        product.version = line;
                    }
                } else {
                    return -1;
                }
                // Read Parameter name, it can be anything specified by the user.
                file >> line;
                // Reads module parameters, following parameter after these is "Name", thus the condition for the while
                while(line != "Name"){
                    // Declare internal md5pair structure
                    md5pair pair;
                    file >> line;
                    if(line != "="){
                        return -1;
                    } else {
                        // Reads executable name, it should have a ',' following it to follow up with its MD5
                        file >> line;
                        // Check for ',' following he executable name
                        if((line[line.length()-1]) != ','){
                            return -1;
                        } else{
                            //Erase the ','
                            line.erase(line.length()-1);
                            // Assign executable name
                            pair.path = line;
                            // Get and assign MD5 number
                            file >> line;
                            pair.num = line;
                        }
                    }
                    // Adds the read executable to the list
                    l_pairs.push_back(pair);
                    // Read the next Parameter name, it can be anything specified by the user.
                    file >> line;
                }
                // Assign the list of executables information to the product structure
                product.pro_ids = l_pairs;

                // Las line read should be "Name" thus the exit of the loop and we proceed to process the product Name
                file >> line;
                if (line != "="){
                    return -1;
                } else {
                    // Retrieve and assign name to the product structure
                    file >> line;
                    product.name = line;
                }
                // Add product to the list of products
                list_products.push_back(product);

            }

        }
        file.close();
    } else {
        return -1;
    }

return 0;

}


/*
****************************************************************************************************************************

openCon:

This function opens the Connection Window via the "Choose LEU" window.


****************************************************************************************************************************
*/


void emct_main::openCon(QWidget *widg){

    // Declare Variables

    QComboBox *txtLEU;
    QWidget *introLEU;
    QString helperString;
    QString errorMes;
    string errors;
    connectionsWindow *connectWindow;

    // Grab the mapping passed in form of a QWidget to a QComboBox
    txtLEU = (QComboBox*) widg;
    // Obtains the parent, main Widget in which the ComboBox is contained
    introLEU = txtLEU->parentWidget();
    // Obtains current selection of the ComboBox
    helperString = txtLEU->currentText();

    // Assures the the chosen option has the correct length for a LEU (8 digits)
    if(helperString.length() == 8){

        leu = helperString.toStdString();
        // Closes "Choose LEU window"
        introLEU->close();
        // Sets up the connections Window and shows it
        connectWindow = new connectionsWindow(this);
        connect(this, SIGNAL (exitChild()), connectWindow ,SLOT (close()));
        connectWindow->show();
    } else {
        errors = "The LEU ID number introduced : \n \n "+helperString.toStdString()+"\n \n is not a valid LEU ID, please try again.\n";
        errorMes = errors.c_str();
        QMessageBox::critical(this, "Invalid LEU ID number", errorMes);
        introLEU->close();
    }


}



/*
****************************************************************************************************************************

executeOption:

This function carries out the specified option in the application window, opening the connections window.


****************************************************************************************************************************
*/

void emct_main::executeOption(){

    // Declare Variables
    connectionsWindow *connectWindow;
    QComboBox *txtLEU;
    QWidget *introLEU;
    QSignalMapper *sm;
    QPushButton *butLEU;
    string saux;
    QString qsaux;
    QLabel *labLEU;
    QVBoxLayout *mainLayout;
    QHBoxLayout *lineLayout;
    QString errorMes;
    string errors;

    // Update Status Bar
    QMainWindow::statusBar()->showMessage("Reading...");

    // Read the specified Configuration File
    if (readConfigFile() != 0){
        // Disable Buttons to ensure user cannot execute multiple commands at once
        disableButtons();
        // Option specified is the current one shown in the ComboBox
        optionNum = chooseOption->currentIndex();
        //Clear log to avoid overlap with previous operations
        log->clear();
        // If the option is to update a LEU
        if (optionNum == 1){

            // Initialize relevant variables
            introLEU =  new QWidget;
            butLEU = new QPushButton("OK", introLEU);
            saux = "Please choose the id number of the LEU you wish to update and press \'OK\'";
            qsaux = saux.c_str();
            txtLEU = new QComboBox;
            labLEU = new QLabel(qsaux);
            sm = new QSignalMapper(this);

            // Create list of possible LEUs to update and place them in a corresponding ComboBox
            for(int i=0; i<(int)leulist.size(); i++){
                saux = leulist[i];
                qsaux = saux.c_str();
                txtLEU->addItem(qsaux);
            }

            // Connect button to the new window, transmitting the chosen LEU to OpenCon
            sm->setMapping(butLEU, (QWidget*) txtLEU);
            connect( butLEU, SIGNAL( clicked() ), sm, SLOT( map() ) );
            connect(sm, SIGNAL(mapped(QWidget *)), this, SLOT(openCon(QWidget *)));

            // Layout and UI calls for a new window to choose a LEU
            mainLayout = new QVBoxLayout;
            lineLayout = new QHBoxLayout;
            lineLayout->addWidget(txtLEU);
            lineLayout->addWidget(butLEU);
            mainLayout->addWidget(labLEU);
            mainLayout->addLayout(lineLayout);
            introLEU->setLayout(mainLayout);

            // Show LEU choosing Window
            introLEU->show();

        // If the option is to update a RBC
        } else if (optionNum == 2){

            // Initialize relevant variables
            introLEU =  new QWidget;
            butLEU = new QPushButton("OK", introLEU);
            saux = "Please choose the id number of the RBC you wish to update and press \'OK\'";
            qsaux = saux.c_str();
            txtLEU = new QComboBox;
            labLEU = new QLabel(qsaux);
            sm = new QSignalMapper(this);

            // Create list of possible RBCs to update and place them in a corresponding ComboBox
            for(int i=0; i<i_count; i++){
                saux = ilist[i].id;
                qsaux = saux.c_str();
                txtLEU->addItem(qsaux);
            }

            // Connect button to the new window, transmitting the chosen RBC to OpenCon
            sm->setMapping(butLEU, (QWidget*) txtLEU);
            connect( butLEU, SIGNAL( clicked() ), sm, SLOT( map() ) );
            connect(sm, SIGNAL(mapped(QWidget *)), this, SLOT(openCon(QWidget *)));

            // Layout and UI calls for a new window to choose a RBC
            mainLayout = new QVBoxLayout;
            lineLayout = new QHBoxLayout;
            lineLayout->addWidget(txtLEU);
            lineLayout->addWidget(butLEU);
            mainLayout->addWidget(labLEU);
            mainLayout->addLayout(lineLayout);
            introLEU->setLayout(mainLayout);

            // Show RBC choosing Window
            introLEU->show();

        // If the option is to Check Version or Update executables, version info is needed, thus we read the versions file.
        } else if(optionNum == 3 || optionNum == 4){

            if(readVersionFile() == 0){
                connectWindow = new connectionsWindow(this);
                connect(this, SIGNAL (exitChild()), connectWindow ,SLOT (close()));
                connectWindow->show();
            } else {
                errors = "There was an error while reading your error file.\n";
                errorMes = errors.c_str();
                QMessageBox::critical(this, "Version file read error", errorMes);
            }


        // Any other option, we simply proceed to the connections Window
        } else {
            connectWindow = new connectionsWindow(this);
            connect(this, SIGNAL (exitChild()), connectWindow ,SLOT (close()));
            connectWindow->show();

        }
    }
    QMainWindow::statusBar()->showMessage(tr("On standby..."));
    configFile->clear();

}




