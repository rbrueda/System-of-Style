#include "httprequests.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include <string>
#include <QJsonValue>
#include <QtNetwork>

#include <fstream>
#include <filesystem>


using namespace std;


HttpRequests::HttpRequests()
{
    apiCode = getAPICode(); // Stores API Code
}

void HttpRequests::addTeamMember(QJsonObject json){

    // Creates url object:
    QUrl url("https://connect.squareupsandbox.com/v2/team-members");
    QNetworkRequest request(url);

    // Set Request Header:
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", apiCode);
    request.setRawHeader("Square-Version", "2023-04-19");

    // Make POST Request:
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    // Wait for Response from Server:
    while (!reply->isFinished()) qApp->processEvents();

    // Print out results in terminal:
    QByteArray response_data = reply->readAll();
    QJsonDocument json2 = QJsonDocument::fromJson(response_data);
    QByteArray ba = json2.toJson();
    QString q = QString(ba);
    std::cout << q.toStdString() << std::endl;

    reply->deleteLater();
}


QByteArray HttpRequests::getAPICode(){
    string api;
    ifstream fin;

    // Check Current Directory:
    // std::cout << filesystem::current_path().string() << std::endl;

    // File Path Might not work on final export:

    // Open file:
    fin.open("../SquareHackathon/apiCode.txt");

    // Reads line contents from fin to String api
    getline (fin, api);

    // Print api code:
//     std::cout << "API: " << std::endl;
//     std::cout << "|"  + api +  "|" << std::endl;

    // Convert String -> QString -> QByteArray
    QString qstr = QString::fromStdString("Bearer " + api);
    return qstr.toUtf8();
}
