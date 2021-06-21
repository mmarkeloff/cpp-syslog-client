/**
 * @file ostream.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_OSTREAM_HPP
#define __CPP_SYSLOG_CLIENT_OSTREAM_HPP

#include <iostream>
#include <memory>

#include "level.hpp"
#include "facility.hpp"
#include "client_int.hpp"
#include "tmode.hpp"
#include "streambuf.hpp"

/**
 * Lib space
 */
namespace syslog {
    /**
     * Stream-designed syslog client
     */
    class ostream;
};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::ostream final : public std::ostream {
private:
    details::streambuf m_Buf; ///< stream buffer
public:
    /**
     * Ctor
     * 
     * @param[in] mode <single|multi> thread
     * @param[in] clnt data sender
     */
    explicit ostream(
        std::unique_ptr<details::IClient>&& clnt,
        std::unique_ptr<details::TMode>&& mode
    ) : 
        m_Buf{std::move(clnt), std::move(mode)},
        std::ostream{&m_Buf} {
    }

    /**
     * Copy ctor
     */
    ostream(const ostream&) = delete;

    /**
     * Copy assignment operator
     */
    ostream &operator=(const ostream&) = delete;

    /**
     * Move ctor
     */
    ostream(
        ostream&& other
    ) noexcept : 
        m_Buf{std::move(other.m_Buf)},
        std::ostream{&m_Buf} {
    }

    /**
     * Move assignment operator
     */
    ostream& operator=(ostream&& other) noexcept {
        // self-assignment check
        if (&other == this)
            return *this;

        m_Buf = std::move(other.m_Buf);
        return *this;
    }

    /**
     * Setter
     *
     * @param[in] addr syslog server IP-address
     *
     * @warning By default, syslog server IP-address is 127.0.0.1
     */
    void setAddr(const char* addr) noexcept { m_Buf.setAddr(addr); }

    /**
     * Setter
     *
     * @param[in] port syslog server port
     *
     * @warning By default, syslog server port is 514
     */
    void setPort(uint16_t port) noexcept { m_Buf.setPort(port); }

    /**
     * Setter
     *
     * @param[in] facility log facility
     *
     * @warning By default, log facility is syslog::LogFacilityMng::LogFacility::LF_LOCAL7
     */
    void setFacility(LogFacilityMng::LogFacility facility) noexcept { m_Buf.setFacility(facility); }

    /**
     * Setter
     *
     * @param[in] lvl log severity level
     *
     * @warning By default, log severity level is syslog::LogLvlMng::LogLvl::LL_DEBUG
     */
    void setLvl(LogLvlMng::LogLvl lvl) noexcept { m_Buf.setLvl(lvl); }
};

/**
 * Lib space
 */
namespace syslog {
    /**
     * Set log severity level in stream
     *
     * @param[in] os stream
     * @param[in] lvl log severity level
     */
    inline ostream &operator<<(
        ostream &os,
        LogLvlMng::LogLvl lvl)
    {
        os.setLvl(lvl);
        return os;
    }
};

#endif // __CPP_SYSLOG_CLIENT_OSTREAM_HPP
