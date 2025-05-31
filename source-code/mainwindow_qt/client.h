#pragma once
#ifndef HEADER_H_


#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <tchar.h>
#include "EAGetMailObj.tlh"


using namespace EAGetMailObjLib;
//#include "stdafx.h"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

extern string IP;
extern string function;

extern const int ConnectNormal;
extern const int ConnectSSLAuto;
extern const int ConnectSTARTTLS;
extern const int ConnectDirectSSL;
extern const int ConnectTryTLS;


//void sendEmail(string subject, string body);
//void sendEmail_Attachment(string subject, string body, string attachment);
//void retrieveEmail();
//DWORD  _getCurrentPath(char* lpPath, DWORD nSize);
//void handleClient(SOCKET clientSocket);
//void sendKeyStroke(SOCKET clientSocket, const std::string& keystroke);
void receiveFile(SOCKET clientSocket, const char* destinationPath);
void receiveVideo(int clientSocket);


#endif
