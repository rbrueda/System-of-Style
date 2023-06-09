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
#include <thread>
#include <QtConcurrent/QtConcurrent>

using namespace std;

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

    ui->clientName->setAlignment(Qt::AlignHCenter);
    ui->nameProfile->setAlignment(Qt::AlignHCenter);

    timeAddEmployeeDropdown();
    initRowIndexesEmployeeBookingTable();

    //error messages displayed if there is an incorrect database code
    if(!mb.initDB()){
        printErrorMessage("ERROR: INVALID DATABASE CREDENTIALS");
        on_settingsButton_clicked();
    }else{
        ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
    }


    ui->bookingsViewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // PHONE NUMBER ONLY NUMBERS:
    ui->clientPhoneNm->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), ui->clientPhoneNm));
    ui->phoneNumber->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), ui->phoneNumber));

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

    //assign teamMember objects to text displayed in QLineEdit
    teamMember["family_name"] = ui->lastName->text();
    teamMember["email_address"] = ui->email->text();
    teamMember["given_name"] = ui->firstName->text();

    teamMember["phone_number"] = ui->employee_countryCodeDropDown->currentText() + ui->phoneNumber->text();
    parent["team_member"] = teamMember;



    //call the request for adding a team member
    QString output = rq.addTeamMember(parent, &result);
    if(result == true){
        selectedProfileID = output;
    }
    else{
        printErrorMessage(output);
        return;
    }

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

    // Checks if schedule can be created --SQL returns an error
    if(!result_db){
        printErrorMessage("Schedule could not be set.");
        rq.inactivateTeamMember(selectedProfileID);
        return;
    }

    //clear the old contents
    ui->firstName->clear();
    ui->lastName->clear();
    ui->phoneNumber->clear();
    ui->email->clear();


    showStaffList();
}

void MainWindow::showStaffList()
{
    //complete http request
    QJsonObject teamMembersJson = rq.getTeamMembers();

    //gets the teamMember's array
    QJsonArray listOfMembers = teamMembersJson["team_members"].toArray();

    ui->staffList->clear();
    ui->employee_dropdrown->clear();

    //make loop
    for (int i = 0; i < listOfMembers.size(); i++){
        //converts first item of array to object
        QJsonObject member1 = listOfMembers[i].toObject(); //gets the ith index of the array (ie gets each sector)


        //converts first member to string
        QString name = member1["given_name"].toString() + " " + member1["family_name"].toString();
        if(!QString::compare(name, QString("Sandbox Seller"))){
            continue;
        }

        QString status = member1["status"].toString();
        QString email = member1["email_address"].toString();
        QString phoneNum = member1["phone_number"].toString();
        QString id = member1["id"].toString();

        //check if teamMember is inactive
        if(status[0] == 'I'){
            continue;
        }

        //does the ui part to print to screen
        //item is the list in the employee manager
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(1, email);
        item->setData(2, name);
        item->setData(3, phoneNum);
        item->setData(4, id);



        ui->staffList->addItem(item);
        ui->employee_dropdrown->addItem(name, id);
    }
}

/*
    Data indexes:
    1: email address
    2: Full Name (displayed on QListWidget)
    3: Phone Number
    4: ID
 */

