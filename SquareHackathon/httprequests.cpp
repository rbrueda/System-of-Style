#include "httprequests.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include <string>
#include <QJsonValue>
#include <QtNetwork>

#include <fstream>
#include <filesystem>
#include <QStringBuilder>


using namespace std;


HttpRequests::HttpRequests()
{
    setAPICode(); // Stores API Code
}


QString HttpRequests::addTeamMember(QJsonObject json, bool * result){

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
    QJsonObject output = json2.object();
    QString returnStr;

    if(output.contains("errors")){
        QJsonArray listOfErrors = output["errors"].toArray();

        for(int i=0; i<listOfErrors.size();i++){
            QJsonObject temp = listOfErrors[i].toObject();
            reply->deleteLater();
            returnStr = returnStr % temp["detail"].toString() % "\n";
        }
        *result = false;
        reply->deleteLater();
        return returnStr;
    }


    QString id = output["team_member"].toObject()["id"].toString();

    *result = true;

    reply->deleteLater();
    return id;
}




QJsonObject HttpRequests::getTeamMembers(){

    // Creates url object:
    QUrl url("https://connect.squareupsandbox.com/v2/team-members/search");
    QNetworkRequest request(url);

    // Set Request Header:
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", apiCode);
    request.setRawHeader("Square-Version", "2023-04-19");

    // Make POST Request:
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.post(request, QByteArray());

    // Wait for Response from Server:
    while (!reply->isFinished()) qApp->processEvents();

    // Print out results in terminal:
    QByteArray response_data = reply->readAll();
    QJsonDocument json2 = QJsonDocument::fromJson(response_data);

    reply->deleteLater();
    return json2.object();
}


QJsonObject HttpRequests::getCustomers(){

    // Creates url object:
    QUrl url("https://connect.squareupsandbox.com/v2/customers");
    QNetworkRequest request(url);

    // Set Request Header:
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", apiCode);
    request.setRawHeader("Square-Version", "2023-04-19");

    // Make POST Request:
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(request);

    // Wait for Response from Server:
    while (!reply->isFinished()) qApp->processEvents();

    // Print out results in terminal:
    QByteArray response_data = reply->readAll();
    QJsonDocument json2 = QJsonDocument::fromJson(response_data);

    reply->deleteLater();
    return json2.object();
}


void HttpRequests::setAPICode(){
    string api;
    ifstream fin;

    // Open file:
    fin.open("apiCode.txt");

    // Reads line contents from fin to String api
    getline (fin, api);

    // Convert String -> QString -> QByteArray
    QString qstr = QString::fromStdString("Bearer " + api);
    apiCode = qstr.toUtf8();
}

void HttpRequests::inactivateTeamMember(QString teamMemberID){
    // Creates url object:
    QUrl url("https://connect.squareupsandbox.com/v2/team-members/" + teamMemberID);
    QNetworkRequest request(url);

    // Set Request Header:
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", apiCode);
    request.setRawHeader("Square-Version", "2023-04-19");

    QJsonObject parent;
    QJsonObject status;

    status["status"] = "INACTIVE";

    parent["team_member"] = status;


    // Make POST Request:
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.put(request, QJsonDocument(parent).toJson());

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


// If result=false, returns the error message
// if result=true, returns the id of the client
QString HttpRequests::addClientMember(QJsonObject json, bool * result){
    QString returnStr = "";

    // Creates url object:
    QUrl url("https://connect.squareupsandbox.com/v2/customers");
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
    QJsonObject output = (QJsonDocument::fromJson(response_data)).object();


    // Checks if there are errors -> e.g. email, phone number
    if(output.contains("errors")){
        QJsonArray listOfErrors = output["errors"].toArray();

        for(int i=0; i<listOfErrors.size();i++){
            QJsonObject temp = listOfErrors[i].toObject();
            reply->deleteLater();
            returnStr = returnStr % temp["detail"].toString() % "\n";
        }
        *result = false;
        reply->deleteLater();
        return returnStr;
    }


    *result = true;
    reply->deleteLater();
    QJsonObject customer = output["customer"].toObject();

    return customer["id"].toString();
}

// Retrieves customer in json format -> use email as input
bool HttpRequests::retrieveCustomer(QString email, QJsonObject * customerData){
    QJsonObject json;
    QJsonObject query;
    QJsonObject filter;
    QJsonObject emailAddress;

    emailAddress["exact"] = email;
    filter["email_address"] = emailAddress;
    query["filter"] = filter;
    json["query"]= query;


    // Creates url object:
    QUrl url("https://connect.squareupsandbox.com/v2/customers/search");
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

    *customerData = json2.object();

    reply->deleteLater();
    return (customerData->contains("customers"));

}


// Returns three elements in a list: {full name, email, phone number}
QList<QString> HttpRequests::getCustomerInfo(QString customerID){
    QUrl url("https://connect.squareupsandbox.com/v2/customers/" % customerID);
    QNetworkRequest request(url);

    // Set Request Header:
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", apiCode);
    request.setRawHeader("Square-Version", "2023-04-19");

    // Make POST Request:
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(request);

    // Wait for Response from Server:
    while (!reply->isFinished()) qApp->processEvents();

    // Print out results in terminal:
    QByteArray response_data = reply->readAll();
    QJsonDocument json2 = QJsonDocument::fromJson(response_data);
    QJsonObject customerInfo = json2.object();
    customerInfo = customerInfo["customer"].toObject();


    reply->deleteLater();

    QList<QString> out = {customerInfo["given_name"].toString() % " " % customerInfo["family_name"].toString(), customerInfo["email_address"].toString() % "\n" % customerInfo["phone_number"].toString()};

    return out;

}

// Returns full name of the team member
QString HttpRequests::getTeamMemberInfo(QString teamMemberID){
    QUrl url("https://connect.squareupsandbox.com/v2/team-members/" % teamMemberID);
    QNetworkRequest request(url);

    // Set Request Header:
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", apiCode);
    request.setRawHeader("Square-Version", "2023-04-19");

    // Make POST Request:
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(request);

    // Wait for Response from Server:
    while (!reply->isFinished()) qApp->processEvents();

    // Print out results in terminal:
    QByteArray response_data = reply->readAll();
    QJsonDocument json2 = QJsonDocument::fromJson(response_data);
    QJsonObject customerInfo = json2.object();
    customerInfo = customerInfo["team_member"].toObject();


    reply->deleteLater();


    return customerInfo["given_name"].toString() % " " % customerInfo["family_name"].toString();
}



