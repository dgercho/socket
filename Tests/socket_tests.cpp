#include <gtest/gtest.h>
#include "../src/socket.h"

constexpr auto TEST_IP_ADDR = "127.0.0.1";
constexpr auto TEST_PORT = 9999;

TEST(create_tcp_sock, BasicAssertions)
{
  //Create a listening socket
  Socket sock1 = Socket(Socket::Protocol::TCP);
  sock1.bind(TEST_PORT);
  sock1.listen(1);
  //Connect to listening socket
  Socket sock2 = Socket(Socket::Protocol::TCP);
  sock2.connect(TEST_IP_ADDR,TEST_PORT);
  //Accept conneciton
  auto sock3 = sock1.accept();
  std::string demo_str = "hello";
  //Try to send data
  int snd = sock3.send(demo_str.c_str(), demo_str.size());
  //Try to receive data
  std::vector<char> recv_buffer(demo_str.size());
  sock2.recv(recv_buffer.data(), demo_str.size());
  //Cleanup
  sock3.close();
  sock2.close();
  sock1.close();
  // Expect equality.
  EXPECT_EQ(recv_buffer.size(), demo_str.size());
}