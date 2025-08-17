#include <gtest/gtest.h>

#include "../src/socket.h"

constexpr auto TEST_IP_ADDR = "127.0.0.1";
#ifdef _WIN32
constexpr auto TEST_PORT_1 = 8001;
constexpr auto TEST_PORT_2 = 8002;
constexpr auto TEST_PORT_3 = 8003;
constexpr auto TEST_PORT_4 = 8004;

#else
constexpr auto TEST_PORT_1 = 9001;
constexpr auto TEST_PORT_2 = 9002;
constexpr auto TEST_PORT_3 = 9003;
constexpr auto TEST_PORT_4 = 9004;

#endif

TEST(create_tcp_sock, BasicAssertions)
{
    // Create a listening socket
    Socket sock1 = Socket();
    sock1.Bind(TEST_PORT_1);
    sock1.Listen(1);
    // Connect to listening socket
    Socket sock2 = Socket();
    sock2.Connect(TEST_IP_ADDR, TEST_PORT_1);
    // Accept conneciton
    auto sock3 = sock1.Accept();
    std::string demo_str = "hello";
    // Try to send data
    int snd = sock3.Send(demo_str.data(), demo_str.size());
    // Try to receive data
    std::vector<char> recv_buffer(demo_str.size());
    sock2.Receive(recv_buffer.data(), demo_str.size());
    recv_buffer.resize(demo_str.size());
    std::string buffer_string;
    buffer_string.assign(recv_buffer.begin(), recv_buffer.end());
    // Cleanup
    sock3.Close();
    sock2.Close();
    sock1.Close();
    // Expect equality.
    EXPECT_STREQ(demo_str.data(), buffer_string.data());
    EXPECT_EQ(buffer_string.size(), demo_str.size());
}

TEST(try_bind_twice, HasCertainMessage)
{
    // Bind a socket
    Socket sock1 = Socket();
    sock1.Bind(TEST_PORT_2);
    // Bind socket with same port
    Socket sock2 = Socket();
    EXPECT_THROW(
        {
            sock2.Bind(TEST_PORT_2);
        },
        std::runtime_error);

    sock2.Close();
    sock1.Close();
}

TEST(try_connect_to_closed_peer, BasicAssertions)
{
    // Create a listening socket
    Socket sock1 = Socket();
    auto connect_result = sock1.Connect(TEST_IP_ADDR, TEST_PORT_3);
    sock1.Close();
    // Expect connection error.
    EXPECT_EQ(connect_result, -1);
}

TEST(partial_recv, BasicAssertions)
{
    // Create a listening socket
    Socket sock1 = Socket();
    sock1.Bind(TEST_PORT_4);
    sock1.Listen(1);
    // Connect to listening socket
    Socket sock2 = Socket();
    sock2.Connect(TEST_IP_ADDR, TEST_PORT_4);
    // Accept conneciton
    auto sock3 = sock1.Accept();
    std::string demo_str = "abC1deFg%H";
    // Try to send data
    int snd = sock3.Send(demo_str.c_str(), demo_str.size());
    // Try to receive data
    std::vector<uint8_t> recv_buffer(demo_str.size());
    size_t total_read = 0;
    total_read += sock2.Receive(recv_buffer.data(), 5);
    total_read += sock2.Receive(&recv_buffer.at(total_read), demo_str.size());
    recv_buffer.resize(demo_str.size());
    // Cleanup
    sock3.Close();
    sock2.Close();
    sock1.Close();
    // Expect equality.
    std::string buffer_string;
    buffer_string.assign(recv_buffer.begin(), recv_buffer.end());
    EXPECT_STREQ(buffer_string.data(), demo_str.data());
    EXPECT_EQ(recv_buffer.size(), demo_str.size());
}