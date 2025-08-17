#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <ws2tcpip.h>
#else  // for Unix
typedef int SOCKET;
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include <string>

class Socket {
 public:
  Socket();
  explicit Socket(SOCKET socket);
  Socket(const Socket&) = delete;
  Socket(Socket&& other) noexcept;
  Socket& operator=(Socket&& other) noexcept;
  Socket& operator=(const Socket&) = delete;
  
  // Destructor, Close the socket
  virtual ~Socket();

  /// @brief Connect to endpoint.
  /// @param address (std::string).
  /// @param port (int).
  /// @return operation result (int) - `0` if connected, `-1` if there was an
  /// error.
  int Connect(std::string address, int port);

  /// @brief Bind operation.
  /// @param port - port to bind.
  void Bind(int port);

  /// @brief Allows to listen for connections.
  /// @param backlog (int) - number of pending connections the socket will hold.
  void Listen(int backlog);

  /// @brief Accept new connections.
  /// @return A new `Socket` class.
  Socket Accept();

  /// @brief Close the socket.
  void Close();

  /// @brief Send data to endpoint.
  /// @param data (const void *) - pointer to data buffer.
  /// @param length (size_t) - length of buffer to send.
  /// @return (int) The number of bytes accepted by the kernel for sending.
  int Send(const void *data, size_t length);

  /// @brief Receive data from endpoint.
  /// @param buffer (void *) - buffer to write received data.
  /// @param length (size_t) - length to read from socket.
  /// @return (int) The number of received bytes.
  int Receive(void *buffer, size_t length);

 private:
  int m_socket;
#ifdef _WIN32
  WSADATA m_wsaData;
#endif
};

#endif