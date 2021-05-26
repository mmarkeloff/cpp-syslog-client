/**
 * @file      sock_wrap.hpp
 * @authors   Max Markeloff (https://github.com/mmarkeloff)
 */

// MIT License
//
// Copyright (c) 2021 Max
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __CPP_SYSLOG_CLIENT_SOCK_WRAP_H
#define __CPP_SYSLOG_CLIENT_SOCK_WRAP_H

#if defined(WIN32)
 #include <windows.h>
#else
 #include <arpa/inet.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
#endif // WIN32

#if defined(WIN32)
 #define CLOSE_SOCKET_CALL closesocket ///< int closesocket(SOCKET s);
#else
 #define CLOSE_SOCKET_CALL close ///< int close(int fd);
#endif // WIN32

#include <string>

/**
 * Lib space
 */
namespace syslog {
    /**
     * Simple cross platform data sender
     */
    class SockWrap;
};

class syslog::SockWrap final {
private:
    static constexpr int32_t DEFAULT_SOCK_VALUE{-1}; ///< default
private:
    uint32_t m_Addr; ///< addr
    uint16_t m_Port; ///< port
    int32_t  m_Sock; ///< socket handler
public:
    /**
     * Ctor by default
     *
     * @warning See syslog::streambuf
     */
    explicit SockWrap() = default;

    /**
     * Ctor
     *
     * @param[in] addr addr
     * @param[in] port port
     */
    explicit SockWrap(
        const char* addr, 
        uint16_t port
    ) :
        m_Addr{inet_addr(addr)}, // const char* -> uint32_t
        m_Port{port},
        m_Sock{DEFAULT_SOCK_VALUE} {
    }

    /**
     * Copy ctor
     */
    SockWrap(const SockWrap&) = delete;

    /**
     * Copy assignment operator
     */
    SockWrap& operator=(const SockWrap&) = delete;

    /**
     * Move ctor
     *
     * @param[in] sock moving syslog::SockWrap class instance
     */
    explicit SockWrap(
        SockWrap&& sock
    ) : 
        m_Addr{sock.m_Addr}, 
        m_Port{sock.m_Port}, 
        m_Sock{sock.m_Sock} 
    {
        sock.m_Sock = DEFAULT_SOCK_VALUE; // uninitialise moving syslog::SockWrap class instance
    }

    /**
     * Move assigment operator
     *
     * @param[in] sock moving syslog::SockWrap class instance
     */
    SockWrap& operator=(SockWrap&& sock) {
        // self-assignment check
        if (&sock == this)
            return *this;

        m_Addr = sock.m_Addr;
        m_Port = sock.m_Port;
        m_Sock = sock.m_Sock;

        sock.m_Sock = DEFAULT_SOCK_VALUE; // uninitialise moving syslog::SockWrap class instance
        return *this;
    }

    /**
     * Dtor
     */
    ~SockWrap() {
        if (isInitialised())
            CLOSE_SOCKET_CALL(m_Sock);
    }

    /**
     * Init socket
     */
    void init() noexcept {
#if defined(WIN32)
        WSADATA data;
        WSAStartup(MAKEWORD(1, 1), &data);
#endif // WIN32

        if (!isInitialised())
            m_Sock = socket(AF_INET, SOCK_DGRAM, 0);
    }

    /**
     * Socket initialised?
     */
    bool isInitialised() const noexcept { return m_Sock != DEFAULT_SOCK_VALUE; }

    /**
     * Send data
     *
     * @param[in] buf data
     */
    void send(const std::string& buf) noexcept {
        if (isInitialised()) {
            sockaddr_in to;
            to.sin_family = AF_INET;
            to.sin_port = htons(m_Port);
            to.sin_addr.s_addr = m_Addr;

            sendto(
                m_Sock, 
                buf.c_str(), 
                buf.size(), 
                0, 
                (sockaddr*)&to, 
                sizeof(to)
            );
        }
    }

    /**
     * Setter
     *
     * @param[in] addr addr
     */
    void setAddr(const char* addr) noexcept { m_Addr = inet_addr(addr); /*const char* -> uint32_t*/ }

    /**
     * Setter
     *
     * @param[in] port port
     */
    void setPort(uint16_t port) noexcept { m_Port = port; }

    /**
     * Getter 
     *
     * @return Socket handler
     */
    int32_t getSock() const noexcept { return m_Sock; }

};

#endif // __CPP_SYSLOG_CLIENT_SOCK_WRAP_H


