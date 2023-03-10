#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else // for Unix
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <string>

class Socket {
public:
    /// @brief Enum of protocols
    enum class Protocol { TCP, UDP };

    /// @brief Constructor of `Socket`, receives a protocol type as argument.
    /// @param protocol 
    Socket(Protocol protocol = Protocol::TCP);

    ~Socket();

    /// @brief Connect to endpoint.
    /// @param address (std::string).
    /// @param port (int).
    /// @return operation result (int) - `0` if connected, `-1` if there was an error.
    int connect(std::string address, int port);

    /// @brief Close the socket.
    void close();

    /// @brief Send data to endpoint.
    /// @param data (const void *) - pointer to data buffer.
    /// @param length (size_t) - length of buffer to send.
    /// @return 
    int send(const void* data, size_t length);

    /// @brief Receive data from endpoint.
    /// @param buffer (void *) - buffer to write received data.
    /// @param length (size_t) - length to read from socket.
    /// @return 
    int recv(void* buffer, size_t length);

private:
    Protocol m_protocol;
    int m_socket;
#ifdef _WIN32
    WSADATA m_wsaData;
#endif
};

#endif