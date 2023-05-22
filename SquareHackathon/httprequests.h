#ifndef HTTPREQUESTS_H
#define HTTPREQUESTS_H

#include <QJsonObject>
#include <QJsonDocument>


class HttpRequests
{
public:
    HttpRequests();
    QByteArray apiCode;
    QString addTeamMember(QJsonObject json);
    QJsonObject getTeamMembers();
    QJsonObject getCustomers();
    void inactivateTeamMember(QString teamMemberID);
    QString addClientMember(QJsonObject json, bool * result);
    bool retrieveCustomer(QString email, QJsonObject * customerData);




private:
    QByteArray getAPICode();
};

#endif // HTTPREQUESTS_H
