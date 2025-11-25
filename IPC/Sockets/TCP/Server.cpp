#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating socket!" << endl;
        return 1;
    }
    cout << "Server socket created successfully" << endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Bind failed!" << endl;
        close(serverSocket);
        return 1;
    }
    cout << "Socket bound to port 8080" << endl;

    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed!" << endl;
        close(serverSocket);
        return 1;
    }
    cout << "Server listening on port 8080..." << endl;

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        cerr << "Accept failed!" << endl;
        close(serverSocket);
        return 1;
    }
    cout << "Client connected!" << endl;


    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        cout << "Message from client: " << buffer << endl;
    }

    close(clientSocket);
    close(serverSocket);
    cout << "Server closed" << endl;

    return 0;
}