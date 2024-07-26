class WebSocketServer {
public:
    WebSocketServer(int port) : m_port(port) {}

    void start() {
        int serverSocket = createSocket();
        bindSocket(serverSocket, m_port);
        listenSocket(serverSocket);

        std::cout << "WebSocket server started on port " << m_port << std::endl;

        while (true) {
            int clientSocket = acceptSocket(serverSocket);
            std::thread clientThread(&WebSocketServer::handleClient, this, clientSocket);
            clientThread.detach();
        }
    }

private:
    int m_port;

    int createSocket() {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            throw std::runtime_error("Failed to create server socket");
        }
        return serverSocket;
    }

    void bindSocket(int serverSocket, int port) {
        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1) {
            throw std::runtime_error("Failed to bind server socket");
        }
    }

    void listenSocket(int serverSocket) {
        if (listen(serverSocket, 5) == -1) {
            throw std::runtime_error("Failed to listen on server socket");
        }
    }

    int acceptSocket(int serverSocket) {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddress, &clientAddressSize);
        if (clientSocket == -1) {
            throw std::runtime_error("Failed to accept client socket");
        }
        return clientSocket;
    }

    void handleClient(int clientSocket) {
        WebSocketSession session(clientSocket);
        session.handshake();
        session.handleMessages();
        close(clientSocket);
    }
};

int main() {
    WebSocketServer server(8080);
    server.start();
    return 0;
}
