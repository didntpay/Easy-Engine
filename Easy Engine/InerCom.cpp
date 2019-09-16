#include "stdafx.h"
#include "InerCom.h"


InerCom::InerCom()
{
	this->client = INVALID_SOCKET;
}

void InerCom::init()
{
	WSADATA wsaData;

	auto start_iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (start_iResult != 0) {
		printf("WSAStartup failed: %d\n", start_iResult);
		return;
	}

	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	auto iResult = getaddrinfo(NULL, PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}
	else
	{
		SOCKET listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		if (listener == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return;
		}

		printf("Sucessfully initialized socket\n");

		auto bind_iResult = bind(listener, result->ai_addr, (int)result->ai_addrlen);
		listen(listener, SOMAXCONN);

		sockaddr_in client_ip;
		int client_size = sizeof(client_ip);

		this->client = accept(listener, (sockaddr*)&client_ip, &client_size);

		//stop it from listning for now		
		closesocket(listener);

	}
}

char* InerCom::recvData() 
{
	char buff[4096];
	while (true)//set up time out later
	{
		memset(buff, 0, 4096);

		int bytereceived = recv(this->client, buff, 4096, 0);

		//send(client, buff, bytereceived + 1, 0);
		if (bytereceived != 0)
		{
			return buff;
		}
	}
}

int InerCom::sendData(char* data, int size)
{
	auto iResult = send(this->client, data, size + 1, 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("Error %d occured when sending data", iResult);
		closesocket(this->client);
		WSACleanup();
		return -1;
	}

	return 0;
}