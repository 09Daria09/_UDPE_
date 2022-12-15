#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
    setlocale(LC_ALL, "ru");
    WSADATA wsadata;

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

    sockaddr_in addrTo;
    addrTo.sin_family = AF_INET;
    addrTo.sin_port = htons(23000);
    inet_pton(AF_INET, "192.168.2.32", &addrTo.sin_addr.s_addr);

    const size_t sendBufSize = 1024;
    char sendBuf[sendBufSize] = "I would like Cornburger, AceCorn and Corn-Cola";

    cout << "Sending data...." << endl;
    int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&addrTo, sizeof(addrTo));
    if (sendResult == SOCKET_ERROR)
    {
        cout << "sendto failed with error " << WSAGetLastError() << endl;
        return 4;
    }
    while (true)
    {
        if (_kbhit())
        {
            _getch();
        }
        const size_t receiveBufSize = 1024;
        char receiveBuf[receiveBufSize];

        sockaddr_in addrFrom;
        int addrFromSize = sizeof(addrFrom);

        int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&addrFrom, &addrFromSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "recvfrom failed with error " << WSAGetLastError() << endl;
            return 4;
        }

        receiveBuf[bytesReceived] = '\0';

        cout << "Data: " << receiveBuf << endl;
    }

    closesocket(udpSocket);
    WSACleanup();
}

