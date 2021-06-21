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

#include "syslog_client.hpp"

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

////////////////////////////////////////////////////////////////////////////
///
//
class ModuleNameFormatter : public IFormatter {
private:
    std::string m_ModuleName;
public:
    ModuleNameFormatter(std::string&& module_name) : m_ModuleName{std::move(module_name)} {}

    std::string key() const noexcept override { return "module"; }

    std::string value() const noexcept override { return m_ModuleName; }
};

#include <thread>
#include <chrono>

////////////////////////////////////////////////////////////////////////////
///
//
TEST_F(TestSyslogClient, sendDebugMsgOverUDP_st) {
    auto syslog{makeUDPClient_st()};

    syslog << LogLvlMng::LL_DEBUG << "Debug test message (st)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos == tail(log).find("Debug test message (st)"));
}

TEST_F(TestSyslogClient, sendDebugMsgOverUDP_mt) {
    auto syslog{makeUDPClient_mt()};

    syslog << LogLvlMng::LL_DEBUG << "Debug test message (mt)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos == tail(log).find("Debug test message (mt)"));
}

TEST_F(TestSyslogClient, sendInfoMsgOverUDP_st) {
    auto syslog{makeUDPClient_st()};

    syslog << LogLvlMng::LL_INFO << "Info test message (st)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Info test message (st)"));
}

TEST_F(TestSyslogClient, sendInfoMsgOverUDP_mt) {
    auto syslog{makeUDPClient_mt()};

    syslog << LogLvlMng::LL_INFO << "Info test message (mt)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Info test message (mt)"));
}

TEST_F(TestSyslogClient, sendNoticeMsgOverUDPSyslogFacility_st) {
    auto syslog{makeUDPClient_st()};
    syslog.setFacility(LogFacilityMng::LF_SYSLOG);

    syslog << LogLvlMng::LL_NOTICE << "Notice syslog facility test message (st)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Notice syslog facility test message (st)"));
}

TEST_F(TestSyslogClient, sendNoticeMsgOverUDPSyslogFacility_mt) {
    auto syslog{makeUDPClient_mt()};
    syslog.setFacility(LogFacilityMng::LF_SYSLOG);

    syslog << LogLvlMng::LL_NOTICE << "Notice syslog facility test message (mt)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Notice syslog facility test message (mt)"));
}

TEST_F(TestSyslogClient, sendCompositeMsgOverUDP_st) {
    auto syslog{makeUDPClient_st()};

    syslog << LogLvlMng::LL_NOTICE;
    syslog << "Composite";
    syslog << " ";
    syslog << "test message";
    syslog << " ";
    syslog << "(st)";
    syslog << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Composite test message (st)"));
}

TEST_F(TestSyslogClient, sendCompositeMsgOverUDP_mt) {
    auto syslog{makeUDPClient_mt()};

    syslog << LogLvlMng::LL_NOTICE;
    syslog << "Composite";
    syslog << " ";
    syslog << "test message";
    syslog << " ";
    syslog << "(mt)";
    syslog << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Composite test message (mt)"));
}

TEST_F(TestSyslogClient, emptyMsgOverUDP_st) {
    auto syslog{makeUDPClient_st()};
    syslog << std::endl;
}

TEST_F(TestSyslogClient, emptyMsgOverUDP_mt) {
    auto syslog{makeUDPClient_mt()};
    syslog << std::endl;
}

TEST_F(TestSyslogClient, emptyMsgWithSettingLogLvl_st) {
    auto syslog{makeUDPClient_st()};
    syslog << LogLvlMng::LL_EMERG;
    syslog << std::endl;
}

TEST_F(TestSyslogClient, emptyMsgWithSettingLogLvl_mt) {
    auto syslog{makeUDPClient_mt()};
    syslog << LogLvlMng::LL_ERR;
    syslog << std::endl;
}

#include <vector>

