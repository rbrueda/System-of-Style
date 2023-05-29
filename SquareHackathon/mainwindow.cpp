#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "httprequests.h"
#include "managebooking.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include <string>
#include <QJsonValue>
#include <QtNetwork>
#include <fstream>
#include <QListWidget>
#include <QStringBuilder>
#include <QCalendarWidget>
#include <cmath>
#include <QMessageBox>
#include <QLineEdit>
#include <QFontDatabase>


using namespace std;

/*
TODO:
1. Fix Color Changing bug on the make account error message
2. Confirm that there are no empty fields in sign up
3. Make the elements of QWidget Employee View in a seperate class (Not Priority)

BUGS:
1. When we click login with typing name, Bisuit Rueda is shown
2. Error Message when sign in with invalid email

*/



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QFontDatabase::addApplicationFont(":/fonts/Rubeckia.ttf");
    ui->setupUi(this);
    // Start Contact List Hidden
    ui->staffProfile->hide();
    ui->addEmployeeWidget->hide();
    showStaffList();
    dropDownForCountryCode();

    // 0: Employee View
    // 1: Main Menu


//    mb.getAllSchedules("2023-08-01");



    timeAddEmployeeDropdown();
    initRowIndexesEmployeeBookingTable();

    if(!mb.initDB()){
        printErrorMessage("ERROR: INVALID DATABASE CREDENTIALS");
        on_settingsButton_clicked();
    }else{
        ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
    }


    // TEST CANCEL APPT:
    mb.cancelAppointment("ivu88944jf");

    // TEST CLIENT VIEW APPOINTMENT
    QString empID_client;
    double time_client;
    QDate date_client;

    mb.getBookingClient("65489jg54",&empID_client, &date_client, &time_client);

    cout << "TEST CLIENT VIEW:" << endl;
    cout << empID_client.toStdString() << endl;
    cout << time_client << endl;
    cout << date_client.toString("MMMM d, yyyy").toStdString() << endl;


//    ui->passwordDataBase->setEchoMode(QLineEdit.Password);


//    mb.getAvailableBookings("2023-08-01", "fj34f3443");



    // PHONE NUMBER ONLY NUMBERS:
//    ui->clientPhoneNm->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), ui->clientPhoneNm));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_submit_clicked()
{
    /*
    JSON FORMAT:

    "team_member": {
        "family_name": "",
        "email_address": "",
        "given_name": "",
        "phone_number": ""
    }
    */

    QJsonObject teamMember;
    QJsonObject parent;
    bool result = true;




    //note: this json file does not have parents

    teamMember["family_name"] = ui->lastName->text();
    teamMember["email_address"] = ui->email->text();
    teamMember["given_name"] = ui->firstName->text();

    teamMember["phone_number"] = ui->employee_countryCodeDropDown->currentText() + ui->phoneNumber->text();
    parent["team_member"] = teamMember;


    QString output = rq.addTeamMember(parent, &result);
    if(result == true){
        selectedProfileID = output;
        cout << output.toStdString() << endl;
    }
    else{
        printErrorMessage(output);
        return;
    }


//     ui->staffProfile->show();

//     Print JSON Contents (also converts to string)
//        QJsonDocument Doc(parent);
//        QByteArray ba = Doc.toJson();
//        QString q = QString(ba);
//        std::cout << q.toStdString() << std::endl;

    //elements from dropDown go to the SQL
    bool result_db = mb.addEmployeeSchedule(selectedProfileID, ui->monStartAddEmployee->itemData(ui->monStartAddEmployee->currentIndex()).toDouble(),
                           ui->monEndAddEmployee->itemData(ui->monEndAddEmployee->currentIndex()).toDouble(),
                           ui->tuesStartAddEmployee->itemData(ui->tuesStartAddEmployee->currentIndex()).toDouble(),
                           ui->tuesEndAddEmployee->itemData(ui->tuesEndAddEmployee->currentIndex()).toDouble(),
                           ui->wedStartAddEmployee->itemData(ui->wedStartAddEmployee->currentIndex()).toDouble(),
                           ui->wedEndAddEmployee->itemData(ui->wedEndAddEmployee->currentIndex()).toDouble(),
                           ui->thursStartAddEmployee->itemData(ui->thursStartAddEmployee->currentIndex()).toDouble(),
                           ui->thursEndAddEmployee->itemData(ui->thursEndAddEmployee->currentIndex()).toDouble(),
                           ui->friStartAddEmployee->itemData(ui->friStartAddEmployee->currentIndex()).toDouble(),
                           ui->friEndAddEmployee->itemData(ui->friEndAddEmployee->currentIndex()).toDouble(),
                           ui->satStartAddEmployee->itemData(ui->satStartAddEmployee->currentIndex()).toDouble(),
                           ui->satEndAddEmployee->itemData(ui->satEndAddEmployee->currentIndex()).toDouble(),
                           ui->sunStartAddEmployee->itemData(ui->sunStartAddEmployee->currentIndex()).toDouble(),
                           ui->sunEndAddEmployee->itemData(ui->sunEndAddEmployee->currentIndex()).toDouble());

    if(!result_db){
        printErrorMessage("Schedule could not be set.");
    }
}

