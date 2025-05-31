#include "thirdwindow.h"
#include "ui_thirdwindow.h"
#include "client.h"
#include "QMessageBox"



string IP;
string request;

ThirdWindow::ThirdWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ThirdWindow)
{
    ui->setupUi(this);
    this->show();

    retrieveMail();
}

ThirdWindow::~ThirdWindow()
{
    delete ui;
}


DWORD  _getCurrentPath(LPTSTR lpPath, DWORD nSize)
{
    DWORD dwSize = ::GetModuleFileName(NULL, lpPath, nSize);
    if (dwSize == 0 || dwSize == nSize)
    {
        return 0;
    }

    // Change file name to current full path
    LPCTSTR psz = _tcsrchr(lpPath, _T('\\'));
    if (psz != NULL)
    {
        lpPath[psz - lpPath] = _T('\0');
        return _tcslen(lpPath);
    }

    return 0;
}
void retrieveEmail()
{
    const int MailServerPop3 = 0;
    const int MailServerImap4 = 1;
    const int MailServerEWS = 2;
    const int MailServerDAV = 3;
    const int MailServerMsGraph = 4;

    const int GetMailInfos_All = 1;
    const int GetMailInfos_NewOnly = 2;
    const int GetMailInfos_ReadOnly = 4;
    const int GetMailInfos_SeqRange = 8;
    const int GetMailInfos_UIDRange = 16;
    const int GetMailInfos_PR_ENTRYID = 32;
    const int GetMailInfos_DateRange = 64;
    const int GetMailInfos_OrderByDateTime = 128;

    // Initialize COM environment
    ::CoInitialize(NULL);

    // Create a folder named "inbox" under current exe file directory
    // to save the emails retrieved.

    TCHAR szPath[MAX_PATH + 1];
    _getCurrentPath(szPath, MAX_PATH);

    TCHAR szMailBox[MAX_PATH + 1];
    wsprintf(szMailBox, _T("%s\\inbox"), szPath);

    // Create a folder to store emails
    ::CreateDirectory(szMailBox, NULL);

    try
    {
        IMailServerPtr oServer = NULL;
        oServer.CreateInstance(__uuidof(EAGetMailObjLib::MailServer));

        // Gmail IMAP4 server address
        oServer->Server = _T("imap.gmail.com");
        oServer->User = _T("tranthithuytien1003@gmail.com");

        // Create app password in Google account
        // https://support.google.com/accounts/answer/185833?hl=en
        oServer->Password = _T("reig xdjz ncpa zmst");
        oServer->Protocol = MailServerImap4;

        // Enable SSL connection
        oServer->SSLConnection = VARIANT_TRUE;

        // Set 993 IMAP4 SSL port
        oServer->Port = 993;

        IMailClientPtr oClient = NULL;
        oClient.CreateInstance(__uuidof(EAGetMailObjLib::MailClient));
        oClient->LicenseCode = _T("TryIt");

        oClient->Connect(oServer);
        _tprintf(_T("Connected\r\n"));

        // retrieve unread/new email only
        oClient->GetMailInfosParam->Reset();
        oClient->GetMailInfosParam->GetMailInfosOptions = GetMailInfos_All;

        IMailInfoCollectionPtr infos = oClient->GetMailInfoList();
        //_tprintf(_T("Total %d unread emails\r\n"), infos->Count);

        int countChecked = 0;

        for (long i = infos->Count - 1; i >= 0 && countChecked <= 100; i--, countChecked++)
        {
            IMailInfoPtr pInfo = infos->GetItem(i);
            EAGetMailObjLib::IMailPtr oMail = oClient->GetMail(pInfo);

            // Đánh dấu đã đọc
            oClient->MarkAsRead(pInfo, VARIANT_TRUE);

            // Kiểm tra tiêu đề
            _bstr_t subject = oMail->Subject;
            if (subject.length() > 5 && wcsncmp(subject, L"[MMT]", 5) == 0)
            {
                request = (LPCSTR)subject;
                IP = (LPCSTR)oMail->TextBody;
                break; // Dừng khi tìm thấy email thỏa mãn
            }
        }

        if (request.empty())
        {
            _tprintf(_T("No new actions needed to do!\n"));
        }

        oClient->Quit();
    }
    catch (_com_error& ep) {
        _tprintf(_T("Error: %s\n"), (const TCHAR*)ep.Description());
    }
}

#include "EASendMailObj.tlh"
using namespace EASendMailObjLib;


const int ConnectNormal = 0;
const int ConnectSSLAuto = 1;
const int ConnectSTARTTLS = 2;
const int ConnectDirectSSL = 3;
const int ConnectTryTLS = 4;

void ThirdWindow::sendEmail(QString subject, QString body)
{
    ::CoInitialize(NULL);
    EASendMailObjLib::IMailPtr oSmtp = NULL;
    oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));
    oSmtp->LicenseCode = "TryIt";

    oSmtp->FromAddr = "tranthithuytien1003@gmail.com";
    oSmtp->AddRecipientEx("tranthithuytien1003@gmail.com", 0);

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

