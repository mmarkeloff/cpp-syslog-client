/**
 * @file client_int.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_CLIENT_INT_HPP
#define __CPP_SYSLOG_CLIENT_CLIENT_INT_HPP

#include <string>

/**
 * Lib space
 */
namespace syslog {
/**
 * Details
 */
namespace details {
    /**
     * Interface for sending data
     */
    class IClient;
};};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::details::IClient {
public:
    /**
     * Dtor
     */
    virtual ~IClient() = default;

    /**
     * Setter
     *
     * @param[in] addr host IP-address
     */
    virtual void setAddr(const char* addr) noexcept = 0;

    /**
     * Setter
     *
     * @param[in] port host port
     */
    virtual void setPort(uint16_t port) noexcept = 0;

    /**
     * Getter 
     *
     * @return Socket handler
     */
    virtual int32_t getSock() const noexcept = 0;

    /**
     * Socket initialised?
     */
    virtual bool isInitialised() const noexcept = 0;

    /**
     * Send data
     *
     * @param[in] buf data
     */
    virtual void send(std::string&& buf) const noexcept = 0;
};

#endif // __CPP_SYSLOG_CLIENT_CLIENT_INT_HPP