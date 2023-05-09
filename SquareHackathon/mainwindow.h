#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "httprequests.h"
#include <QMainWindow>
#include <QListWidgetItem>

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

    void on_ViewAllBookings_clicked();

    void on_signInButton_clicked();

private:
    Ui::MainWindow *ui;
    HttpRequests rq;
    QString selectedProfileID;
    QSet<QString> customerEmails; // If there's time -> change QSet to something stored locally
    void showStaffList();
    void showClientList();
    void dropDownForCountryCode();

};
#endif // MAINWINDOW_H
