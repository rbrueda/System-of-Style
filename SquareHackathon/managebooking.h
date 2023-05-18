#ifndef MANAGEBOOKING_H
#define MANAGEBOOKING_H

#include <QtSql>

class ManageBooking
{
public:
    ManageBooking();
    QList<double> getAvailableBookings(QString date, QString employeeID);
    bool addBooking(QString date, double time, QString customerID, QString employeeID);
    QList<double> getScheduleEmployee(QString date, QString employeeID);



private:
    QSqlDatabase initDB();
    QSqlDatabase mainDB;
};

#endif // MANAGEBOOKING_H
