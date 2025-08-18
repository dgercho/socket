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
    ZeroMemory(&other.m_wsaData, sizeof(other.m_wsaData));
#endif
    return *this;
}

int Socket::Connect(std::string address, int port)
{
    sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    return ::connect(m_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
}

void Socket::Bind(int port)
{
    sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(m_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == INVALID_SOCKET)
    {
        throw SocketBindException(std::to_string(GetSocketError()));
    }
}

void Socket::Listen(int backlog)
{
    if (::listen(m_socket, backlog) == INVALID_SOCKET)
    {
        throw SocketListenException(std::to_string(GetSocketError()));
    }
}

Socket Socket::Accept()
{
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    SOCKET client_socket = ::accept(m_socket, (sockaddr *)&client_addr, &client_addr_size);
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