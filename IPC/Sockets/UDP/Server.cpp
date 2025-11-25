#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main()
{
    cout << " UDP SERVER " << endl;
    
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating UDP socket!" << endl;
        return 1;
    }
    cout << " UDP socket created (SOCK_DGRAM - connectionless)" << endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9090);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Bind failed!" << endl;
        close(serverSocket);
        return 1;
    }
    cout << " Socket bound to port 9090" << endl;

    cout << " Server ready (NO connection needed - UDP is connectionless)" << endl;
    cout << " Waiting for datagrams..." << endl;

    char buffer[1024] = { 0 };
    sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);

    int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0,
                                 (struct sockaddr*)&clientAddress, &clientLen);

    if (bytesReceived > 0) {
        cout << "\n Datagram received!" << endl;
        cout << "   From: " << inet_ntoa(clientAddress.sin_addr) 
             << ":" << ntohs(clientAddress.sin_port) << endl;
        cout << "   Message: " << buffer << endl;
        cout << "   Bytes: " << bytesReceived << endl;
    }

    const char* response = "Hello from UDP Server!";
    sendto(serverSocket, response, strlen(response), 0,
           (struct sockaddr*)&clientAddress, clientLen);
    cout << "\n Response sent back to client" << endl;

    close(serverSocket);
    cout << "\n✓ UDP server closed" << endl;

    return 0;
}
