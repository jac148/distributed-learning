#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // 추가

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating client socket\n";
        return -1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); // 수정
    serverAddress.sin_port = htons(5555);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Client is connected to the Parameter Server\n";

    // Simulate sending data to the server (replace with your actual data)
    const char* data = "Training data";
    send(clientSocket, data, strlen(data), 0);

    // Receive acknowledgment from the server
    char buffer[256];
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Received reply from server: " << buffer << "\n";

    // Additional logic for processing server response can be added here

    close(clientSocket);
    return 0;
}
