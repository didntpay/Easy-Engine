#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

#define PORT "8080"
class InerCom
{
private:
	SOCKET client;

public:
	InerCom();
	void init();
	char* recvData();
	int sendData(char* data, int size);
};

