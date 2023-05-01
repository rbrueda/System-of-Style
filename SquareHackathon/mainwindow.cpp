#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "squareapirequests.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include <string>
#include <QJsonValue>
#include <QtNetwork>


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

    addTeamMember(parent);

//    QJsonDocument Doc(parent);
//    QByteArray ba = Doc.toJson();
//    QString q = QString(ba);
//    std::cout << q.toStdString() << std::endl;
}

void MainWindow::addTeamMember(QJsonObject json){
    QUrl url("https://connect.squareupsandbox.com/v2/team-members");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader("Authorization", "Bearer EAAAEDllvmPs57fW_V-T10ifv_xJ2uyXQrLYxXYZiz7S23suCNiMCLwWKH8JV-TR");
    request.setHeader("Square-Version", "2023-04-19");

    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    // Wait for Response from Server:

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    reply->deleteLater();

}
