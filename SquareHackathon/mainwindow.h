#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "httprequests.h"
#include "managebooking.h"
#include <QMainWindow>
#include <QListWidgetItem>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_submit_clicked();

    void on_staffList_itemDoubleClicked(QListWidgetItem *item);

    void on_deactivateButton_clicked();

    void on_addEmployeeButton_clicked();

    void on_employeeManagerButton_clicked();

    void on_bookManagerButton_clicked();

    void on_SignpButton_clicked();

    void on_makeAccountButton_clicked();

    void on_client_countryCodeDropDown_activated(int index);

    void on_AddBookingsButton_clicked();

    void on_signInButton_clicked();

    void on_submitDateButton_clicked();


    void on_submitAddBookingButton_clicked();

    void timeAddEmployeeDropdown();


    void on_refreshEmployeeBookings_clicked();

    void on_backButtonEmployeeManager_clicked();

    void on_pushButton_clicked();

    void on_bookingsViewTable_itemDoubleClicked(QTableWidgetItem *item);
   void on_settingsButton_clicked();

    void on_submitSquareApiCodeButton_clicked();

    void on_submitDataBaseLoginButton_clicked();

    void on_pushButton_2_clicked();
    void on_employee_countryCodeDropDown_activated(int index);

    void on_backButton_viewBookings_clicked();

    void on_viewAllBookingsButton_clicked();

    void on_cancel_button_viewAppt_clicked();

    void on_backButton_signInView_clicked();


    void on_backButton_signupView_clicked();

    void on_backButton_clientView_clicked();

    void on_mainMenuButton_clientView_clicked();

private:
    Ui::MainWindow *ui;
    HttpRequests rq;
    ManageBooking mb;
    QString selectedProfileID;
    QString currentClientID;
    QSet<QString> customerEmails; // If there's time -> change QSet to something stored locally
    void showStaffList();
    void showClientList();
    void dropDownForCountryCode();
    void updateAvaliableTimes_AddBooking(QDate date, QString idEmployee);

    void printErrorMessage(QString message);
    void initRowIndexesEmployeeBookingTable();

    QString convertTime(double time);
    void displayForManageBookingView();
    void updateTableCustomer(QString customerID, double row, int col);


};
#endif // MAINWINDOW_H
