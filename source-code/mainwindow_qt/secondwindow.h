#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
//#include "ui_secondwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class SecondWindow;
}
QT_END_NAMESPACE

class SecondWindow : public QMainWindow
{
	Q_OBJECT

public:
	SecondWindow(QWidget* parent = nullptr);
	~SecondWindow();

private slots:
	void on_pushButton_readMail_clicked();
	void on_pushButton_readMail_2_clicked();
	void updateDateTime(); // Thêm hàm slot để cập nhật ngày giờ

private:
	Ui::SecondWindow* ui;
};

#endif