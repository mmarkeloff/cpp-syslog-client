/**
 * @file pid.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_PID_HPP
#define __CPP_SYSLOG_CLIENT_PID_HPP

#if defined(WIN32)
 #include <windows.h>
#else
 #include <unistd.h>
#endif // WIN32
#include <string.h>
#include <string>

#include "hex.hpp"

/**
 * Lib space
 */
namespace syslog {
/**
 * Details
 */
namespace details {
    /**
     * Class for handle platform specific process ID type
     */
    class pid;
};};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::details::pid final {
private:
    int32_t m_PID; ///< process ID
public:
    /**
     * Ctor
     */
    pid(
    ) : 
        m_PID{
            static_cast<int32_t>(
#if defined(WIN32)
                GetCurrentProcessId()
#else
                getpid()
#endif // WIN32
            )
        } {
    }

    /**
     * Get process ID 
     * 
     * @return int32_t
     */
    int32_t get() const noexcept { return m_PID; }

    /**
     * Get process ID in hex format
     */
    std::string hex() const noexcept { return int2hex(m_PID); }
};

#endif // __CPP_SYSLOG_CLIENT_PID_HPP