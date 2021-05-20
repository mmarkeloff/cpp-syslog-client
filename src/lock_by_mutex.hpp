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

#ifndef __CPP_SYSLOG_CLIENT_LOCK_BY_MUTEX_HPP
#define __CPP_SYSLOG_CLIENT_LOCK_BY_MUTEX_HPP

#include <mutex>
#include <atomic>

/**
 * Lib space
 */
namespace syslog {
    /**
     * Locking by mutex implementation
     */
    class LockByMUTEX;
};

class syslog::LockByMUTEX final {
private:
    std::mutex       m_Mtx; ///< mutex object
    std::atomic_bool m_isEntered; ///< mutex is already captured?
public:
    /**
     * Ctor
     */
    explicit LockByMUTEX() : m_isEntered{false} {}

    /**
     * Dtor
     */
    ~LockByMUTEX() { m_Mtx.unlock(); /*just for security*/ }

    /**
     * Waiting here for another thread
     */
    bool enter() noexcept { 
        m_Mtx.lock(); 
        m_isEntered = true;
        return m_isEntered;
    }

    /**
     * Release the mutex
     */
    void exit() noexcept { 
        m_isEntered = false;
        m_Mtx.unlock(); 
    }

    /**
     * Mutex is already captured?
     */
    bool isEntered() const noexcept { return m_isEntered; }
};

#endif // __CPP_SYSLOG_CLIENT_LOCK_BY_MUTEX_HPP