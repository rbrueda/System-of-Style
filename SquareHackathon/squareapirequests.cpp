#include "squareapirequests.h"
#include <QUrl>
#include <QtNetwork>


SquareAPIRequests::SquareAPIRequests(QObject *parent)
    : QObject{parent}
{

}

void SquareAPIRequests::addTeamMember(QJsonObject json){
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
