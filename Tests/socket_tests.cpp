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
    sock1.bind(TEST_PORT_1);
    sock1.listen(1);
    // Connect to listening socket
    Socket sock2 = Socket();
    sock2.connect(TEST_IP_ADDR, TEST_PORT_1);
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

TEST(try_bind_twice, HasCertainMessage)
{
    // Bind a socket
    Socket sock1 = Socket();
    sock1.bind(TEST_PORT_2);
    // Bind socket with same port
    Socket sock2 = Socket();
    EXPECT_THROW(
        {
            sock2.bind(TEST_PORT_2);
        },
        std::runtime_error);

    sock2.close();
    sock1.close();
}

TEST(try_connect_to_closed_peer, BasicAssertions)
{
    // Create a listening socket
    Socket sock1 = Socket();
    auto connect_result = sock1.connect(TEST_IP_ADDR, TEST_PORT_3);
    sock1.close();
    // Expect connection error.
    EXPECT_EQ(connect_result, -1);
}

TEST(partial_recv, BasicAssertions)
{
    // Create a listening socket
    Socket sock1 = Socket();
    sock1.bind(TEST_PORT_4);
    sock1.listen(1);
    // Connect to listening socket
    Socket sock2 = Socket();
    sock2.connect(TEST_IP_ADDR, TEST_PORT_4);
    // Accept conneciton
    auto sock3 = sock1.accept();
    std::string demo_str = "abC1deFg%H";
    // Try to send data
    int snd = sock3.send(demo_str.c_str(), demo_str.size());
    // Try to receive data
    std::vector<char> recv_buffer(demo_str.size());
    size_t total_read = 0;
    total_read += sock2.recv(recv_buffer.data(), 5);
    total_read += sock2.recv(&recv_buffer.at(total_read), demo_str.size());
    recv_buffer.resize(demo_str.size());
    // Cleanup
    sock3.close();
    sock2.close();
    sock1.close();
    // Expect equality.
    EXPECT_STREQ(demo_str.data(), recv_buffer.data());
    EXPECT_EQ(recv_buffer.size(), demo_str.size());
}