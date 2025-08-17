#pragma once

#include <stdexcept>
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

#define DECLARE_EXCEPTION(exception_name, exception_value)              \
    class exception_name final: public SocketException {                \
    public:                                                             \
        exception_name(): SocketException(std::move(exception_value)) { \
                                                                        \
        }                                                               \
    };


DECLARE_EXCEPTION(SocketCreateException, "Error creating socket")