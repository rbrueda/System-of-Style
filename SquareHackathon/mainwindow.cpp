#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include <string>
#include <QJsonValue>

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
//    string firstName, lastName, email, phoneNumber;
    QJsonObject teamMember;
    QJsonObject parent;

    teamMember["family_name"] = ui->lastName->text();
    teamMember["email_address"] = ui->email->text();
    teamMember["given_name"] = ui->firstName->text();
    teamMember["phone_number"] = ui->phoneNumber->text();

    parent["team_member"] = teamMember;

    QJsonDocument Doc(parent);
    QByteArray ba = Doc.toJson();
    QString q = QString(ba);
    std::cout << q.toStdString() << std::endl;
}

