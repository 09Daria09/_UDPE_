#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <map>
#include <iostream>
using namespace std;
typedef map<string, int> MyMap;
typedef MyMap::iterator Iter;

int main()
{
	setlocale(LC_ALL, "ru");
	WSADATA wsadata;
	MyMap m;
	Iter it;
	char buf[200];
	m.insert(pair<string, int>("SweetCorn", 5000));
	m.insert(pair<string, int>("Cornburger", 9000));
	m.insert(pair<string, int>("Corn-Cola", 2000));
	m.insert(pair<string, int>("AceCorn", 4000));

	int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (res != NO_ERROR)
	{
		cout << "WSAStartup failked with error " << res << endl;
		return 1;
	}

	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpSocket == INVALID_SOCKET)
	{
		cout << "socket failed with error " << WSAGetLastError() << endl;
		return 2;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(23000);
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);
	if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
	{
		cout << "bind failed with error " << WSAGetLastError() << endl;
		return 3;
	}



	while (true)
	{
		const size_t receiveBufSize = 1024;
		char receiveBuf[receiveBufSize];

		sockaddr_in senderAddr;
		int senderAddrSize = sizeof(senderAddr);
		int sec = 0;
		cout << "Receiving data...." << endl;
		int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

		if (bytesReceived == SOCKET_ERROR)
		{
			cout << "recvfrom failed with error " << WSAGetLastError() << endl;
			return 4;
		}

		receiveBuf[bytesReceived] = '\0';

		string stringForClient;
		for (int i = 0; i < bytesReceived; i++)
		{
			stringForClient.push_back(receiveBuf[i]);
		}

		cout << "Data: " << stringForClient << endl;

		for (Iter it = m.begin(); it != m.end(); it++)
		{
			int index = 0;
			while ((index = stringForClient.find(it->first, index)) != std::string::npos) {
				index += it->first.length();
				sec += it->second;
				cout << it->first << endl;
			}
		}

		const size_t sendBufSize1 = 1024;
		char sendBuf1[sendBufSize1] = "Рады вас приветствовать в Cornburger ^)";
		int sendResult1 = sendto(udpSocket, sendBuf1, strlen(sendBuf1), 0, (SOCKADDR*)&senderAddr, senderAddrSize);

		const size_t sendBufSize2 = 1024;
		char sendBuf2[sendBufSize2] = "Ваш заказ будет готовиться ";
		char sendSec[sendBufSize2] = " секунд";
		sprintf_s(buf, "%d", sec);
		strcat_s(sendBuf2, buf);
		strcat_s(sendBuf2, sendSec);
		int sendResult2 = sendto(udpSocket, sendBuf2, strlen(sendBuf2), 0, (SOCKADDR*)&senderAddr, senderAddrSize);

		Sleep(sec);
		const size_t sendBufSize = 1024;
		char sendBuf[sendBufSize] = "Ваш заказ готов :) \nХорошего вам Cornстроения";
		int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);


		if (sendResult == SOCKET_ERROR)
		{
			cout << "sendto failed with error " << WSAGetLastError() << endl;
			return 4;
		}

	}
	closesocket(udpSocket);
	WSACleanup();

}