TEST_F(TestSyslogClient, sendErrorMsgs_mt) {
    auto syslog{makeUDPClient_mt()};

    auto sendMsg = [&]() {
        syslog << LogLvlMng::LL_ERR << "Test message from thread: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    std::vector<std::thread> threads;
    for (auto i = 0; i < 16; i++)
        threads.push_back(std::thread{sendMsg});

    for (auto& thread : threads) 
        thread.join();
}

TEST_F(TestSyslogClient, eachThreadHasHisOwnSyslogClientInst_st) {
    auto sendMsg = []() {
        auto syslog{makeUDPClient_st()};
        syslog << LogLvlMng::LL_WARNING << "Test message from thread: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    std::vector<std::thread> threads;
    for (auto i = 0; i < 16; i++)
        threads.push_back(std::thread{sendMsg});

    for (auto& thread : threads) 
        thread.join();
}

TEST_F(TestSyslogClient, eachThreadHasHisOwnSyslogClientInst_mt) {
    auto sendMsg = []() {
        auto syslog{makeUDPClient_mt()};
        syslog << LogLvlMng::LL_CRIT << "Test message from thread: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    std::vector<std::thread> threads;
    for (auto i = 0; i < 16; i++)
        threads.push_back(std::thread{sendMsg});

    for (auto& thread : threads) 
        thread.join();
}

TEST_F(TestSyslogClient, sendMsgsByLogLvlWasSettedInMainThread_mt) {
    auto syslog{makeUDPClient_mt()};
    syslog << LogLvlMng::LL_WARNING;

    auto sendMsg = [&]() {
        for (auto i = 0; i < 64; ++i) {
            // syslog::LogLvlMng::LL_WARNING
            syslog << i;
            syslog << " test message from thread: ";
            syslog << std::this_thread::get_id();
            syslog << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    };

    std::vector<std::thread> threads;
    for (auto i = 0; i < 16; i++)
        threads.push_back(std::thread{sendMsg});

    for (auto& thread : threads) 
        thread.join();
}

TEST_F(TestSyslogClient, sendMsgsByCallingSetters_mt) {
    auto syslog{makeUDPClient_mt()};

    auto callSettersAndSendMsg = [&](std::size_t id) {
        for (auto i = 0; i < 32; ++i) {
            syslog.setAddr("127.0.0.1"); 
            syslog.setPort(514); 

            if (0 == (id & 1)) {
                syslog.setLvl(LogLvlMng::LL_ERR); 
                syslog.setFacility(LogFacilityMng::LF_DAEMON);
            }
            else {
                syslog.setLvl(LogLvlMng::LL_INFO);
                syslog.setFacility(LogFacilityMng::LF_AUTH);
            }

            syslog << "Test message" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    };

    std::vector<std::thread> threads;
    for (auto i = 0; i < 16; ++i)
        threads.push_back(std::thread{callSettersAndSendMsg, i});

    for (auto& thread : threads) 
        thread.join();
}

TEST_F(TestSyslogClient, sendMsgsByCallingAnotherSetters_mt) {
    auto syslog{makeUDPClient_mt()};

    auto callSettersAndSendMsg = [&](std::size_t id) {
        for (auto i = 0; i < 128; ++i) {
            if (id % 3 == 0) {
                syslog.setAddr("127.0.0.1"); 
                syslog.setPort(514); 
                syslog.setFacility(LogFacilityMng::LF_DAEMON);
            }
            else if (id % 2 == 0) {
                syslog.setAddr("0.0.0.0"); 
                syslog.setPort(0); 
                syslog.setFacility(LogFacilityMng::LF_CRON2);
            }
            else {
                syslog.setLvl(LogLvlMng::LL_INFO);
                syslog.setFacility(LogFacilityMng::LF_AUTH);
            }

            syslog << "Test message" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    };

    std::vector<std::thread> threads;
    for (auto i = 0; i < 32; ++i)
        threads.push_back(std::thread{callSettersAndSendMsg, i});

    for (auto& thread : threads) 
        thread.join();
}

#include <future>

TEST_F(TestSyslogClient, asyncSendMsg_st) {
    auto syslog{makeUDPClient_st()};

    auto sendMsg = std::async([&]() {
        syslog << LogLvlMng::LL_INFO << "Async test message (st)" << std::endl;
    });

    sendMsg.get();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Async test message (st)"));
}

TEST_F(TestSyslogClient, asyncSendMsg_mt) {
    auto syslog{makeUDPClient_mt()};
    syslog.setFacility(LogFacilityMng::LF_AUDIT);

    auto sendMsg = std::async([&]() {
        syslog << LogLvlMng::LL_ERR << "Async test message (mt)" << std::endl;
    });

    sendMsg.get();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    ASSERT_TRUE(std::string::npos != tail(log).find("Async test message (mt)"));
}

TEST_F(TestSyslogClient, sendMsgWithNoFormattersOverUDP_st) {
    auto syslog{makeUDPClient_st()};
    syslog.cleanFormatters();

    syslog << LogLvlMng::LL_INFO << "Test message with no formatter flags (st)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    auto line{tail(log)};

    ASSERT_TRUE(std::string::npos != line.find("Test message with no formatter flags (st)"));
    ASSERT_TRUE(std::string::npos == line.find("["));
    ASSERT_TRUE(std::string::npos == line.find("]"));
}

TEST_F(TestSyslogClient, sendMsgWithNoFormattersOverUDP_mt) {
    auto syslog{makeUDPClient_mt()};
    syslog.cleanFormatters();

    syslog << LogLvlMng::LL_INFO << "Test message with no formatter flags (mt)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    auto line{tail(log)};

    ASSERT_TRUE(std::string::npos != line.find("Test message with no formatter flags (mt)"));
    ASSERT_TRUE(std::string::npos == line.find("["));
    ASSERT_TRUE(std::string::npos == line.find("]"));
}

TEST_F(TestSyslogClient, sendMsgWithDefaultFormattersOverUDP_st) {
    auto syslog{makeUDPClient_st()};

    syslog << LogLvlMng::LL_INFO << "Test message with default formatter flags (st)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    auto line{tail(log)};

    ASSERT_TRUE(std::string::npos != line.find("Test message with default formatter flags (st)"));
    ASSERT_TRUE(std::string::npos != line.find("[pid"));
    ASSERT_TRUE(std::string::npos != line.find("]"));
}

TEST_F(TestSyslogClient, sendMsgWithDefaultFormattersOverUDP_mt) {
    auto syslog{makeUDPClient_mt()};

    syslog << LogLvlMng::LL_INFO << "Test message with default formatter flags (mt)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    auto line{tail(log)};

    ASSERT_TRUE(std::string::npos != line.find("Test message with default formatter flags (mt)"));
    ASSERT_TRUE(std::string::npos != line.find("[pid"));
    ASSERT_TRUE(std::string::npos != line.find("]"));
}

TEST_F(TestSyslogClient, sendMsgWithNewFormatterOverUDP_st) {
    auto syslog{makeUDPClient_st()};
    syslog.addFormatter(std::make_shared<ModuleNameFormatter>("test"));

    syslog << LogLvlMng::LL_INFO << "Test message with new formatter flag (st)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    auto line{tail(log)};

    ASSERT_TRUE(std::string::npos != line.find("Test message with new formatter flag (st)"));
    ASSERT_TRUE(std::string::npos != line.find("[pid"));
    ASSERT_TRUE(std::string::npos != line.find("[module"));
    ASSERT_TRUE(std::string::npos != line.find("]"));
}

TEST_F(TestSyslogClient, sendMsgWithNewFormatterOverUDP_mt) {
    auto syslog{makeUDPClient_st()};
    syslog.addFormatter(std::make_shared<ModuleNameFormatter>("test"));

    syslog << LogLvlMng::LL_INFO << "Test message with new formatter flag (mt)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream log{logPath};
    auto line{tail(log)};

    ASSERT_TRUE(std::string::npos != line.find("Test message with new formatter flag (mt)"));
    ASSERT_TRUE(std::string::npos != line.find("[pid"));
    ASSERT_TRUE(std::string::npos != line.find("[module"));
    ASSERT_TRUE(std::string::npos != line.find("]"));

    system("cat /var/log/syslog");
}