void MainWindow::showStaffList()
{
    //complete http request
    QJsonObject teamMembersJson = rq.getTeamMembers();

    //gets the teamMember's array
    QJsonArray listOfMembers = teamMembersJson["team_members"].toArray();

    ui->staffList->clear();

    //make loop
    for (int i = 0; i < listOfMembers.size(); i++){
        //converts first item of array to object
        QJsonObject member1 = listOfMembers[i].toObject(); //gets the ith index of the array (ie gets each sector)

        //converts first member to string
        QString name = member1["given_name"].toString() + " " + member1["family_name"].toString();
        QString status = member1["status"].toString();
        QString email = member1["email_address"].toString();
        QString phoneNum = member1["phone_number"].toString();
        QString id = member1["id"].toString();

        if(status[0] == 'I'){
            continue;
        }

        //does the ui part to print to screen
        //item is the list in the employee manager
        QListWidgetItem* item = new QListWidgetItem();
//        item->setText(name);
        item->setData(1, email);
        item->setData(2, name);
        item->setData(3, phoneNum);
        item->setData(4, id);



        ui->staffList->addItem(item);
        ui->employee_dropdrown->addItem(name, id);
    }

    //    cout << "Type of team_members : " << typeid(teamMembersJson["team_members"]).name() << endl;

//    item->setText(QString(teamMembersJson["team_members"]));



//    QListWidgetItem* item = new QListWidgetItem();
//    item->setText(QString("Python"));

//    ui->staffList->addItem(item);

//    std::cout << "Item Added" << std::endl;
}


/*
    Data indexes:
    1: email address
    2: Full Name (displayed on QListWidget)
    3: Phone Number
    4: ID
 */

void MainWindow::on_staffList_itemDoubleClicked(QListWidgetItem *item){
    ui->staffProfile->show();
    ui->addEmployeeWidget->hide();

    ui->emailProfile->setText(item->data(1).toString());
    ui->nameProfile->setText(item->data(2).toString());
    ui->phoneNumberProfile->setText(item->data(3).toString());

    selectedProfileID = item->data(4).toString();

//    QList<double> test = {0, 2.5, 4, 6.5, 9, 11.5, 9, 11.5, 0, 2.5, 3, 8.5, 12, 18.5};
    QList<double> test = mb.getWorkSchedule(selectedProfileID);
    int hour;
    double minute;
    QList<QString> string = {"", "", "", "", "", "", "", "", "", "", "", "", "", ""};
    for (int i = 0; i<test.size(); i++){
        if (test[i] == -1){
            string[i] = QString::number(test[i]);
            continue;
        }
        hour = floor(test[i]);
        if (hour == 0){
            hour = 12;

        }
        else if (hour > 12){
            hour = hour - 12;
        }

        // Saying newString[i] is problematic -> use append instead
        string[i] = QString::number(hour) + ":"; // Use QStrings

         minute = test[i] - hour;
        if (fmod(test[i], 1) == 0){
            string[i] = string[i] + "00";
        }
        else{
            string[i] = string[i] + "30";
        }
        if (test[i] < 12){
            string[i] = string[i] + "AM";
        }
        else{
            string[i] = string[i] + "PM";
        }
        std::cout<<string[i].toStdString()<<endl;
    }
    //checks if any of the pairs are -1 (ie wont display any text in that QListEdit box)
    if (string[0] != "-1"){
        ui->monScheduleStaff->setText(string[0] % "-" % string[1]);
    }else{
        ui->monScheduleStaff->setText("");
    }
    if (string[2] != "-1"){
        ui->tuesScheduleStaff->setText(string[2] % "-" % string[3]);
    }else{
        ui->tuesScheduleStaff->setText("");
    }
    if (string[4] != "-1"){
        ui->wedScheduleStaff->setText(string[4] % "-" % string[5]);
    }else{
        ui->wedScheduleStaff->setText("");
    }
    if (string[6] != "-1"){
            ui->thursScheduleStaff->setText(string[6] % "-" % string[7]);
    }else{
            ui->thursScheduleStaff->setText("");
    }
    if (string[8] != "-1"){
        ui->friScheduleStaff->setText(string[8] % "-" % string[9]);
    }else{
        ui->friScheduleStaff->setText("");
    }
    if (string[10] != "-1"){
         ui->satScheduleStaff->setText(string[10] % "-" % string[11]);
    }else{
         ui->satScheduleStaff->setText("");
    }
    if (string[12] != "-1"){
        ui->sunScheduleStaff->setText(string[12] % "-" % string[13]);
    }else{
        ui->sunScheduleStaff->setText("");
    }

}