void MainWindow::on_staffList_itemDoubleClicked(QListWidgetItem *item){
    //show correspond statck widget
    ui->staffProfile->show();
    ui->addEmployeeWidget->hide();

    //sets corresponding text for qLineEdit
    ui->emailProfile->setText(item->data(1).toString());
    ui->nameProfile->setText(item->data(2).toString());
    ui->phoneNumberProfile->setText(item->data(3).toString());

    //rerieves the id of teamMember
    selectedProfileID = item->data(4).toString();

    //calls the request to get the employee schedule
    QList<double> test = mb.getWorkSchedule(selectedProfileID);
    QList<QString> string = {"", "", "", "", "", "", "", "", "", "", "", "", "", ""};
    //loops the through each element in the array and converts to standard time (__:__AM/PM)
    for (int i = 0; i<test.size(); i++){
        if (test[i] == -1){
            string[i] = QString::number(test[i]);
            continue;
        }
        string[i] = convertTime(test[i]);
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
    //calls the request to deactivate the team member to the API
    rq.inactivateTeamMember(selectedProfileID);
    //calls the managebooking class remove the schedule ionformation to the database
    mb.removeSchedule(selectedProfileID);
    //update staff schedule
    showStaffList();
    //hide the specific team member's profile (since it has already been deactivated)
    ui->staffProfile->hide();
}


void MainWindow::on_addEmployeeButton_clicked()
{
    //hide the current staff profile
    ui->staffProfile->hide();
    ui->addEmployeeWidget->show();
}


void MainWindow::on_employeeManagerButton_clicked()
{
    //hide and show wanted widgets
    showStaffList();
    ui->mainStackWidget->setCurrentWidget(ui->employeeManagerView);
    ui->staffProfile->hide();
}


void MainWindow::on_bookManagerButton_clicked()
{

    ui->mainStackWidget->setCurrentWidget(ui->signinView);
    showClientList();
}

void MainWindow::showClientList()
{
    //call the request to get all the customers in the API
    QJsonObject customerJson = rq.getCustomers();

    //convert the JSON content to a QArray
    QJsonArray listOfClients = customerJson["customers"].toArray();

    ui->clientListWidget->clear();
    for (int i = 0; i < listOfClients.size(); i++){
        //converts first item of array to object
        QJsonObject member2 = listOfClients[i].toObject(); //gets the ith index of the array (ie gets each sector)

        //converts first member to string
        QString name = member2["given_name"].toString() + " " + member2["family_name"].toString();
        QString email = member2["email_address"].toString();
        customerEmails.insert(email);
        QString phoneNum = member2["phone_number"].toString();
        QString id = member2["id"].toString();

        //does the ui part to print to screen
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(1, email);
        item->setData(2, name);
        item->setData(3, phoneNum);
        item->setData(4, id);

        ui->clientListWidget->addItem(item);
    }
}


void MainWindow::on_SignpButton_clicked()
{
    //show the signUpView widget
    ui->mainStackWidget->setCurrentWidget(ui->signupView);
}


void MainWindow::on_makeAccountButton_clicked()
{
    //make it a json object and assign each text inputted to member in QJsonObject
    QJsonObject clientMember;
    bool result = true;
    //note: this json file does not have parents

    //check if account is already made (ie. check if the email is in the database)
    if(!customerEmails.contains(ui->clientEmail->text())){
        //assign the object member contents from text in QLineEdit
        clientMember["family_name"] = ui->clientLastName->text();
        clientMember["email_address"] = ui->clientEmail->text();
        clientMember["given_name"] = ui->clientFirstName->text();
        clientMember["phone_number"] = ui->client_countryCodeDropDown->currentText() + ui->clientPhoneNm->text();
        QString output = rq.addClientMember(clientMember, &result);

        //if there are errors
        if(result == true){
            currentClientID = output;

            ui->clientEmail->clear();
            ui->clientLastName->clear();
            ui->clientFirstName->clear();
            ui->clientPhoneNm->clear();

            //shows the clinet profile info on the screen
            ui->clientPhoneNumber->setText(clientMember["phone_number"].toString());
            ui->clientEmailAddress->setText(clientMember["email_address"].toString());
            ui->clientName->setText(clientMember["family_name"].toString() % " " % clientMember["given_name"].toString());
            ui->mainStackWidget->setCurrentWidget(ui->manageClientView);
            displayForManageBookingView();


        }
        else{
            printErrorMessage(output);
        }

    }else{
        printErrorMessage("Account already exists. Please sign in.");
    }
}


void MainWindow::on_AddBookingsButton_clicked()
{
    //check if the client has not sign up yet
    if(currentClientID.isNull()){
        printErrorMessage("You must login/signup to continue");
        return;
    }
    //if they have signed up: set the current widget to addBooking widget
    ui->NestedSideBarStackWidget->setCurrentWidget(ui->addBookingWidget);

}

//**
void MainWindow::dropDownForCountryCode(){
    ui->client_countryCodeDropDown->setEditable(true);
    ui->employee_countryCodeDropDown->setEditable(true);

    // Reads all avaliable country codes
    QFile file("country-codes-phone.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line = in.readLine();
    line = in.readLine();

    // For each country code avaliable -> add the code and the country name to the drop down
    while (!line.isNull()) {
        QStringList lineList = line.split(u',');

        ui->client_countryCodeDropDown->addItem(lineList[0] + " (+" + lineList[1] + ")", lineList[1]);
        ui->employee_countryCodeDropDown->addItem(lineList[0] + " (+" + lineList[1] + ")", lineList[1]);
        line = in.readLine();
    }
    ui->client_countryCodeDropDown->view()->setMinimumWidth(300);
    ui->employee_countryCodeDropDown->view()->setMinimumWidth(300);

}




// Updates Text on Country Code Dropdown based on what is selected
void MainWindow::on_client_countryCodeDropDown_activated(int index)
{

    QLineEdit * LineEditStr = ui->client_countryCodeDropDown->lineEdit();

    LineEditStr->setText("+" + ui->client_countryCodeDropDown->itemData(index).toString());

}


void MainWindow::on_signInButton_clicked()
{
    //check if user did not sign in
    if(ui->client_signIn_email->text() == ""){
        printErrorMessage("Please Type an email in");
        return;
    }

    QJsonObject customersJson;

    //if the email does not follow proper formatting
    if(!rq.retrieveCustomer(ui->client_signIn_email->text(), &customersJson)){
        printErrorMessage("Invalid Email!");
        return;
    }

    ui->client_signIn_email->clear();
    ui->mainStackWidget->setCurrentWidget(ui->manageClientView);


    //gets the teamMember's array
    QJsonArray signInMember = customersJson["customers"].toArray();
    QJsonObject member1 = signInMember[0].toObject();

    QString name = member1["given_name"].toString() + " " + member1["family_name"].toString();
    QString status = member1["status"].toString();
    QString email = member1["email_address"].toString();
    QString phoneNum = member1["phone_number"].toString();
    QString id = member1["id"].toString();


    //display the contents from json object to the QLineEdit
    ui->clientPhoneNumber->setText(phoneNum);
    ui->clientEmailAddress->setText(email);
    ui->clientName->setText(name);

    currentClientID = id;

    displayForManageBookingView();


}

// Check whether there is a booking and show the cancel booking view:
void MainWindow::displayForManageBookingView(){
    QString employeeID;
    double time;
    QDate date;

    // Get the appointment information and puts the information on the UI
    if(mb.getBookingClient(currentClientID,&employeeID, &date, &time)){
        ui->clientViewSideStack->setCurrentWidget(ui->cancelBookingView);

        ui->client_apptEmployee->setText(rq.getTeamMemberInfo(employeeID));
        ui->client_apptDate->setText(date.toString("MMMM d, yyyy"));
        ui->client_apptTime->setText(convertTime(time));

        return;
    }
    ui->clientViewSideStack->setCurrentWidget(ui->addBookingView);
}


QString MainWindow::convertTime(double time){
    QString result;
    //converts decimal value to __:__AM/PM time format
    int hour = floor(time);
    if (hour == 0){
        hour = 12;

    }
    else if (hour > 12){
        hour = hour - 12;
    }

    // Saying newString[i] is problematic -> use append instead
    result = QString::number(hour) + ":"; // Use QStrings

    if (fmod(time, 1) == 0){
        result = result + "00";
    }
    else{
        result = result + "30";
    }
    if (time < 12){
        result = result + "AM";
    }
    else{
        result = result + "PM";
    }
    return result;
}

void MainWindow::on_submitDateButton_clicked()
{
    //set the date to the selected date on the calendar
    QDate date = ui->calendarWidget->selectedDate();

    //retrieve the same of the employee
    QString selectionvalue= ui->employee_dropdrown->currentText();
    //id of employee
    QString idEmployee = ui->employee_dropdrown->itemData(ui->employee_dropdrown->currentIndex()).toString();

    //calls function to update the times in the time dropdown
    updateAvaliableTimes_AddBooking(date, idEmployee);

}

void MainWindow::updateAvaliableTimes_AddBooking(QDate date, QString idEmployee)
{
    // Remove previous times
    ui->timeAddBookingDropdown->clear();
    // Get List of Avaliable times
    QList<double> test = mb.getAvailableBookings(date, idEmployee);

    // Add times to the dropdown
    for (int i = 0; i<test.size(); i++){
        QString string = convertTime(test[i]);
        ui->timeAddBookingDropdown->addItem(string, test[i]);

    }
}

// Call this function when you want a popup error message
void MainWindow::printErrorMessage(QString message){
    QMessageBox messageBox;
    messageBox.critical(0,"Error", message);
    messageBox.setFixedSize(500,200);
}



void MainWindow::on_submitAddBookingButton_clicked()
{
    // Get the Date:
    QString date = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    //Get the ID:
    QString idEmployee = ui->employee_dropdrown->itemData(ui->employee_dropdrown->currentIndex()).toString();

    // Get the time:
    double time = ui->timeAddBookingDropdown->itemData(ui->timeAddBookingDropdown->currentIndex()).toDouble();

    // Add Booking with information
    mb.addBooking(date, time, currentClientID, idEmployee);

    // Change view to view booking
    displayForManageBookingView();

}


// Make Dropdown to select employee schedule -> occurs when employee is made
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
    // Clear the table:
    ui->bookingsViewTable->clearContents();

    // Get Date from date select
    QString date = ui->viewBookings_selectDate->date().toString("yyyy-MM-dd");
    QList<QString> employeeList;

    // Get QHash of the employees booked for the given day
    QHash<QPair<QString,double>, QString> allAppointments = mb.getAllSchedules(date, &employeeList);
    ui->bookingsViewTable->setColumnCount(employeeList.size());

    QList<QFuture<void>> tasks; // List of tasks to run in parallel

    // For each employee...
    for(int col=0; col<employeeList.size(); col++){
        // Employee Header Item:
        QTableWidgetItem * headerItem = new QTableWidgetItem(rq.getTeamMemberInfo(employeeList[col]));
        ui->bookingsViewTable->setHorizontalHeaderItem(col, headerItem);

        // For Each avaliable time -> check if there is a customer in that time
        for(double row=0; row<24;row+=0.5){
            QPair<QString,double> key;
            key.first = employeeList[col];
            key.second = row;
            // Make TableWidgetItem for that customer...
            if(allAppointments.contains(key)){
                tasks.append(QtConcurrent::run(std::mem_fn(&MainWindow::updateTableCustomer), this, QString(allAppointments[key]), row, col));
            }

        }
    }
    for(int i=0;i< tasks.size(); i++){
        tasks[i].waitForFinished();
    }

}

// Makes Table Item for a customer -> function is ran in parallel
void MainWindow::updateTableCustomer(QString customerID, double row, int col){
    QList<QString> itemValues = rq.getCustomerInfo(customerID);
    QTableWidgetItem *item = new QTableWidgetItem(itemValues[0]);
    item->setData(3, itemValues[1]);
    ui->bookingsViewTable->setItem(row*2,col, item);
}



// In the view BOoking Table, set the times on the left most column
// Also sets 24 rows
void MainWindow::initRowIndexesEmployeeBookingTable(){
    QList<QString> times = {"12:00AM", "12:30AM", "1:00AM", "1:30AM", "2:00AM", "2:30AM", "3:00AM", "3:30AM", "4:00AM", "4:30AM", "5:00AM", "5:30AM", "6:00AM", "6:30AM", "7:00AM", "7:30AM", "8:00AM", "8:30AM", "9:00AM", "9:30AM", "10:00AM", "10:30AM", "11:00AM", "11:30AM", "12:00PM", "12:30PM", "1:00PM", "1:30PM", "2:00PM", "2:30PM", "3:00PM", "3:30PM", "4:00PM", "4:30PM", "5:00PM", "5:30PM", "6:00PM", "6:30PM", "7:00PM", "7:30PM", "8:00PM", "8:30PM", "9:00PM", "9:30PM", "10:00PM", "10:30PM", "11:00PM", "11:30PM" };

    ui->bookingsViewTable->setRowCount(times.size());
    for(int i=0; i<times.size();i++){
        QTableWidgetItem * rowIndex = new QTableWidgetItem(times[i]);
        ui->bookingsViewTable->setVerticalHeaderItem(i, rowIndex);
    }
}


// Goes to main menu
void MainWindow::on_pushButton_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}


