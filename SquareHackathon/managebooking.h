#ifndef MANAGEBOOKING_H
#define MANAGEBOOKING_H

#include <QtSql>

class ManageBooking
{

struct ScheduleBlock{
    QString customerID;
    double time;
};


public:
    ManageBooking();
    QList<double> getAvailableBookings(QDate date, QString employeeID);
    bool addBooking(QString date, double time, QString customerID, QString employeeID);
    QList<double> getScheduleEmployee(QString date, QString employeeID);
    void getAllSchedules(QString date);




private:
    QSqlDatabase initDB();
    QSqlDatabase mainDB;
};

#endif // MANAGEBOOKING_H
