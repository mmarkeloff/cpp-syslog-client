/**
 * @file tmode.hpp
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

#ifndef __CPP_SYSLOG_CLIENT_THREAD_MODE_HPP
#define __CPP_SYSLOG_CLIENT_THREAD_MODE_HPP

#include <mutex>

/**
 * Lib space
 */
namespace syslog {
/**
 * Details
 */
namespace details {
    /**
     * Class for manage single/multi thread policy
     */
    class TMode;

    /**
     * Single thread
     */
    class st;

    /**
     * Multi thread
     */
    class mt;
};};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::details::TMode {
public:
    /**
     * Dtor
     */
    virtual ~TMode() = default;

    /**
     * Lock mutex
     */
    virtual void lock() noexcept { }

    /**
     * Unlock mutex
     */
    virtual void unlock() noexcept { }

    /**
     * Lock recursive mutex
     */
    virtual void lockRec() noexcept { }

    /**
     * Unlock recursive mutex
     */
    virtual void unlockRec() noexcept { }
};

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::details::st : public syslog::details::TMode { };

////////////////////////////////////////////////////////////////////////////
///
//
class syslog::details::mt : public syslog::details::TMode { 
private:
    std::mutex           m_Mtx; ///< mutex
    std::recursive_mutex m_RecMtx; ///< recursive mutex
    std::size_t          m_OwnershipDepth; ///< for recursive mutex
public:
    /**
     * Ctor
     */
    mt() : m_OwnershipDepth{0} {}

    ~mt() { 
        m_Mtx.unlock(); 
        for (auto i = 0; i < m_OwnershipDepth; ++i)
            m_RecMtx.unlock();
    }

    void lock() noexcept override { m_Mtx.lock(); }

    void unlock() noexcept override { m_Mtx.unlock(); }

    void lockRec() noexcept override { 
        m_RecMtx.lock(); 
        ++m_OwnershipDepth;
    }

    void unlockRec() noexcept override {
        auto copied{m_OwnershipDepth};

        m_OwnershipDepth = 0;
        for (auto i = 0; i < copied; ++i)
            m_RecMtx.unlock(); 
    }
};

#endif // __CPP_SYSLOG_CLIENT_THREAD_MODE_HPP

