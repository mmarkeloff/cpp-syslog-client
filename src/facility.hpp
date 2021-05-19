/**
 * @author Max Markeloff (https://github.com/mmarkeloff)
 * 
 * MIT License
 *
 * Copyright (c) 2021 Max
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __CPP_SYSLOG_CLIENT_FACILITY_HPP
#define __CPP_SYSLOG_CLIENT_FACILITY_HPP

#include <map>
#include <algorithm>

#define STR2FACILITY_ITEM const char*, syslog::FacilityMng::Facility
#define FACILITY2STR_ITEM syslog::FacilityMng::Facility, const char*

/**
 * Lib space
 */
namespace syslog {
    /**
     * Class for manage log facility
     */
    class FacilityMng;
};

class syslog::FacilityMng {
public:
    /**
     * Available log facility
     */
    enum Facility {
        LF_KERN = 0, ///< KERN
        LF_USER, ///< USER
        LF_MAIL, ///< MAIL
        LF_DAEMON, ///< DAEMON
        LF_AUTH, ///< AUTH
        LF_SYSLOG, ///< SYSLOG
        LF_LPR, ///< LPR
        LF_NEWS, ///< NEWS
        LF_UUCP, ///< UUCP
        LF_CRON, ///< CRON
        LF_AUTHPRIV, ///< AUTHPRIV
        LF_FTP, ///< FTP
        LF_NTP, ///< NTP
        LF_AUDIT, ///< AUDIT
        LF_ALERT, ///< ALERT
        LF_CRON2, ///< CRON2
        LF_LOCAL0, ///< LOCAL0
        LF_LOCAL1, ///< LOCAL1
        LF_LOCAL2, ///< LOCAL2
        LF_LOCAL3, ///< LOCAL3
        LF_LOCAL4, ///< LOCAL4
        LF_LOCAL5, ///< LOCAL5
        LF_LOCAL6, ///< LOCAL6
        LF_LOCAL7 ///< LOCAL7
    };
public:
    /**
     * Dtor
     */ 
    virtual ~FacilityMng() = default;
};

#endif // __CPP_SYSLOG_CLIENT_FACILITY_HPP