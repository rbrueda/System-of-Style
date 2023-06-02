#include "managebooking.h"
#include <QtSql>
#include <iostream>
#include <QPair>

using namespace std;

ManageBooking::ManageBooking()
{

}


bool ManageBooking::initDB(){
    try{
        QFile file("../SquareHackathon/bookingDBLogin.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            cout << "ERROR!" << endl;

        QTextStream in(&file);
        mainDB = QSqlDatabase::addDatabase("QMYSQL");
        mainDB.setHostName(in.readLine());
        mainDB.setDatabaseName("HairSalonBookingManager");
        mainDB.setUserName(in.readLine());
        mainDB.setPassword(in.readLine());
        return mainDB.open();
    }catch(...){ //used so an error widget can pop up instead of the program not being able to run
        //(...) means not parameters needed
        return false; //will be executed since try block contains an error (try block will not be executed?)
    }
}



// SELECT mon_start, mon_end, tues_start, ... FROM EmployeeShifts WHERE employeeID = " ... "
QList<double> ManageBooking::getWorkSchedule(QString employeeID){
    QSqlQuery query("SELECT mon_start, mon_end, tues_start, tues_end, wed_start, wed_end, thurs_start, thurs_end, fri_start, fri_end, sat_start, sat_end, sun_start, sun_end FROM EmployeeShifts WHERE employeeID = \'" % employeeID % "\';");


    QList<double> out;
    //will allow array to be linked
    query.next();
    for (int i = 0; i < 14; i++){
        if (query.value(i).isNull()){
            out.append(-1);
            cout<<"NULL HERE"<<endl;
        }
        else{
            out.append(query.value(i).toDouble());
        }
    }

    for(int i =0; i< out.size(); i++){
        cout << to_string(out[i]) <<endl;
    }
    return out;
}

QList<double> ManageBooking::getAvailableBookings(QDate date, QString employeeID){

    QString dateStr = date.toString("yyyy-MM-dd");

    int dayOfWeek = date.dayOfWeek();

    QSqlQuery query2;
    switch(dayOfWeek){
        case 1:
            cout << "monday" << endl;
            query2.exec("SELECT mon_start, mon_end FROM EmployeeShifts WHERE employeeID=\"" % employeeID % "\";");
            break;
        case 2:
            cout << "tuesday" << endl;
            query2.exec("SELECT tues_start, tues_end FROM EmployeeShifts WHERE employeeID=\"" % employeeID % "\";");
            break;
        case 3:
            cout << "wednessday" << endl;
            query2.exec("SELECT wed_start, wed_end FROM EmployeeShifts WHERE employeeID=\"" % employeeID % "\";");
            break;
        case 4:
            cout << "thurs" << endl;
            query2.exec("SELECT thurs_start, thurs_end FROM EmployeeShifts WHERE employeeID=\"" % employeeID % "\";");
            break;
        case 5:
            cout << "fri" << endl;
            query2.exec("SELECT fri_start, fri_end FROM EmployeeShifts WHERE employeeID=\"" % employeeID % "\";");
            break;
        case 6:
            cout << "sat" << endl;
            query2.exec("SELECT sat_start, sat_end FROM EmployeeShifts WHERE employeeID=\"" % employeeID % "\";");
            break;
        case 7:
            cout << "sun" << endl;
            query2.exec("SELECT sun_start, sun_end FROM EmployeeShifts WHERE employeeID=\"" % employeeID % "\";");
            break;
        default:
            cout << "ERROR" << endl;
    }

    query2.next();
    if(!query2.value(0).isNull()){
        cout << query2.value(0).toString().toStdString() << endl;
        cout << query2.value(1).toString().toStdString() << endl;
    }

    QSqlQuery query("SELECT timeOfDay FROM Appointments WHERE apptDate=DATE(\"" % dateStr % "\") and employeeID=\'" % employeeID % "\' ORDER BY timeOfDay ASC;");

    cout << query.lastQuery().toStdString() << endl;

    QList<double> out;
    double currValue = query2.value(0).toDouble();
    double endTime = query2.value(1).toDouble();

    double timeOfDay = endTime;
    while (query.next()) {
        timeOfDay = query.value(0).toDouble();
        while(currValue < timeOfDay){
            out.append(currValue);
            currValue += 0.5;
        }
        currValue += 0.5;
    }
    while(currValue < endTime){
        out.append(currValue);
        currValue += 0.5;
    }
    for(int i =0; i< out.size(); i++){
        cout << to_string(out[i]) <<endl;
    }
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

QHash<QPair<QString,double>, QString> ManageBooking::getAllSchedules(QString date, QList<QString> * employees){
    QSqlQuery query("SELECT employeeID, customerID, timeOfDay FROM Appointments WHERE apptDate=DATE(\"" % date % "\") ORDER BY timeOfDay ASC;");

    QSet<QString> employeeSet;
    QHash<QPair<QString,double>, QString> out;

//    cout << "WORKS" << endl;
    while (query.next()) {
            QPair<QString, double> data;
            data.first = query.value(0).toString();
            data.second = query.value(2).toDouble();

            out[data] = query.value(1).toString();
            employeeSet.insert(data.first);
    }

    *employees = employeeSet.values();
    return out;
}


bool ManageBooking::addEmployeeSchedule(QString empID, double mon_start, double mon_end,
                                        double tues_start, double tues_end,
                                        double wed_start, double wed_end,
                                        double thurs_start, double thurs_end,
                                        double fri_start, double fri_end,
                                        double sat_start, double sat_end,
                                        double sun_start, double sun_end){
    QSqlQuery query;
    query.prepare("INSERT INTO EmployeeShifts (employeeID, mon_start, mon_end, tues_start, tues_end, wed_start, wed_end, thurs_start, thurs_end, fri_start, fri_end, sat_start, sat_end, sun_start, sun_end)"
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    query.bindValue(0, empID);

    if(mon_start == -1 || mon_end == -1){
        query.bindValue(1, QVariant(QVariant::String));
        query.bindValue(2, QVariant(QVariant::String));
    }else{
        query.bindValue(1, mon_start);
        query.bindValue(2, mon_end);
    }

    if(tues_start == -1 || tues_end == -1){
        query.bindValue(3, QVariant(QVariant::String));
        query.bindValue(4, QVariant(QVariant::String));
    }else{
        query.bindValue(3, tues_start);
        query.bindValue(4, tues_end);
    }

    if(wed_start == -1 || wed_end == -1){
        query.bindValue(5, QVariant(QVariant::String));
        query.bindValue(6, QVariant(QVariant::String));
    }else{
        query.bindValue(5, wed_start);
        query.bindValue(6, wed_end);
    }

    if(thurs_start == -1 || thurs_end == -1){
        query.bindValue(7, QVariant(QVariant::String));
        query.bindValue(8, QVariant(QVariant::String));
    }else{
        query.bindValue(7, thurs_start);
        query.bindValue(8, thurs_end);
    }

    if(fri_start == -1 || fri_end == -1){
        query.bindValue(9, QVariant(QVariant::String));
        query.bindValue(10, QVariant(QVariant::String));
    }else{
        query.bindValue(9, fri_start);
        query.bindValue(10, fri_end);
    }

    if(sat_start == -1 || sat_end == -1){
        query.bindValue(11, QVariant(QVariant::String));
        query.bindValue(12, QVariant(QVariant::String));
    }else{
        query.bindValue(11, sat_start);
        query.bindValue(12, sat_end);
    }

    if(sun_start == -1 || sun_end == -1){
        query.bindValue(13, QVariant(QVariant::String));
        query.bindValue(14, QVariant(QVariant::String));
    }else{
        query.bindValue(13, sun_start);
        query.bindValue(14, sun_end);
    }

    return query.exec();
}

bool ManageBooking::removeSchedule(QString employeeID){
    QSqlQuery query;
    query.prepare("DELETE FROM EmployeeShifts WHERE employeeID=?;");
    query.bindValue(0, employeeID);
    return query.exec();

}


bool ManageBooking::cancelAppointment(QString customerID){
    QSqlQuery query;
    query.prepare("DELETE FROM Appointments WHERE customerID=?;");
    query.bindValue(0, customerID);
    return query.exec();

}

bool ManageBooking::getBookingClient(QString customerID, QString * employeeID, QDate * apptDate, double * timeOfDay){
    QSqlQuery query("SELECT timeOfDay, apptDate, employeeID FROM Appointments WHERE customerID= \"" % customerID % "\";");

    if(!query.next()){
        return false;
    }
    *timeOfDay = query.value(0).toDouble();
    *apptDate = QDate::fromString(query.value(1).toString(), "yyyy-MM-dd");
    *employeeID = query.value(2).toString();
    return true;
}
