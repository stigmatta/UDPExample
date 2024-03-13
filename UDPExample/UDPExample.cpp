#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <sstream>
using namespace std;

int main()
{
    WSADATA wsadata;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD point{ 0,0 };
    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //10.4.1.134
    if (udpSocket == INVALID_SOCKET)
    { 
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    cout << "Receiving data...." << endl;
    int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);
    receiveBuf[bytesReceived] = '\0';

    istringstream iss(receiveBuf);
    string hello,name,color;
    getline(iss, hello, '!');
    getline(iss, name, ',');
    getline(iss, color);
    int nColor=0;
    nColor = stoi(color);

    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "recvfrom failed with error " << WSAGetLastError() << endl;
        return 4;
    }

    cout << "Received from " << senderAddr.sin_addr.s_host << endl;
    point.X = 6,point.Y = 2;
    cout << "Data: ";
    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, nColor);
    cout << receiveBuf << endl;
    SetConsoleTextAttribute(h, 7);
    const size_t sendBufSize = 1024;
    char sendBuf[sendBufSize] = "Hello from UDPExample";

    int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
    if (sendResult == SOCKET_ERROR)
    {
        cout << "sendto failed with error " << WSAGetLastError() << endl;
        return 4;
    }


    closesocket(udpSocket);
    WSACleanup();
}

