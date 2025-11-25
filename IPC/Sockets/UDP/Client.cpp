#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main()
{
    cout << " UDP CLIENT " << endl;

    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        cerr << "Error creating UDP socket!" << endl;
        return 1;
    }
    cout << " UDP socket created (SOCK_DGRAM - connectionless)" << endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9090);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    cout << " Ready to send (NO connection needed - UDP is connectionless)" << endl;

    const char* message = "Hello from UDP Client!";
    int bytesSent = sendto(clientSocket, message, strlen(message), 0,
                           (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    if (bytesSent > 0) {
        cout << "\n Datagram sent!" << endl;
        cout << "   To: 127.0.0.1:9090" << endl;
        cout << "   Message: " << message << endl;
        cout << "   Bytes: " << bytesSent << endl;
    }

    cout << "\n Waiting for response..." << endl;
    char buffer[1024] = { 0 };
    sockaddr_in fromAddress;
    socklen_t fromLen = sizeof(fromAddress);

    int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0,
                                 (struct sockaddr*)&fromAddress, &fromLen);

    if (bytesReceived > 0) {
        cout << "\n Response received!" << endl;
        cout << "   From: " << inet_ntoa(fromAddress.sin_addr) 
             << ":" << ntohs(fromAddress.sin_port) << endl;
        cout << "   Message: " << buffer << endl;
        cout << "   Bytes: " << bytesReceived << endl;
    }

    close(clientSocket);
    cout << "\n UDP client closed" << endl;

    return 0;
}
