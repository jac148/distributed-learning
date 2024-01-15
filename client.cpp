#include <iostream>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Client {
public:
    Client(int numParameters) : numParameters(numParameters) {
        // Initialize the socket
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            throw std::runtime_error("Error creating socket");
        }

        // Server address
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(50000);
        serverAddr.sin_addr.s_addr = inet_addr("192.168.200.138");

        // Connect to the server
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            throw std::runtime_error("Error connecting to server");
        }
    }

    ~Client() {
        close(clientSocket);
    }

    void sendUpdate(const std::vector<double>& update) {
        send(clientSocket, update.data(), update.size() * sizeof(double), 0);
    }

    std::vector<double> getServerModel() {
        std::vector<double> model(numParameters, 0.0);

        // logic for receiving the model from the server
        // ...

        return model;
    }

    void run() {
        try {
            for (int i = 0; i < 10; ++i) {
                // local computation using the current model
                // ...

                // Send request to server to get the current model
                std::vector<double> currentModel = getServerModel();

                // Send update to server
                sendUpdate(currentModel);

                sleep(1);  // Simulate some processing time
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in run: " << e.what() << std::endl;
        }
    }

private:
    int numParameters;
    int clientSocket;
    struct sockaddr_in serverAddr;
};

int main() {
    try {
        // input: Num of parameters for  machine learning model
        int numParameters;
        std::cout << "Enter the number of parameters for the model: ";
        std::cin >> numParameters;

        Client client(numParameters);

        client.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    }

    return 0;
}