void ThirdWindow::sendEmail_Attachment(QString subject, QString body, QString attachment)
{
    ::CoInitialize(NULL);
    EASendMailObjLib::IMailPtr oSmtp = NULL;
    oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));
    oSmtp->LicenseCode = "TryIt";

    oSmtp->FromAddr = "tranthithuytien1003@gmail.com";
    oSmtp->AddRecipientEx("tranthithuytien1003@gmail.com", 0);

    oSmtp->Subject = subject.toStdString().c_str();
    oSmtp->BodyText = body.toStdString().c_str();
    oSmtp->AddAttachment(attachment.toStdString().c_str());

    ifstream checkFile(attachment.toStdString().c_str(), ios::binary);
    if (!checkFile) {
        cerr << "Attachment file not found or inaccessible." << endl;
        return;
    }
    checkFile.close();
    if (checkFile.seekg(0, ios::end).tellg() == 0) {
        cerr << "File is empty." << endl;
        return;
    }


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
    }
}

void ThirdWindow::handleClient(SOCKET clientSocket)
{
    // Nhận dữ liệu từ client (ảnh chụp màn hình)
    char buffer[4096];
    int receivedBytes;
    std::ofstream outFile("result.png", std::ios::binary);

    if (outFile.is_open()) {
        while ((receivedBytes = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            outFile.write(buffer, receivedBytes);
        }
        outFile.close();
    }
    else {
        std::cerr << "Failed to write to file." << std::endl;
    }

    if (receivedBytes < 0) {
        int errorCode = WSAGetLastError();
        std::cerr << "recv failed with error: " << errorCode << std::endl;
    }

    // Gửi email với ảnh đính kèm
    sendEmail_Attachment(QString::fromStdString("Capture Screen"), QString::fromStdString("Here is the screenshot."), QString::fromStdString("result.png"));
}

void ThirdWindow::receiveKeyloggerData(SOCKET clientSocket)
{
    char buffer[4096];
    int receivedBytes;

    // Tạo file lưu dữ liệu keylogger nhận từ server
    ofstream keyLogFile("received_keylogger_data_log.txt", ios::binary);

    if (!keyLogFile.is_open())
    {
        cerr << "Failed to open received_keylogger_data_log.txt to save keylogger data." << endl;
        return;
    }

    cout << "Receiving keylogger data from server..." << endl;

    // Nhận dữ liệu từ server và ghi vào file
    while ((receivedBytes = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0)
    {
        keyLogFile.write(buffer, receivedBytes); // Ghi dữ liệu trực tiếp vào file
        QString message = QString("Received %1 bytes").arg(receivedBytes);
        QMessageBox::information(this, "Received", message);
    }

    if (receivedBytes == 0)
    {
        cout << "Keylogger data transmission completed." << endl;
    }
    else if (receivedBytes == SOCKET_ERROR)
    {
        QMessageBox::information(this, "ERROR", "Error while receiving keylogger data from server.");
    }

    keyLogFile.close();
    cout << "File successfully saved as 'received_keylogger_data_log.txt'." << endl;
}


void receiveFile(SOCKET clientSocket, const char* destinationPath) 
{
    ofstream dest(destinationPath, ios::binary);
    if (!dest.is_open()) {
        cerr << "Error opening destination file." << endl;
        return;
    }

    char buffer[1024];
    int bytesReceived = 0;

    // Nhận dữ liệu qua socket và ghi vào file
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        dest.write(buffer, bytesReceived);  // Ghi vào file
    }

    if (bytesReceived == 0) {
        cout << "File received successfully." << endl;
    }
    else if (bytesReceived == SOCKET_ERROR) {
        cerr << "Error receiving file." << endl;
    }

    dest.close();
}

void receiveVideo(int clientSocket)
{
    // Nhận kích thước file từ server
    std::streamsize fileSize;
    if (recv(clientSocket, (char*)&fileSize, sizeof(fileSize), 0) <= 0) {
        std::cerr << "Failed to receive file size!" << std::endl;
        return;
    }
    std::cout << "Receiving file of size: " << fileSize << " bytes" << std::endl;

    // Mở file để ghi dữ liệu video
    std::ofstream outFile("received_video.mp4", std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Cannot open file to save video!" << std::endl;
        return;
    }

    // Nhận dữ liệu video từ server và ghi vào file
    char buffer[1024];
    std::streamsize bytesReceived = 0;

    while (bytesReceived < fileSize) {
        int chunkSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (chunkSize <= 0) {
            std::cerr << "Connection closed or error occurred during receiving!" << std::endl;
            break;
        }
        outFile.write(buffer, chunkSize);
        bytesReceived += chunkSize;

        //std::cout << "Received: " << bytesReceived << "/" << fileSize << " bytes" << std::endl;
    }

    outFile.close();
    std::cout << "Video received and saved as 'received_video.mp4' successfully!" << std::endl;
}

void ThirdWindow::retrieveMail()
{
    // Khởi tạo socket và kết nối tới server
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    // Khởi tạo Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Winsock creation failed!" << endl;
        return;
    }

    // Tạo socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed!" << endl;
        WSACleanup();
        return;
    }

    // Đọc email để lấy địa chỉ IP và yêu cầu
    retrieveEmail();

    // Cấu hình địa chỉ server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9090);  // Cổng server
    serverAddr.sin_addr.s_addr = inet_addr(IP.c_str());  // Địa chỉ IP server

    // Kết nối đến server
    if (::connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cerr << "Can not connect!" << endl;
        WSACleanup();
        return;
    }

    // In địa chỉ IP và yêu cầu lên label
    ui->IP->setText(QString::fromStdString("IP: " + IP));
    ui->request->setText(QString::fromStdString("Request: " + request.substr(6)));
    // Đóng kết nối

    // Gửi yêu cầu đến server
    if (strncmp(request.substr(6).c_str(), "List App", 8) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);

        string result;
        char tmp[1024];
        int bytesReceived = 0;

        while (true)
        {
            bytesReceived = recv(clientSocket, tmp, sizeof(tmp), 0);

            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                break;  // Nếu không nhận được dữ liệu hoặc gặp lỗi, thoát khỏi vòng lặp
            }

            tmp[bytesReceived] = '\0';
            result += tmp;
        }
        sendEmail(QString::fromStdString("[MMT | Result] List App"), QString::fromStdString(result));
    }

    else if (strncmp(request.substr(6).c_str(), "Start App", 9) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);

        sendEmail(QString::fromStdString("[MMT | Result] Start App"), QString::fromStdString("Start successfully!"));


    }
    else if (strncmp(request.substr(6).c_str(), "Stop App", 8) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        sendEmail(QString::fromStdString("[MMT | Result] Stop App"), QString::fromStdString("Stop successfully!"));

    }
    else if (strncmp(request.substr(6).c_str(), "List Services", 14) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);

        string result;
        char tmp[1024];
        int bytesReceived = 0;

        while (true)
        {
            bytesReceived = recv(clientSocket, tmp, sizeof(tmp), 0);

            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                break;  // Nếu không nhận được dữ liệu hoặc gặp lỗi, thoát khỏi vòng lặp
            }

            tmp[bytesReceived] = '\0';
            result += tmp;
        }
        sendEmail(QString::fromStdString("[MMT | Result] List Services"), QString::fromStdString(result));
    }
    else if (strncmp(request.substr(6).c_str(), "Start Service", 13) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        sendEmail(QString::fromStdString("[MMT | Result] Start Service"), QString::fromStdString("Start successfully"));

    }
    else if (strncmp(request.substr(6).c_str(), "Stop Service", 12) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        sendEmail(QString::fromStdString("[MMT | Result] Stop Service"), QString::fromStdString("Stop successfully"));
    }
    else if (strncmp(request.substr(6).c_str(), "Shutdown", 8) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        sendEmail(QString::fromStdString("[MMT | Result] Shutdown"), QString::fromStdString("Shutdown successfully"));

    }
    else if (strncmp(request.substr(6).c_str(), "Reset", 5) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        sendEmail(QString::fromStdString("[MMT | Result] Reset"), QString::fromStdString("Reset successfully"));

    }
    else if (strncmp(request.substr(6).c_str(), "Copy File", 9) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        string path;
        for (int i = request.size() - 1; i >= 0; --i)
        {
            if (request[i] == '.') {
                // Nếu gặp dấu chấm, dừng và in ra các ký tự phía sau dấu chấm
                for (int j = i + 1; j < request.size(); ++j) {
                    path += request[j];
                }
                break;
            }
        }
        receiveFile(clientSocket, ("copied_file." + path).c_str());
        sendEmail_Attachment(QString::fromStdString("[MMT | Result] Copy File"), QString::fromStdString("This is your file: "), QString::fromStdString(("copied_file." + path).c_str()));

    }
    else if (strncmp(request.substr(6).c_str(), "Delete File", 11) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        sendEmail(QString::fromStdString("[MMT | Result] Delete File"), QString::fromStdString("Delete successfully"));

    }
    else if (strncmp(request.substr(6).c_str(), "Capture Screen", 14) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        receiveFile(clientSocket, "result.png");
        sendEmail_Attachment(QString::fromStdString("[MMT | Result] Capture Screen"), QString::fromStdString("This is your file: "), QString::fromStdString("result.png"));
    }
    else if (strncmp(request.substr(6).c_str(), "Keylogger", 9) == 0)
    {

        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);

        receiveFile(clientSocket, ("keylogger.txt"));
        sendEmail_Attachment("[MMT | Result] Keylogger", "Keywords have been pressed: ", "keylogger.txt");
        
    }
    else if (strncmp(request.substr(6).c_str(), "Start Webcam", 12) == 0)
    {
        send(clientSocket, request.substr(6).c_str(), strlen(request.substr(6).c_str()), 0);
        receiveVideo(clientSocket);
        sendEmail_Attachment(QString::fromStdString("[MMT | Result] Start Webcam"), QString::fromStdString("This is your video: "), QString::fromStdString("received_video.mp4"));
    }


    closesocket(clientSocket);
    WSACleanup();
}