// Employee Manager -> press back button
void MainWindow::on_backButtonEmployeeManager_clicked()
{
    //set current widget to the main menu
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}


// Displays customer information from view booking when name is double clicked
void MainWindow::on_bookingsViewTable_itemDoubleClicked(QTableWidgetItem *item)
{
    QString emailPhone = item->data(3).toString();


    // Make Popup:
    QMessageBox customerInfoPopup;

    customerInfoPopup.setText("Contact Information");
    customerInfoPopup.setInformativeText(item->text() % "\n" % emailPhone);

    QFont font;
    font.setPointSize(12);
    customerInfoPopup.setFont(font);
    customerInfoPopup.exec();
}



void MainWindow::on_settingsButton_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->settingsView);

    // Puts credentials from square API on the settings page
    ifstream MyFile("apiCode.txt");
    string lineRead;
    getline(MyFile,lineRead);
    ui->squareApiCode->setText(QString::fromStdString (lineRead));
    MyFile.close();

    // Puts credentials from database on the settings page
    ifstream MyFile2("bookingDBLogin.txt");
    getline(MyFile2, lineRead);
    ui->urlDataBase->setText(QString::fromStdString (lineRead));
    getline(MyFile2, lineRead);
    ui->usernameDataBase->setText(QString::fromStdString (lineRead));
    getline(MyFile2, lineRead);
    ui->passwordDataBase->setText(QString::fromStdString (lineRead));
    MyFile2.close();

}


