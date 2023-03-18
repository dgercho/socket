#include "socket.h"

#include <cstring>
#include <stdexcept>
#include <system_error>

constexpr auto SOCKET_ERROR_CODE = -1;

Socket::Socket() :
#ifdef _WIN32
      m_socket(INVALID_SOCKET)
#else
      m_socket(0)
#endif
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
    {
        throw std::runtime_error(std::strerror(errno));
    }
#endif
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == SOCKET_ERROR_CODE)
    {
#ifdef _WIN32
        WSACleanup();
#endif
        throw std::runtime_error(std::strerror(errno));
    }
}

Socket::Socket(SOCKET socket) : m_socket(std::move(socket))
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
    {
        throw std::runtime_error(std::strerror(errno));
    }
#endif
}

int Socket::connect(std::string address, int port)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    return ::connect(m_socket, (sockaddr *)&addr, sizeof(addr));
}

void Socket::bind(int port)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(m_socket, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR_CODE)
    {
        throw std::runtime_error(std::strerror(errno));
    }
}

void Socket::listen(int backlog)
{
    if (::listen(m_socket, backlog) == SOCKET_ERROR_CODE)
    {
        throw std::runtime_error(std::strerror(errno));
    }
}

Socket Socket::accept()
{
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    SOCKET client_socket = ::accept(m_socket, (sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == SOCKET_ERROR_CODE)
    {
        throw std::runtime_error(std::strerror(errno));
    }

    return Socket(std::move(client_socket));
}

int Socket::send(const void *data, size_t length)
{
    return ::send(m_socket, reinterpret_cast<const char *>(data), length, 0);
}

int Socket::recv(void *buffer, size_t length)
{
    return ::recv(m_socket, reinterpret_cast<char *>(buffer), length, 0);
}

void Socket::close()
{
    if (m_socket != SOCKET_ERROR_CODE)
    {
        int result = 0;
#ifdef _WIN32
        result = ::closesocket(m_socket);
#else
        result = ::close(m_socket);
#endif
        m_socket = SOCKET_ERROR_CODE;
    }
}

Socket::~Socket()
{
    close();
#ifdef _WIN32
    WSACleanup();
#endif
}