#pragma once

#include <QMainWindow>
#include "ui_fourthwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui {
	class FourthWindow;
}
QT_END_NAMESPACE

class FourthWindow : public QMainWindow
{
	Q_OBJECT

public:
	FourthWindow(QWidget *parent = nullptr);
	~FourthWindow();
private slots:
	void on_pushButton_send_clicked();
	void sendEmail(QString receiver, QString subject, QString body);


private:
	Ui::FourthWindow* ui;
};
