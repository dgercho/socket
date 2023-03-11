#include <gtest/gtest.h>

#include "../src/socket.h"

constexpr auto TEST_IP_ADDR = "127.0.0.1";
constexpr auto TEST_PORT = 9999;

TEST(create_tcp_sock, BasicAssertions) {
  // Create a listening socket
  Socket sock1 = Socket(Socket::Protocol::TCP);
  sock1.bind(TEST_PORT);
  sock1.listen(1);
  // Connect to listening socket
  Socket sock2 = Socket(Socket::Protocol::TCP);
  sock2.connect(TEST_IP_ADDR, TEST_PORT);
  // Accept conneciton
  auto sock3 = sock1.accept();
  std::string demo_str = "hello";
  // Try to send data
  int snd = sock3.send(demo_str.c_str(), demo_str.size());
  // Try to receive data
  std::vector<char> recv_buffer(demo_str.size());
  sock2.recv(recv_buffer.data(), demo_str.size());
  // Cleanup
  sock3.close();
  sock2.close();
  sock1.close();
  // Expect equality.
  EXPECT_STREQ(demo_str.data(), recv_buffer.data());
  EXPECT_EQ(recv_buffer.size(), demo_str.size());
}

TEST(try_bind_twice, HasCertainMessage) {
  // Bind a socket
  Socket sock1 = Socket(Socket::Protocol::TCP);
  sock1.bind(TEST_PORT);
  // Bind socket with same port
  Socket sock2 = Socket(Socket::Protocol::TCP);
  EXPECT_THROW(
      {
        try {
          sock2.bind(TEST_PORT);
        } catch (const std::runtime_error& e) {
          EXPECT_STREQ("Address already in use", e.what());
          throw;
        }
      },
      std::runtime_error);

  sock2.close();
  sock1.close();
}

TEST(try_connect_to_closed_peer, BasicAssertions) {
  // Create a listening socket
  Socket sock1 = Socket(Socket::Protocol::TCP);
  auto connect_result = sock1.connect(TEST_IP_ADDR, TEST_PORT);
  // Expect connection error.
  EXPECT_EQ(connect_result, -1);
}

TEST(partial_recv, BasicAssertions) {
  // Create a listening socket
  Socket sock1 = Socket(Socket::Protocol::TCP);
  sock1.bind(TEST_PORT);
  sock1.listen(1);
  // Connect to listening socket
  Socket sock2 = Socket(Socket::Protocol::TCP);
  sock2.connect(TEST_IP_ADDR, TEST_PORT);
  // Accept conneciton
  auto sock3 = sock1.accept();
  std::string demo_str = "abC1deFg%H";
  // Try to send data
  int snd = sock3.send(demo_str.c_str(), demo_str.size());
  // Try to receive data
  std::vector<char> recv_buffer(demo_str.size());
  size_t total_read = 0;
  total_read += sock2.recv(recv_buffer.data(), 5);
  total_read += sock2.recv(recv_buffer.data() + total_read, 5);
  // Cleanup
  sock3.close();
  sock2.close();
  sock1.close();
  // Expect equality.
  EXPECT_STREQ(demo_str.data(), recv_buffer.data());
  EXPECT_EQ(recv_buffer.size(), demo_str.size());
}