/**
 * @file streambuf.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_STREAMBUF_HPP
#define __CPP_SYSLOG_CLIENT_STREAMBUF_HPP

#include <streambuf>
#include <string>
#include <memory>

#include "level.hpp"
#include "facility.hpp"
#include "pid.hpp"
#include "client_int.hpp"
#include "tmode.hpp"

/**
 * Lib space
 */
namespace syslog {
/**
 * Details
 */
namespace details {
    /**
     * Stream buffer
     */
    class streambuf;
};};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::details::streambuf final : public std::streambuf {
private:
    std::string                       m_Buf; ///< data to send
    LogLvlMng::LogLvl                 m_Lvl; ///< log severity level
    LogFacilityMng::LogFacility       m_Facility; ///< log facility
    details::pid                      m_PID; ///< process ID
    std::unique_ptr<details::IClient> m_Clnt; ///< data sender
    std::unique_ptr<details::TMode>   m_Mode; ///< <single|multi> thread
public:
    /**
     * Ctor
     */
    streambuf(
        std::unique_ptr<details::IClient>&& clnt,
        std::unique_ptr<details::TMode>&& mode
    ) : 
        m_Lvl{LogLvlMng::LogLvl::LL_DEBUG},
        m_Facility{LogFacilityMng::LogFacility::LF_LOCAL7},
        m_Clnt{std::move(clnt)},
        m_Mode{std::move(mode)} {
    }

    /**
     * Copy ctor
     */
    streambuf(const streambuf&) = delete;

    /**
     * Copy assignment operator
     */
    streambuf &operator=(const streambuf&) = delete;

    /**
     * Move ctor
     */
    streambuf(
        streambuf&& other
    ) noexcept :
        m_Buf{std::move(other.m_Buf)},
        m_Lvl{other.m_Lvl},
        m_Facility{other.m_Facility},
        m_PID{other.m_PID},
        m_Clnt{std::move(other.m_Clnt)},
        m_Mode{std::move(other.m_Mode)} {
    }

    /**
     * Move assignment operator
     */
    streambuf& operator=(streambuf&& other) noexcept {
        // self-assignment check
        if (&other == this)
            return *this;

        m_Buf = std::move(other.m_Buf);
        m_Lvl = other.m_Lvl;
        m_Facility = other.m_Facility;
        m_PID = other.m_PID;
        m_Clnt = std::move(other.m_Clnt);
        m_Mode = std::move(other.m_Mode);

        return *this;
    }

    /**
     * Setter
     *
     * @param[in] lvl log severity level
     *
     * @warning By default, log severity level is syslog::LogLvlMng::LogLvl::LL_DEBUG
     * @warning Lock zone
     */
    void setLvl(LogLvlMng::LogLvl lvl) noexcept { 
        m_Mode->lock();
        m_Lvl = lvl; 
        m_Mode->unlock();
    }

    /**
     * Setter
     *
     * @param[in] facility log facility
     *
     * @warning By default, log facility is syslog::LogFacilityMng::LogFacility::LF_LOCAL7
     * @warning Lock zone
     */
    void setFacility(LogFacilityMng::LogFacility facility) noexcept { 
        m_Mode->lock();
        m_Facility = facility; 
        m_Mode->unlock();
    }

    /**
     * Setter
     *
     * @param[in] addr syslog server IP-address
     *
     * @warning By default, syslog server IP-address is 127.0.0.1
     * @warning Lock zone
     */
    void setAddr(const char *addr) noexcept { 
        m_Mode->lock();
        m_Clnt->setAddr(addr);
        m_Mode->unlock(); 
    }

    /**
     * Setter
     *
     * @param[in] port syslog server port
     *
     * @warning By default, syslog server port is 514
     * @warning Lock zone
     */
    void setPort(uint16_t port) noexcept { 
        m_Mode->lock();
        m_Clnt->setPort(port); 
        m_Mode->unlock(); 
    }
protected:
    /**
     * Send data to syslog server
     * 
     * @warning Unlock recursive mutex
     */
    int sync() override {
        if (!m_Buf.empty()) {
            m_Mode->lock();
            auto ready{m_Clnt->isInitialised()};
            m_Mode->unlock();

            if (ready) {
                m_Mode->lock();
                // https://datatracker.ietf.org/doc/html/rfc5424#section-6.2.1
                auto pri{"<" + std::to_string((m_Facility << 3) + m_Lvl) + ">"};
                m_Mode->unlock();

                std::string data{pri};
                data += " ";
                data += m_PID.hex();
                data += " ";
                data += m_Buf;

                m_Mode->lock();
                m_Clnt->send(std::move(data));
                m_Mode->unlock();
            }

            m_Buf.erase();
            m_Mode->unlockRec();
        }

        return 0;
    }

    /**
     * Increment data char-by-char and send it to syslog server by EOF
     *
     * @param[in] ch char
     * 
     * @warning On each char, capturing a recursive mutex
     */
    int_type overflow(
        int_type ch
    ) override 
    {
        if (traits_type::eof() == ch) {
            sync(); // its time to send data to syslog server
        }
        else {
            m_Mode->lockRec();
            m_Buf += static_cast<char>(ch);
        }

        return ch;
    }
};

#endif // __CPP_SYSLOG_CLIENT_STREAMBUF_HPP