#ifndef HTTPREQUESTS_H
#define HTTPREQUESTS_H

#include <QJsonObject>
#include <QJsonDocument>


class HttpRequests
{
public:
    HttpRequests();
    QByteArray apiCode;
    void addTeamMember(QJsonObject json);
    QJsonObject getTeamMembers();
    QJsonObject getCustomers();
    void inactivateTeamMember(QString teamMemberID);
    void addClientMember(QJsonObject json);


private:
    QByteArray getAPICode();
};

#endif // HTTPREQUESTS_H
