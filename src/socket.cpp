#include "socket.h"

#include <cstring>
#include "exceptions.h"

int GetSocketError() {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

Socket::Socket()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
    {
        throw SocketCreateException(std::to_string(GetSocketError()));
    }
#endif
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
#ifdef _WIN32
        WSACleanup();
#endif
        throw SocketCreateException(std::to_string(GetSocketError()));
    }
}

Socket::Socket(SOCKET socket) : m_socket(socket) {}

Socket::Socket(Socket&& other) noexcept : m_socket(other.m_socket) 
{
    other.m_socket = INVALID_SOCKET;

}

Socket& Socket::operator=(Socket&& other) noexcept {
    m_socket = other.m_socket;
    other.m_socket = INVALID_SOCKET;
#ifdef _WIN32
    m_wsaData = other.m_wsaData;
#endif
    return *this;
}

int Socket::Connect(const std::string& address, int port)
{
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    auto response = inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
    if (response != 1) {
        throw InetPtonException(std::to_string(GetSocketError()));
    }
    response = ::connect(m_socket, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
    if (response != 0) {
        throw SocketConnectException(std::to_string(GetSocketError()));
    }
    return response;
}

void Socket::Bind(const std::string& address, int port)
{
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    auto response = inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
    if (response != 1) {
        throw InetPtonException(std::to_string(GetSocketError()));
    }
    if (::bind(m_socket, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) == INVALID_SOCKET)
    {
        throw SocketBindException(std::to_string(GetSocketError()));
    }
}

void Socket::Listen(int backlog)
{
    if (::listen(m_socket, backlog) != 0)
    {
        throw SocketListenException(std::to_string(GetSocketError()));
    }
}

Socket Socket::Accept()
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_size = sizeof(client_addr);
    SOCKET client_socket = ::accept(m_socket, reinterpret_cast<struct sockaddr *>(&client_addr), &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        throw SocketAcceptException(std::to_string(GetSocketError()));
    }

    return Socket(std::move(client_socket));
}

int Socket::Send(const void *data, size_t length)
{
    return ::send(m_socket, reinterpret_cast<const char *>(data), length, 0);
}

int Socket::Receive(void *buffer, size_t length)
{
    return ::recv(m_socket, reinterpret_cast<char *>(buffer), length, 0);
}

void Socket::Close() noexcept
{
    if (m_socket != INVALID_SOCKET)
    {
        int result = 0;
#ifdef _WIN32
        result = ::closesocket(m_socket);
        WSACleanup();
#else
        result = ::close(m_socket);
#endif
        m_socket = INVALID_SOCKET;
    }
}

Socket::~Socket()
{
    Close();
}