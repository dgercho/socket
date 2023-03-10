#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <ws2tcpip.h>
typedef uint32_t socklen_t;
#else // for Unix
typedef int SOCKET;
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <string>

class Socket
{
public:
    /// @brief Enum of protocols
    enum class Protocol
    {
        TCP,
        UDP
    };

    /// @brief Default constructor of `Socket`.
    /// @param protocol (Protocol) - the protocol used by the socket.
    Socket(Protocol protocol = Protocol::TCP);

    ~Socket();

    /// @brief Connect to endpoint.
    /// @param address (std::string).
    /// @param port (int).
    /// @return operation result (int) - `0` if connected, `-1` if there was an error.
    int connect(std::string address, int port);

    /// @brief Bind operation.
    /// @param port - port to bind.
    void bind(int port);

    /// @brief Allows to listen for connections.
    /// @param backlog (int) -  
    void listen(int backlog);
    
    /// @brief Accept new connections.
    /// @return A new socket.
    Socket accept();

    /// @brief Close the socket.
    void close();

    /// @brief Send data to endpoint.
    /// @param data (const void *) - pointer to data buffer.
    /// @param length (size_t) - length of buffer to send.
    /// @return The number of bytes accepted by the kernel for sending.
    int send(const void *data, size_t length);

    /// @brief Receive data from endpoint.
    /// @param buffer (void *) - buffer to write received data.
    /// @param length (size_t) - length to read from socket.
    /// @return The number of received bytes.
    int recv(void *buffer, size_t length);

protected:
    
    Socket(SOCKET socket, Protocol protocol);

private:
    Protocol m_protocol;
    int m_socket;
#ifdef _WIN32
    WSADATA m_wsaData;
#endif
};

#endif