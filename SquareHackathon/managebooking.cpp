#include "managebooking.h"
#include <QtSql>
#include <iostream>

using namespace std;

ManageBooking::ManageBooking()
{
    mainDB = initDB();
}

QSqlDatabase ManageBooking::initDB(){
    QFile file("../SquareHackathon/bookingDBLogin.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        cout << "ERROR!" << endl;


    QTextStream in(&file);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(in.readLine());
    db.setDatabaseName("HairSalonBookingManager");
    db.setUserName(in.readLine());
    db.setPassword(in.readLine());
    bool ok = db.open();
    return db;
}

QList<double> ManageBooking::getAvailableBookings(QDate date, QString employeeID){

    QString dateStr = date.toString("yyyy-MM-dd");

    int dayOfWeek = date.dayOfWeek();

    QSqlQuery query2;
    switch(dayOfWeek){
    case 1:


    }



    QSqlQuery query("SELECT timeOfDay FROM Appointments WHERE apptDate=DATE(" % dateStr % ") and employeeID=\'" % employeeID % "\' ORDER BY timeOfDay ASC;");

//    cout << query.lastQuery().toStdString() << endl;
    QList<double> out;
    double currValue = 0;
    double timeOfDay = 24;
    while (query.next()) {
        timeOfDay = query.value(0).toDouble();
        while(currValue < timeOfDay){
            out.append(currValue);
            currValue += 0.5;
        }
        currValue += 0.5;
    }
    while(currValue < 24){
        out.append(currValue);
        currValue += 0.5;
    }
//    for(int i =0; i< out.size(); i++){
//        cout << to_string(out[i]) <<endl;
//    }
    return out;
}

bool ManageBooking::addBooking(QString date, double time, QString customerID, QString employeeID){
    QSqlQuery query;
    query.prepare("INSERT INTO Appointments (apptDate, timeOfDay, customerID, employeeID)"
                  "VALUES (?, ?, ?, ?)");
    query.bindValue(0, date);
    query.bindValue(1, time);
    query.bindValue(2, customerID);
    query.bindValue(3, employeeID);
    return query.exec();
}

QList<double> ManageBooking::getScheduleEmployee(QString date, QString employeeID){
    QSqlQuery query("SELECT timeOfDay FROM Appointments WHERE apptDate=DATE(" % date % ") and employeeID=\'" % employeeID % "\' ORDER BY timeOfDay ASC;");

    QList<double> out;
    while (query.next()) {
        out.append(query.value(0).toDouble());

    }

        for(int i =0; i< out.size(); i++){
            cout << to_string(out[i]) <<endl;
        }
    return out;
}

void ManageBooking::getAllSchedules(QString date){
    QSqlQuery query("SELECT timeOfDay, employeeID, customerID FROM Appointments WHERE apptDate=DATE(\"" % date % "\") ORDER BY timeOfDay ASC;");

//    QHash<QString, struct  ScheduleBlock> out;

//    cout << "WORKS" << endl;
    while (query.next()) {
            cout << query.value(0).toString().toStdString() << endl;
            cout << query.value(1).toString().toStdString() << endl;
            cout << query.value(2).toString().toStdString() << endl;
            cout << "---" << endl;

//            struct ScheduleBlock;

            // out.append(query.value(0).toDouble());
    }

//    for(int i =0; i< out.size(); i++){
//            cout << to_string(out[i]) <<endl;
//    }
//    return out;
}

