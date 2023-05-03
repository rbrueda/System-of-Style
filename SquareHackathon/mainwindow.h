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

    void on_pushButton_clicked();

    void on_staffList_itemDoubleClicked(QListWidgetItem *item);



private:
    Ui::MainWindow *ui;
    HttpRequests rq;
    void showStaffList();

};
#endif // MAINWINDOW_H
