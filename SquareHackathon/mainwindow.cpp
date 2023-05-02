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

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
//     Print JSON Contents (also converts to string)
//        QJsonDocument Doc(parent);
//        QByteArray ba = Doc.toJson();
//        QString q = QString(ba);
//        std::cout << q.toStdString() << std::endl;
}

void MainWindow::showStaffList(){
    QListWidgetItem* item = new QListWidgetItem();

    QJsonObject teamMembersJson = rq.getTeamMembers();

    QJsonArray listOfMembers = teamMembersJson["team_members"].toArray();

    QJsonObject member1 = listOfMembers[0].toObject();

    QString name = member1["given_name"].toString();

    item->setText(name);

    ui->staffList->addItem(item);

    std::cout << "Item Added" << std::endl;

    //    cout << "Type of team_members : " << typeid(teamMembersJson["team_members"]).name() << endl;

//    item->setText(QString(teamMembersJson["team_members"]));



//    QListWidgetItem* item = new QListWidgetItem();
//    item->setText(QString("Python"));

//    ui->staffList->addItem(item);

//    std::cout << "Item Added" << std::endl;
}

