#include "fourthwindow.h"
#include "ui_secondwindow.h"
#include "QMessageBox"
#include "QLabel"
#include "client.h"

FourthWindow::FourthWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::FourthWindow)
{
	ui->setupUi(this);
    this->show();
}

FourthWindow::~FourthWindow()
{
	delete ui;
}

#include "EASendMailObj.tlh"
using namespace EASendMailObjLib;

void FourthWindow::sendEmail(QString receiver, QString subject, QString body)
{
    ::CoInitialize(NULL);
    EASendMailObjLib::IMailPtr oSmtp = NULL;
    oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));
    oSmtp->LicenseCode = "TryIt";

    oSmtp->FromAddr = "tranthithuytien1003@gmail.com";
    oSmtp->AddRecipientEx(receiver.toStdString().c_str(), 0);

    oSmtp->Subject = subject.toStdString().c_str();
    oSmtp->BodyText = body.toStdString().c_str();

    oSmtp->ServerAddr = "smtp.gmail.com";
    oSmtp->UserName = "tranthithuytien1003@gmail.com";
    oSmtp->Password = "reig xdjz ncpa zmst";
    oSmtp->ServerPort = 587;
    oSmtp->ConnectType = ConnectSSLAuto;

    if (oSmtp->SendMail() == 0) {
        QMessageBox::information(this, "Send email", "Send successfully");
    }
    else {
        QMessageBox::warning(this, "Send email", "Send failed");
        //cout << "Failed to send email: " << (const char*)oSmtp->GetLastErrDescription() << endl;
    }
}

void FourthWindow::on_pushButton_send_clicked()
{
    QString to = ui->lineEdit_to->text();
    QString title = ui->lineEdit_title->text();
    QString context = ui->lineEdit_context->text();

    if (!to.isEmpty() && !title.isEmpty() && !context.isEmpty())
    {
        sendEmail(to, title, context);
    }

    else
    {
        QMessageBox::warning(this, "Send mail", "Not enough information. Please try again!");
    }
}
