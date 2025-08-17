# Socket
Simple TCP socket wrapper class for C++. Runs on Windows, Linux and macOS.

## How to use it?
Copy the the files from `src` to your project (`socket.cpp` and `socket.h`), and include the header file.

## Example Workflow
```
    #include <src/socket.h>
    #include <string>

    int main() {

        //Create a socket
        Socket sock = Socket();
        sock.Connect("127.0.0.1", 8000);

        //Send data
        std::string data_to_send = "Hello world :S";
        sock.Send(data_to_send.data(),data_to_send.size());

        sock.Close();
        return 0;
    }
```
