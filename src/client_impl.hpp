/**
 * @file client_impl.hpp
 * @authors Max Markeloff (https://github.com/mmarkeloff)
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

#ifndef __CPP_SYSLOG_CLIENT_CLIENT_IMPL_HPP
#define __CPP_SYSLOG_CLIENT_CLIENT_IMPL_HPP

#if defined(WIN32)
 #include <windows.h>
#else
 #include <arpa/inet.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
#endif // WIN32
#include <string>

#if defined(WIN32)
 #include "winwsa.hpp"
#endif // WIN32
#include "client_int.hpp"

/**
 * Lib space
 */
namespace syslog {
    /**
     * Class for sending data over UDP
     */
    class UDPClient;
};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::UDPClient : public syslog::details::IClient {
private:
    static constexpr const char *const DEFAULT_ADDR{"127.0.0.1"}; ///< default
    static constexpr uint16_t          DEFAULT_PORT{514}; ///< default
    static constexpr int32_t           DEFAULT_SOCK{-1}; ///< default
private:
    uint32_t m_Addr; ///< host IP-address
    uint16_t m_Port; ///< host port
    int32_t  m_Sock; ///< socket handler
public:
    /**
     * Ctor
     */
    UDPClient(
    ) :
        m_Addr{inet_addr(DEFAULT_ADDR)}, // const char* -> uint32_t
        m_Port{DEFAULT_PORT},
        m_Sock{DEFAULT_SOCK} 
    {
#if defined(WIN32)
        details::WinWSA::instance().startup();
#endif // WIN32
        m_Sock = socket(AF_INET, SOCK_DGRAM, 0);
    }

    /**
     * Copy ctor
     */
    UDPClient(const UDPClient&) = delete;

    /**
     * Copy assignment operator
     */
    UDPClient& operator=(const UDPClient&) = delete;

    /**
     * Move ctor
     *
     * @param[in] other moving syslog::UDPClient class instance
     */
    explicit UDPClient(
        UDPClient&& other
    ) noexcept : 
        m_Addr{other.m_Addr}, 
        m_Port{other.m_Port}, 
        m_Sock{other.m_Sock} 
    {
        other.m_Sock = DEFAULT_SOCK; // uninitialise moving syslog::UDPClient class instance
    }

    /**
     * Move assigment operator
     *
     * @param[in] other moving syslog::UDPClient class instance
     */
    UDPClient& operator=(UDPClient&& other) noexcept {
        // self-assignment check
        if (&other == this)
            return *this;

        m_Addr = other.m_Addr;
        m_Port = other.m_Port;
        m_Sock = other.m_Sock;

        other.m_Sock = DEFAULT_SOCK; // uninitialise moving syslog::UDPClient class instance
        return *this;
    }

    /**
     * Dtor
     */
    ~UDPClient() {
        if (isInitialised()) {
#if defined(WIN32)
            closesocket(m_Sock);
            details::WinWSA::instance().cleanup();
#else
            close(m_Sock);
#endif // WIN32
        }
    }

    /**
     * Setter
     *
     * @param[in] addr addr
     */
    void setAddr(const char* addr) noexcept override { m_Addr = inet_addr(addr); /*const char* -> uint32_t*/ }

    /**
     * Setter
     *
     * @param[in] port port
     */
    void setPort(uint16_t port) noexcept override { m_Port = port; }

    /**
     * Getter 
     *
     * @return Socket handler
     */
    int32_t getSock() const noexcept override { return m_Sock; }

    /**
     * Socket initialised?
     */
    bool isInitialised() const noexcept override { return m_Sock != DEFAULT_SOCK; }

    /**
     * Send data
     *
     * @param[in] buf data
     */
    void send(
        std::string&& buf
    ) const noexcept override 
    { 
        auto moved{std::move(buf)};
        if (!moved.empty() && isInitialised()) {
            sockaddr_in to;
            to.sin_family = AF_INET;
            to.sin_port = htons(m_Port);
            to.sin_addr.s_addr = m_Addr;

            sendto(
                m_Sock, 
                moved.c_str(), 
                moved.size(), 
                0, 
                (sockaddr*)&to, 
                sizeof(to)
            );
        }
    }
};

#endif // __CPP_SYSLOG_CLIENT_CLIENT_IMPL_HPP


