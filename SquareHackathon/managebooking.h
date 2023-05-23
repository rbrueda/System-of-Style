#ifndef MANAGEBOOKING_H
#define MANAGEBOOKING_H

#include <QtSql>

class ManageBooking
{


public:
    struct ScheduleBlock{
        QString customerID;
        double time;
    }scheduleBlock;

    ManageBooking();
    QList<double> getWorkSchedule(QString employeeID);
    QList<double> getAvailableBookings(QDate date, QString employeeID);
    bool addBooking(QString date, double time, QString customerID, QString employeeID);
    QList<double> getScheduleEmployee(QString date, QString employeeID);
    QHash<QString, QPair<QString, double>> getAllSchedules(QString date);

    bool addEmployeeSchedule(QString empID, double mon_start, double mon_end,
                                            double tues_start, double tues_end,
                                            double wed_start, double wed_end,
                                            double thurs_start, double thurs_end,
                                            double fri_start, double fri_end,
                                            double sat_start, double sat_end,
                                            double sun_start, double sun_end);


private:
    QSqlDatabase initDB();
    QSqlDatabase mainDB;
};

#endif // MANAGEBOOKING_H
