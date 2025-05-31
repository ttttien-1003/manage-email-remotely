#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "thirdwindow.h"
#include "fourthwindow.h"

#include "QMessageBox"
#include "QLabel"

SecondWindow::SecondWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SecondWindow)
{
    ui->setupUi(this);

    // Tạo một timer để cập nhật ngày giờ mỗi giây
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SecondWindow::updateDateTime);
    timer->start(1000);  // Cập nhật mỗi giây

    // Gọi hàm cập nhật lần đầu
    updateDateTime();
}


SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::updateDateTime()
{
    // Lấy ngày giờ hiện tại
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Định dạng ngày giờ
    QString dateTimeString = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");

    // Hiển thị trên QLabel
    ui->label->setText(dateTimeString);
}

void SecondWindow::on_pushButton_readMail_clicked()
{
    this->close();  // Close the current login window
    // Open the new window after successful login
    ThirdWindow* thirdWindow = new ThirdWindow(this);
    thirdWindow->show();
}

// Hiển thị thông báo "gửi mail" khi nhấn nút pushButton_sendMail
void SecondWindow::on_pushButton_readMail_2_clicked()
{
    this->close();
    FourthWindow* fourthWindow = new FourthWindow(this);
    fourthWindow->show();
}
