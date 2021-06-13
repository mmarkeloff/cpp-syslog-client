/**
 * @file syslog_client.cpp
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

#include <sstream>
#include <fstream>

#include <gtest/gtest.h>

#include <syslog_client.hpp>

using namespace syslog;

////////////////////////////////////////////////////////////////////////////
///
//
class TestSyslogClient : public ::testing::Test {
protected:
    static constexpr const char* const logPath{"/var/log/syslog"};
protected:
    void SetUp() { }

    void TearDown() { }

    std::string tail(std::ifstream& in) {
        std::string res;
        while (in >> std::ws && std::getline(in, res));
        return res;
    }
};

#include <thread>
#include <chrono>

////////////////////////////////////////////////////////////////////////////
///
//
TEST_F(TestSyslogClient, sendDebugMessage) {
    ostream syslog;

    syslog << LogLvlMng::LL_DEBUG << "Debug test message" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos == tail(log).find("Debug test message"));
}

TEST_F(TestSyslogClient, sendInfoMessage) {
    ostream syslog;

    syslog << LogLvlMng::LL_INFO << "Info test message" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Info test message"));
}

TEST_F(TestSyslogClient, sendNoticeMessageSyslogFacility) {
    ostream syslog;
    syslog.setFacility(FacilityMng::LF_SYSLOG);

    syslog << LogLvlMng::LL_NOTICE << "Notice syslog facility test message" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Notice syslog facility test message"));
}

TEST_F(TestSyslogClient, sendCompositeMessage) {
    ostream syslog;

    syslog << LogLvlMng::LL_NOTICE;
    syslog << "Notice";
    syslog << " ";
    syslog << "test message";
    syslog << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Notice test message"));
}

#include <vector>

TEST_F(TestSyslogClient, sendErrorMessagesByMultiplyThreads) {
    auto f = [](ostream& syslog) {
        std::ostringstream ss;
        ss << std::this_thread::get_id();

        syslog << LogLvlMng::LL_ERR << "Error test message from thread: " << ss.str() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    ostream syslog;
    std::vector<std::thread> threads;

    for (std::size_t i = 0; i < 16; i++)
        threads.push_back(std::thread{f, std::ref(syslog)});

    for (auto& thread : threads) 
        thread.join();
}

TEST_F(TestSyslogClient, eachThreadHasHisOwnSyslogClientInstance) {
    auto f = []() {
        std::ostringstream ss;
        ss << std::this_thread::get_id();

        ostream syslog;
        syslog << LogLvlMng::LL_WARNING << "Test message from thread: " << ss.str() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    std::vector<std::thread> threads;

    for (std::size_t i = 0; i < 16; i++)
        threads.push_back(std::thread{f});

    for (auto& thread : threads) 
        thread.join();
}

TEST_F(TestSyslogClient, sendMessagesWithSavedLogLvlByMultiplyThreads) {
    auto f = [](ostream& syslog) {
        for (auto i = 0; i < 128; ++i) {
            syslog << i;
            syslog << " test message from thread: ";
            syslog << std::this_thread::get_id();
            syslog << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    };

    ostream syslog;
    syslog << LogLvlMng::LL_WARNING;

    std::vector<std::thread> threads;

    for (std::size_t i = 0; i < 16; i++)
        threads.push_back(std::thread{f, std::ref(syslog)});

    for (auto& thread : threads) 
        thread.join();
}