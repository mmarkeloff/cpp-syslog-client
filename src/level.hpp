/**
 * @file level.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_LEVEL_HPP
#define __CPP_SYSLOG_CLIENT_LEVEL_HPP

/**
 * Lib space
 */
namespace syslog {
    /**
     * Class for manage log severity level
     */
    class LogLvlMng;
};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::LogLvlMng final {
public:
    /**
     * Available log severity level
     * 
     * @link https://datatracker.ietf.org/doc/html/rfc5424#section-6.2.1
     */
    enum LogLvl {
        LL_EMERG = 0, ///< EMERG
        LL_ALERT, ///< ALERT
        LL_CRIT, ///< CRIT
        LL_ERR, ///< ERR
        LL_WARNING, ///< WARNING
        LL_NOTICE, ///< NOTICE
        LL_INFO, ///< INFO
        LL_DEBUG ///< DEBUG
    };
};

#endif // __CPP_SYSLOG_CLIENT_LEVEL_HPP
