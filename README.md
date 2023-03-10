# Socket
Simple socket wrapper class for CPP. Working in Unix and Windows.

[![CMake CI (Unix and Windows)](https://github.com/dgercho/socket/blob/dev/.github/workflows/CMake.yml/badge.svg)](hhttps://github.com/dgercho/socket/blob/dev/.github/workflows/CMake.yml)

## How to use it?
Copy the the files from `src` to your project (`socket.cpp` and `socket.h`), and include the header file.

## Example Workflow
```
    #include <src/socket.h>
    #include <string>

    int main() {

        //Create a socket
        Socket sock = Socket(Socket::Protocol::TCP);
        sock.connect("127.0.0.1", 8000);

        //Send data
        std::string data_to_send = "Hello world :S";
        sock.send(data_to_send.data(),data_to_send.size());

        sock.close();
        return 0;
    }
```
