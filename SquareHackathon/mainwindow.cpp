#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "httprequests.h"
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


using namespace std;

/*
TODO:
1. Fix Color Changing bug on the make account error message
2. Confirm that there are no empty fields in sign up
3. Make the elements of QWidget Employee View in a seperate class (Not Priority)
    */



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Start Contact List Hidden
    ui->staffProfile->hide();
    ui->addEmployeeWidget->hide();
    showStaffList();
    dropDownForCountryCode();

    // 0: Employee View
    // 1: Main Menu
    ui->mainStackWidget->setCurrentWidget(ui->mainMenuView);

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

    // jsonVarName[key] = value;
    teamMember["family_name"] = ui->lastName->text();
    teamMember["email_address"] = ui->email->text();
    teamMember["given_name"] = ui->firstName->text();
    teamMember["phone_number"] = ui->phoneNumber->text();
    parent["team_member"] = teamMember;

    rq.addTeamMember(parent);
    showStaffList();
//     ui->staffProfile->show();

//     Print JSON Contents (also converts to string)
//        QJsonDocument Doc(parent);
//        QByteArray ba = Doc.toJson();
//        QString q = QString(ba);
//        std::cout << q.toStdString() << std::endl;
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
    ui->NestedSideBarStackWidget->setCurrentWidget(ui->DefaultWidget);
    ui->ErrorMessageWidget->hide();
//    ui->signupWidget->hide();
    ui->mainStackWidget->setCurrentWidget(ui->bookManagerView);
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
    ui->NestedSideBarStackWidget->setCurrentWidget(ui->signupWidget);
//    cout << "IT RUNS!!" << endl;
//    ui->signupWidget->show();
}


void MainWindow::on_makeAccountButton_clicked()
{
    //make it a json object and assign each text inputted to member in QJsonObject
    QJsonObject clientMember;
    QString id = "something";
    //note: this json file does not have parents

    if(!customerEmails.contains(ui->clientEmail->text())){
        clientMember["family_name"] = ui->clientLastName->text();
        clientMember["email_address"] = ui->clientEmail->text();
        clientMember["given_name"] = ui->clientFirstName->text();
        clientMember["phone_number"] = ui->client_countryCodeDropDown->currentText() + ui->clientPhoneNm->text();
        QString output = rq.addClientMember(clientMember);
        if(output.isNull()){
            ui->ErrorMessageWidget->hide();
        }
        else{
            ui->txt_errorMessage->setText(output);
            ui->ErrorMessageWidget->show();
            ui->txt_errorMessage->setText("<font color='red'>" + output + "</font>");
            //split output line to 2 string variables
        }

    }else{
//        ui->txt_errorMessage->setText(QString("Account already exists. Please sign in."));
        ui->ErrorMessageWidget->show();
//        ui->label->setText("<font color='red'>text</font>");
    }
    currentClientID = id;
}


void MainWindow::on_AddBookingsButton_clicked()
{
    ui->NestedSideBarStackWidget->setCurrentWidget(ui->addBookingWidget);
}


void MainWindow::dropDownForCountryCode(){
    ui->client_countryCodeDropDown->setEditable(true);
    QFile file("../SquareHackathon/country-codes-phone.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line = in.readLine();
    line = in.readLine();
    while (!line.isNull()) {
        QStringList lineList = line.split(u',');

        ui->client_countryCodeDropDown->addItem(lineList[0] + " (+" + lineList[1] + ")", lineList[1]);
        line = in.readLine();
    }
    ui->client_countryCodeDropDown->view()->setMinimumWidth(300);

}


void MainWindow::on_ViewAllBookings_clicked()
{
    ui->NestedSideBarStackWidget->setCurrentWidget(ui->ViewAllBookingsWidget);
}

void MainWindow::on_client_countryCodeDropDown_activated(int index)
{

    QLineEdit * LineEditStr = ui->client_countryCodeDropDown->lineEdit();

    LineEditStr->setText("+" + ui->client_countryCodeDropDown->itemData(index).toString());

}


void MainWindow::on_signInButton_clicked()
{
    ui->NestedSideBarStackWidget->setCurrentWidget(ui->signInProfileWidget);
    QJsonObject customersJson = rq.retrieveCustomer(ui->client_signIn_email->text());

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
    QString date = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    std::cout<<"Hello!\n";
    std::cout<< "date " << date.toStdString()<<endl;
    QString selectionvalue= ui->employee_dropdrown->currentText();
    //id of employee
    QString idEmployee = ui->employee_dropdrown->itemData(2).toString();
    //    std::cout << "Choice Selected: " << selectionvalue.toStdString();
    std::cout << selectionvalue.toStdString()<<endl;
    std::cout <<"id: " << idEmployee.toStdString()<<endl;

    //id of customer
    std::cout <<"id: " <<currentClientID.toStdString()<<endl;
}

QList<double> test = {0.0, 0.5, 2.0, 12.0, 17.5, 20.0};
int hour;
double minute;
QList<string> newString;
for (int i = 0; i<test.size(); i++){
    hour = floor(test[i]);
    if (hour == 0){
        hour = 12;
    }
    else if (hour > 12){
        hour = hour - 12;
    }
    newString[i] = string(hour) + ":";

    minute = test[i] - hour;
    if (minute == 0.5){
        newString[i] = newString[i] + "30";
    }
    else if (minute == 0.0){
        newString[i] = newString[i] + "00";
    }
}