// Updates Square API credentials from settings
void MainWindow::on_submitSquareApiCodeButton_clicked()
{
    ofstream MyFile("apiCode.txt");
    MyFile<<ui->squareApiCode->text().toStdString()<<endl;
    MyFile.close();

    rq.setAPICode();
}


// Updates SQL Credentials from settings
void MainWindow::on_submitDataBaseLoginButton_clicked()
{
    ofstream MyFile("bookingDBLogin.txt");
    MyFile<<ui->urlDataBase->text().toStdString()<<endl;
    MyFile<<ui->usernameDataBase->text().toStdString()<<endl;
    MyFile<<ui->passwordDataBase->text().toStdString()<<endl;

    if(!mb.initDB()){
        printErrorMessage("ERROR: INVALID DATABASE CREDENTIALS");
    }
}


// Goes to main menu from employee view
void MainWindow::on_pushButton_2_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}


// Edits the field when country code is selected
void MainWindow::on_employee_countryCodeDropDown_activated(int index)
{
    QLineEdit * LineEditStr = ui->employee_countryCodeDropDown->lineEdit();

    LineEditStr->setText("+" + ui->employee_countryCodeDropDown->itemData(index).toString());
}



// View Bookings -> goes to main menu
void MainWindow::on_backButton_viewBookings_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}

// Main Menu go to view bookings view
void MainWindow::on_viewAllBookingsButton_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->viewAllBookingsView);
    ui->viewBookings_selectDate->setDate(QDate::currentDate());
}


//View Appointment cancel -> goes to add booking + deletes appt
void MainWindow::on_cancel_button_viewAppt_clicked()
{
    mb.cancelAppointment(currentClientID);
    ui->clientViewSideStack->setCurrentWidget(ui->addBookingView);
}

// Back Button on sign in view -> main menu
void MainWindow::on_backButton_signInView_clicked(){
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}

// BackButton on SignUp View -> Sign in
void MainWindow::on_backButton_signupView_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->signinView);
}

// ClientView Back Button -> sign in
void MainWindow::on_backButton_clientView_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->signinView);
}


// ClientView MainMenu Button -> main menu
void MainWindow::on_mainMenuButton_clientView_clicked()
{
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);
}