void MainWindow::on_deactivateButton_clicked()
{
    rq.inactivateTeamMember(selectedProfileID);
    showStaffList();
}


void MainWindow::on_addEmployeeButton_clicked()
{
    ui->staffProfile->hide();
    ui->addEmployeeWidget->show();
}


void MainWindow::on_employeeManagerButton_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->employeeManagerView);
}


void MainWindow::on_bookManagerButton_clicked()
{
//    ui->NestedSideBarStackWidget->setCurrentWidget(ui->DefaultWidget);

    ui->mainStackWidget->setCurrentWidget(ui->signinView);
    showClientList();
}

void MainWindow::showClientList()
{
    QJsonObject customerJson = rq.getCustomers();

    QJsonArray listOfClients = customerJson["customers"].toArray();

    ui->clientListWidget->clear();
    for (int i = 0; i < listOfClients.size(); i++){
        //converts first item of array to object
        QJsonObject member2 = listOfClients[i].toObject(); //gets the ith index of the array (ie gets each sector)

        //converts first member to string
        QString name = member2["given_name"].toString() + " " + member2["family_name"].toString();
//        QString status = member1["status"].toString(); //future use

        //instead:
        //QString unsubscription = member1["email_unsubscribed"].toString();
        QString email = member2["email_address"].toString();
        customerEmails.insert(email);

        QString phoneNum = member2["phone_number"].toString();
        QString id = member2["id"].toString();

//        if(unsubscription[0] == 'f'){
//            continue;
//        }

        //does the ui part to print to screen
        QListWidgetItem* item = new QListWidgetItem();
        //        item->setText(name);
        item->setData(1, email); //would the order of these change?
        item->setData(2, name);
        item->setData(3, phoneNum);
        item->setData(4, id);



        ui->clientListWidget->addItem(item);
    }
}


void MainWindow::on_SignpButton_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->signupView);
//    cout << "IT RUNS!!" << endl;
//    ui->signupWidget->show();
}


void MainWindow::on_makeAccountButton_clicked()
{
    //make it a json object and assign each text inputted to member in QJsonObject
    QJsonObject clientMember;
    bool result = true;
    //note: this json file does not have parents

    if(!customerEmails.contains(ui->clientEmail->text())){
        clientMember["family_name"] = ui->clientLastName->text();
        clientMember["email_address"] = ui->clientEmail->text();
        clientMember["given_name"] = ui->clientFirstName->text();
        clientMember["phone_number"] = ui->client_countryCodeDropDown->currentText() + ui->clientPhoneNm->text();
        QString output = rq.addClientMember(clientMember, &result);
        if(result == true){
//            ui->ErrorMessageWidget->hide();
            currentClientID = output;
            cout << output.toStdString() << endl;
            ui->mainStackWidget->setCurrentWidget(ui->manageClientView);
        }
        else{
            printErrorMessage(output);
        }

    }else{
        printErrorMessage("Account already exists. Please sign in.");
//        ui->txt_errorMessage->setText(QString("Account already exists. Please sign in."));
//        ui->ErrorMessageWidget->show();
//        ui->label->setText("<font color='red'>text</font>");
    }

}


