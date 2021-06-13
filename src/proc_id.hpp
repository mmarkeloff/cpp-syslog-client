/**
 * @file proc_id.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_PROC_ID_HPP
#define __CPP_SYSLOG_CLIENT_PROC_ID_HPP

#if defined(WIN32)
 #include <windows.h>
#else
 #include <unistd.h>
#endif // WIN32
#include <string.h>
#include <string>

/**
 * Lib space
 */
namespace syslog {
    /**
     * Class for handle platform specific process ID type
     */
    class ProcID;

#if defined(WIN32)
    using proc_id_t = DWORD; ///< GetCurrentProcessId() return type
#else
    using proc_id_t = pid_t; ///< getpid() return type
#endif // WIN32
};

class syslog::ProcID final {
private:
    proc_id_t m_ProcID; ///< process ID <DWORD|pid_t>
public:
    /**
     * Ctor
     */
    ProcID(
    ) : 
        m_ProcID{
#if defined(WIN32)
        GetCurrentProcessId()
#else
        getpid()
#endif // WIN32
        } {
    }

    /**
     * Get process ID 
     *
     * @return <DWORD|pid_t>
     */
    proc_id_t get() const noexcept { return m_ProcID; }

    /**
     * Get process ID in hex format (string)
     */
    std::string getInHexFmt() const noexcept {
        char res[32];
        sprintf(res, "%08x", m_ProcID);
        return res;
    }
};

#endif // __CPP_SYSLOG_CLIENT_PROC_ID_HPP