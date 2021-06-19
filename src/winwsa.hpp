/**
 * @file winwsa.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_WINWSA_HPP
#define __CPP_SYSLOG_CLIENT_WINWSA_HPP

#if defined(WIN32)
#include <windows.h>

#include "crtp_st.hpp"

/**
 * Lib space
 */
namespace syslog {
/**
 * Details
 */
namespace details {
    /**
     * Class for handling WSAStartup()/WSACleanup()
     * 
     * @warning Singleton
     */
    class WinWSA;
};};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::details::WinWSA final : public crtp_st::Base<WinWSA> {
private:
    WSADATA m_WSAData;
public:
    WinWSA() { WSAStartup(MAKEWORD(1, 1), &m_WSAData); }

    ~WinWSA() { WSACleanup(); }
};

#endif // WIN32
#else
#error Wrong OS
#endif // __CPP_SYSLOG_CLIENT_WINWSA_HPP