void MainWindow::on_AddBookingsButton_clicked()
{
    if(currentClientID.isNull()){
        printErrorMessage("You must login/signup to continue");
        return;
    }
    ui->NestedSideBarStackWidget->setCurrentWidget(ui->addBookingWidget);

}


void MainWindow::dropDownForCountryCode(){
    ui->client_countryCodeDropDown->setEditable(true);
    ui->employee_countryCodeDropDown->setEditable(true);

    QFile file("../SquareHackathon/country-codes-phone.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line = in.readLine();
    line = in.readLine();
    while (!line.isNull()) {
        QStringList lineList = line.split(u',');

        ui->client_countryCodeDropDown->addItem(lineList[0] + " (+" + lineList[1] + ")", lineList[1]);
        ui->employee_countryCodeDropDown->addItem(lineList[0] + " (+" + lineList[1] + ")", lineList[1]);
        line = in.readLine();
    }
    ui->client_countryCodeDropDown->view()->setMinimumWidth(300);
    ui->employee_countryCodeDropDown->view()->setMinimumWidth(300);

}



void MainWindow::on_client_countryCodeDropDown_activated(int index)
{

    QLineEdit * LineEditStr = ui->client_countryCodeDropDown->lineEdit();

    LineEditStr->setText("+" + ui->client_countryCodeDropDown->itemData(index).toString());

}


void MainWindow::on_signInButton_clicked()
{
    if(ui->client_signIn_email->text() == ""){
        printErrorMessage("Please Type an email in");
        return;
    }

    QJsonObject customersJson;

    if(!rq.retrieveCustomer(ui->client_signIn_email->text(), &customersJson)){
        printErrorMessage("Invalid Email!");
        return;
    }

    ui->mainStackWidget->setCurrentWidget(ui->manageClientView);


    //gets the teamMember's array
    QJsonArray signInMember = customersJson["customers"].toArray();
    QJsonObject member1 = signInMember[0].toObject();

    QString name = member1["given_name"].toString() + " " + member1["family_name"].toString();
    QString status = member1["status"].toString();
    QString email = member1["email_address"].toString();
    QString phoneNum = member1["phone_number"].toString();
    QString id = member1["id"].toString();


    ui->clientPhoneNumber->setText(phoneNum);
    ui->clientEmailAddress->setText(email);
    ui->clientName->setText(name);

    currentClientID = id;
}


void MainWindow::on_submitDateButton_clicked()
{
//    QString date = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    QDate date = ui->calendarWidget->selectedDate();

    std::cout<<"Hello!\n";
    std::cout<< "date " << date.toString().toStdString()<<endl;
    QString selectionvalue= ui->employee_dropdrown->currentText();
    //id of employee
    QString idEmployee = ui->employee_dropdrown->itemData(2).toString();
    //    std::cout << "Choice Selected: " << selectionvalue.toStdString();
    std::cout << selectionvalue.toStdString()<<endl;
    std::cout <<"id: " << idEmployee.toStdString()<<endl;

    //id of customer
    std::cout <<"id: " <<currentClientID.toStdString()<<endl;

    updateAvaliableTimes_AddBooking(date, idEmployee);

}

void MainWindow::updateAvaliableTimes_AddBooking(QDate date, QString idEmployee)
{
//QList<double> test = {0.0, 0.5, 2.0, 12.0, 17.5, 20.0};
    QList<double> test = mb.getAvailableBookings(date, idEmployee);
    int hour;
    double minute;
    QString string;
    for (int i = 0; i<test.size(); i++){
        hour = floor(test[i]);
        if (hour == 0){
            hour = 12;

        }
        else if (hour > 12){
            hour = hour - 12;
        }

        // Saying newString[i] is problematic -> use append instead
        string = QString::number(hour) + ":"; // Use QStrings

        minute = test[i] - hour;
        if (fmod(test[i], 1) == 0){
            string = string + "00";
        }
        else{
            string = string + "30";
        }
        if (test[i] < 12){
            string = string + "AM";
        }
        else{
            string = string + "PM";
        }
        ui->timeAddBookingDropdown->addItem(string, test[i]);

    }
}

void MainWindow::printErrorMessage(QString message){
    QMessageBox messageBox;
    messageBox.critical(0,"Error", message);
    messageBox.setFixedSize(500,200);
}

//employee id
//client id
//date
//time

void MainWindow::on_submitAddBookingButton_clicked()
{
//    QJsonObject Booking;
    QString date = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    //id of employee
    QString idEmployee = ui->employee_dropdrown->itemData(2).toString();
//    QString time = ui->timeAddBookingDropdown->itemData(4).toString();
//    QString time = ui->timeAddBookingDropdown->currentText();
    double time = ui->timeAddBookingDropdown->itemData(ui->timeAddBookingDropdown->currentIndex()).toDouble();
//    cout<<"time: " << time.toStdString() <<endl; //time is an issue***

    mb.addBooking(date, time, currentClientID, idEmployee);

    //close the window for add bookings

    //id of customer
//    QString idClient = currentClientID.toString();

}


void MainWindow::timeAddEmployeeDropdown(){
    QList<QString> times = {"12:00AM", "12:30AM", "1:00AM", "1:30AM", "2:00AM", "2:30AM", "3:00AM", "3:30AM", "4:00AM", "4:30AM", "5:00AM", "5:30AM", "6:00AM", "6:30AM", "7:00AM", "7:30AM", "8:00AM", "8:30AM", "9:00AM", "9:30AM", "10:00AM", "10:30AM", "11:00AM", "11:30AM", "12:00PM", "12:30PM", "1:00PM", "1:30PM", "2:00PM", "2:30PM", "3:00PM", "3:30PM", "4:00PM", "4:30PM", "5:00PM", "5:30PM", "6:00PM", "6:30PM", "7:00PM", "7:30PM", "8:00PM", "8:30PM", "9:00PM", "9:30PM", "10:00PM", "10:30PM", "11:00PM", "11:30PM" };

    for (double i = 0; i < 48; i++){
        ui->monStartAddEmployee->addItem(times[i], i/2);
        ui->tuesStartAddEmployee->addItem(times[i], i/2);
        ui->wedStartAddEmployee->addItem(times[i], i/2);
        ui->thursStartAddEmployee->addItem(times[i], i/2);
        ui->friStartAddEmployee->addItem(times[i], i/2);
        ui->satStartAddEmployee->addItem(times[i], i/2);
        ui->sunStartAddEmployee->addItem(times[i], i/2);
        ui->monEndAddEmployee->addItem(times[i], i/2);
        ui->tuesEndAddEmployee->addItem(times[i], i/2);
        ui->wedEndAddEmployee->addItem(times[i], i/2);
        ui->thursEndAddEmployee->addItem(times[i], i/2);
        ui->friEndAddEmployee->addItem(times[i], i/2);
        ui->satEndAddEmployee->addItem(times[i], i/2);
        ui->sunEndAddEmployee->addItem(times[i], i/2);
    }
    ui->monStartAddEmployee->addItem("NULL", -1);
    ui->tuesStartAddEmployee->addItem("NULL", -1);
    ui->wedStartAddEmployee->addItem("NULL", -1);
    ui->thursStartAddEmployee->addItem("NULL", -1);
    ui->friStartAddEmployee->addItem("NULL", -1);
    ui->satStartAddEmployee->addItem("NULL", -1);
    ui->sunStartAddEmployee->addItem("NULL", -1);
    ui->monEndAddEmployee->addItem("NULL", -1);
    ui->tuesEndAddEmployee->addItem("NULL", -1);
    ui->wedEndAddEmployee->addItem("NULL", -1);
    ui->thursEndAddEmployee->addItem("NULL", -1);
    ui->friEndAddEmployee->addItem("NULL", -1);
    ui->satEndAddEmployee->addItem("NULL", -1);
    ui->sunEndAddEmployee->addItem("NULL", -1);
}



void MainWindow::on_refreshEmployeeBookings_clicked()
{
    ui->bookingsViewTable->clearContents();
    QString date = ui->viewBookings_selectDate->date().toString("yyyy-MM-dd");
    QList<QString> employeeList;
    QHash<QPair<QString,double>, QString> allAppointments = mb.getAllSchedules(date, &employeeList);
    ui->bookingsViewTable->setColumnCount(employeeList.size());

    for(int col=0; col<employeeList.size(); col++){
        QTableWidgetItem * headerItem = new QTableWidgetItem(rq.getTeamMemberInfo(employeeList[col]));
        ui->bookingsViewTable->setHorizontalHeaderItem(col, headerItem);
        for(double row=0; row<24;row+=0.5){
            QPair<QString,double> key;
            key.first = employeeList[col];
            key.second = row;
            if(allAppointments.contains(key)){
                QList<QString> itemValues = rq.getCustomerInfo(allAppointments[key]);
                QTableWidgetItem *item = new QTableWidgetItem(itemValues[0]);
                item->setData(3, itemValues[1]);
                ui->bookingsViewTable->setItem(row*2,col, item);
            }

        }
    }

}


void MainWindow::initRowIndexesEmployeeBookingTable(){
    QList<QString> times = {"12:00AM", "12:30AM", "1:00AM", "1:30AM", "2:00AM", "2:30AM", "3:00AM", "3:30AM", "4:00AM", "4:30AM", "5:00AM", "5:30AM", "6:00AM", "6:30AM", "7:00AM", "7:30AM", "8:00AM", "8:30AM", "9:00AM", "9:30AM", "10:00AM", "10:30AM", "11:00AM", "11:30AM", "12:00PM", "12:30PM", "1:00PM", "1:30PM", "2:00PM", "2:30PM", "3:00PM", "3:30PM", "4:00PM", "4:30PM", "5:00PM", "5:30PM", "6:00PM", "6:30PM", "7:00PM", "7:30PM", "8:00PM", "8:30PM", "9:00PM", "9:30PM", "10:00PM", "10:30PM", "11:00PM", "11:30PM" };

    ui->bookingsViewTable->setRowCount(times.size());
    for(int i=0; i<times.size();i++){
        QTableWidgetItem * rowIndex = new QTableWidgetItem(times[i]);
        ui->bookingsViewTable->setVerticalHeaderItem(i, rowIndex);
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}

void MainWindow::on_backButtonEmployeeManager_clicked()
{
    //set current widget to the main menu
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}

void MainWindow::on_bookingsViewTable_itemDoubleClicked(QTableWidgetItem *item)
{
    QString emailPhone = item->data(3).toString();

    QMessageBox customerInfoPopup;

    customerInfoPopup.setText("Contact Information");
    customerInfoPopup.setInformativeText(item->text() % "\n" % emailPhone);

    customerInfoPopup.exec();
}



void MainWindow::on_settingsButton_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->settingsView);
    ifstream MyFile("../SquareHackathon/apiCode.txt");
    string lineRead;
    getline(MyFile,lineRead);
    ui->squareApiCode->setText(QString::fromStdString (lineRead));
    MyFile.close();
    ifstream MyFile2("../SquareHackathon/bookingDBLogin.txt");
    getline(MyFile2, lineRead);
    ui->urlDataBase->setText(QString::fromStdString (lineRead));
    getline(MyFile2, lineRead);
    ui->usernameDataBase->setText(QString::fromStdString (lineRead));
    getline(MyFile2, lineRead);
    ui->passwordDataBase->setText(QString::fromStdString (lineRead));
    MyFile2.close();

}


void MainWindow::on_submitSquareApiCodeButton_clicked()
{
    ofstream MyFile("../SquareHackathon/apiCode.txt");
    MyFile<<ui->squareApiCode->text().toStdString()<<endl;
    MyFile.close();

    rq.setAPICode();
}


void MainWindow::on_submitDataBaseLoginButton_clicked()
{
    ofstream MyFile("../SquareHackathon/bookingDBLogin.txt");
    MyFile<<ui->urlDataBase->text().toStdString()<<endl;
    MyFile<<ui->usernameDataBase->text().toStdString()<<endl;
    MyFile<<ui->passwordDataBase->text().toStdString()<<endl;

    if(!mb.initDB()){
        printErrorMessage("ERROR: INVALID DATABASE CREDENTIALS");
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}


void MainWindow::on_employee_countryCodeDropDown_activated(int index)
{
    QLineEdit * LineEditStr = ui->employee_countryCodeDropDown->lineEdit();

    LineEditStr->setText("+" + ui->employee_countryCodeDropDown->itemData(index).toString());
}




void MainWindow::on_backButton_viewBookings_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}


void MainWindow::on_viewAllBookingsButton_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->viewAllBookingsView);
    ui->viewBookings_selectDate->setDate(QDate::currentDate());
}

