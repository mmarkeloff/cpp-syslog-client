/**
 * @file      syslog_client.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_SYSLOG_CLIENT_HPP
#define __CPP_SYSLOG_CLIENT_SYSLOG_CLIENT_HPP

#include <iostream>
#include <streambuf>
#include <string>

#include "facility.hpp"
#include "level.hpp"
#include "proc_id.hpp"
#include "lock_by_mutex.hpp"
#include "sock_wrap.hpp"

/**
 * Lib space
 */
namespace syslog {
    /**
     * Syslog buffer
     */
    class streambuf final : public std::streambuf {
    private:
        std::string                   m_Buf; ///< data
        syslog::LockByMUTEX*          m_Mtx; ///< mutex
        syslog::LogLvlMng::LogLvl     m_Lvl; ///< log severity level
        syslog::SockWrap              m_Sock; ///< socket
        syslog::FacilityMng::Facility m_Facility; ///< log facility
        syslog::ProcID                m_ProcID; ///< process ID
    public:
        /**
         * Ctor
         * 
         * @warning m_Mtx is nullptr by default !!!
         */
        explicit streambuf(
        ) : 
            m_Mtx{nullptr},
            m_Lvl{syslog::LogLvlMng::LogLvl::LL_DEBUG}, 
            m_Facility{syslog::FacilityMng::Facility::LF_LOCAL7} {
        }

        /**
         * Setter
         *
         * @param[in] mtx mutex
         */
        void setMtx(syslog::LockByMUTEX& mtx) noexcept { m_Mtx = &mtx; }

        /**
         * Setter
         *
         * @param[in] lvl log severity level
         *
         * @warning By default, log severity level is syslog::LogLvlMng::LogLvl::LL_DEBUG
         */
        void setLvl(syslog::LogLvlMng::LogLvl lvl) noexcept { m_Lvl = lvl; }

        /**
         * Setter
         *
         * @param[in] sock socket
         * 
         * @warning Socket may be not initialised
         */
        void setSock(syslog::SockWrap&& sock) noexcept { m_Sock = std::move(sock); }

        /**
         * Setter
         *
         * @param[in] addr syslog server addr
         *
         * @warning By default, syslog server addr is 127.0.0.1
         */
        void setAddr(const char* addr) noexcept { m_Sock.setAddr(addr); }

        /**
         * Setter
         *
         * @param[in] port syslog server port
         *
         * @warning By default, syslog server port is 514
         */
        void setPort(uint16_t port) noexcept { m_Sock.setPort(port); }

        /**
         * Setter
         *
         * @param[in] facility log facility
         *
         * @warning By default, log facility is syslog::FacilityMng::Facility::LF_LOCAL7
         */
        void setFacility(syslog::FacilityMng::Facility facility) noexcept { m_Facility = facility; }
    protected:
        /**
         * Send data to syslog
         */
        int sync() {
            if (m_Buf.size()) {
                if (m_Sock.isInitialised()) {
                    char pri[32];
                    sprintf(
                        pri, 
                        "<%u>", 
                        (m_Facility << 3) + m_Lvl
                    );

                    std::string data{pri};
                    data += " ";
                    data += m_ProcID.getInHexFmt();
                    data += " ";
                    data += m_Buf;

                    m_Sock.send(data);
                }

                m_Buf.erase();
            }

            if (m_Mtx) // cannot be nullptr here
                m_Mtx->exit();

            return 0;
        }

        /**
         * Increment data char-by-char and send it to syslog by EOF
         *
         * @param[in] ch char
         */
        int_type overflow(int_type ch) {
            if (traits_type::eof() == ch) {
                sync(); // its time to send data to syslog
            }
            else {
                m_Buf += static_cast<char>(ch);
            }

            return ch;
        }
    };

    /**
     * Stream-designed syslog client
     */
    class ostream final : public std::ostream {
    private:
        static constexpr uint16_t          DEFAULT_SYSLOG_SRV_PORT{514}; ///< default
        static constexpr const char* const DEFAULT_SYSLOG_SRV_ADDR{"127.0.0.1"}; ///< default
    private:
        streambuf           m_LogBuf; ///< syslog buffer
        const char*         m_Addr; ///< syslog server addr
        uint16_t            m_Port; ///< syslog server port
        syslog::LockByMUTEX m_Mtx; ///< mutex
    public:
        /**
         * Ctor
         */
        explicit ostream(
        ) : 
            std::ostream{&m_LogBuf}, 
            m_Addr{DEFAULT_SYSLOG_SRV_ADDR},
            m_Port{DEFAULT_SYSLOG_SRV_PORT}
        {
            // 127.0.0.1:514 by default
            syslog::SockWrap sock{m_Addr, m_Port};

            sock.init();

            m_LogBuf.setMtx(m_Mtx);
            m_LogBuf.setSock(std::move(sock));
        }

        /**
         * Setter
         *
         * @param[in] lvl log severity level
         *
         * @warning By default, log severity level is syslog::LogLvlMng::LogLvl::LL_DEBUG
         */
        void setLvl(syslog::LogLvlMng::LogLvl lvl) noexcept { m_LogBuf.setLvl(lvl); }

        /**
         * Setter
         *
         * @param[in] addr syslog server addr
         *
         * @warning By default, syslog server addr is 127.0.0.1
         */
        void setAddr(const char* addr) noexcept { 
            m_Addr = addr;
            m_LogBuf.setAddr(addr); 
        }

        /**
         * Setter
         *
         * @param[in] port syslog server port
         *
         * @warning By default, syslog server port is 514
         */
        void setPort(uint16_t port) noexcept { 
            m_Port = port;
            m_LogBuf.setPort(port); 
        }

        /**
         * Setter
         *
         * @param[in] facility log facility
         *
         * @warning By default, log facility is syslog::FacilityMng::Facility::LF_LOCAL7
         */
        void setFacility(syslog::FacilityMng::Facility facility) noexcept { m_LogBuf.setFacility(facility); }

        /**
         * Getter
         * 
         * @return Mutex
         */
        syslog::LockByMUTEX& getMtx() noexcept { return m_Mtx; }
    };

    /**
     * Set log severity level in stream
     *
     * @param[in] os stream
     * @param[in] lvl log severity level
     */
    inline ostream& operator<<(
        ostream& os, 
        const syslog::LogLvlMng::LogLvl lvl
    ) 
    { 
        os.getMtx().enter();
        os.setLvl(lvl); 
        return os;
    }
}

#endif // __CPP_SYSLOG_CLIENT_SYSLOG_CLIENT_HPP