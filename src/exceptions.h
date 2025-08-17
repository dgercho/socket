#pragma once

#include <exception>
#include <string>

class SocketException: public std::exception {
public:
    SocketException(std::string&& value) : m_value(std::move(value)) {
    }

    const char* what() const noexcept override {
        return m_value.c_str();
    }
private:
    std::string m_value;
};

#define DECLARE_EXCEPTION(exception_name, exception_value)                                                  \
    class exception_name final: public SocketException {                                                    \
    public:                                                                                                 \
        exception_name(std::string&& value): SocketException(std::string(exception_value) + ": " + value) { \
                                                                                                            \
        }                                                                                                   \
    };


DECLARE_EXCEPTION(SocketCreateException, "Error creating socket")
DECLARE_EXCEPTION(SocketBindException, "Socket bind error")
DECLARE_EXCEPTION(SocketListenException, "Socket listen error")
DECLARE_EXCEPTION(SocketAcceptException, "Socket accept error")