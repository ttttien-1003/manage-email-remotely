#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_GMAIL->text();
    QString pass = ui->lineEdit_password->text();

    if (username == "admin" && pass == "123")
    {
        QMessageBox::information(this, "Login", "Login successfully");
        this->close();  // Close the current login window
        // Open the new window after successful login
        SecondWindow* secondWindow = new SecondWindow(this);
        secondWindow->show();
    }
    else {
        QMessageBox::warning(this, "Login", "Wrong Gmail or App password. Please try again!");
    }
}

