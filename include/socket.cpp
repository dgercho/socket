#include "socket.h"

constexpr auto SOCKET_ERROR = -1;

Socket::Socket(Protocol protocol): m_protocol(protocol)
#ifdef _WIN32
    , m_socket(INVALID_SOCKET)
#else
    , m_socket(0)
#endif
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed.");
    }
#endif

    int type = (protocol == Protocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    m_socket = socket(AF_INET, type, 0);

    if (m_socket == SOCKET_ERROR) {
#ifdef _WIN32
        WSACleanup();
#endif
        throw std::runtime_error("Failed to create socket.");
    }
}

Socket::~Socket() {
    close();
#ifdef _WIN32
    WSACleanup();
#endif
}

int Socket::connect(std::string address, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    return ::connect(m_socket, (sockaddr*)&addr, sizeof(addr));
}

void Socket::close() {
    if (m_socket != SOCKET_ERROR) {
#ifdef _WIN32
        ::closesocket(m_socket);
#else
        ::close(m_socket);
#endif
        m_socket = SOCKET_ERROR;
    }
}

int Socket::send(const void* data, size_t length) {
    return ::send(m_socket, (const uint8_t*)data, length, 0);
}

int Socket::recv(void* buffer, size_t length) {
    return ::recv(m_socket, (uint8_t*)buffer, length, 0);
}