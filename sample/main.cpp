/**
 * @file main.cpp
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

#include <thread>
#include <chrono>
#include <vector>

#include "syslog_client.hpp"

#if defined(WIN32)
 #pragma comment(lib, "Ws2_32.lib")
#endif

////////////////////////////////////////////////////////////////////////////
///
//
class ModuleNameFormatter : public syslog::IFormatter {
private:
    std::string m_ModuleName;
public:
    ModuleNameFormatter(std::string&& module_name) : m_ModuleName{std::move(module_name)} {}

    std::string key() const noexcept override { return "module"; }

    std::string value() const noexcept override { return m_ModuleName; }
};

////////////////////////////////////////////////////////////////////////////
///
//
int main() {
    auto syslog{syslog::makeUDPClient_st()};
    syslog.setFacility(syslog::LogFacilityMng::LF_LOCAL3);

    syslog << syslog::LogLvlMng::LL_INFO << "Message " << 1 << std::endl;

    auto moved{std::move(syslog)};

    moved << syslog::LogLvlMng::LL_NOTICE << "Message " << 2 << std::endl;

    syslog = syslog::makeUDPClient_mt();
    syslog.setLvl(syslog::LogLvlMng::LL_EMERG);
    syslog.setAddr("127.0.0.1");
    syslog.setPort(514);

    auto sendMsg = [&](){ 
        // syslog::LogLvlMng::LL_EMERG
        syslog << "Message from thread " << std::this_thread::get_id() << std::endl; 
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    std::vector<std::thread> threads;
    for (auto i = 0; i < 8; ++i)
        threads.push_back(std::thread{sendMsg});

    for (auto& thread : threads) 
        thread.join();

    syslog.addFormatter(std::make_shared<ModuleNameFormatter>("main")); 

    syslog << syslog::LogLvlMng::LL_INFO << "Message with new formatter flag" << std::endl;
}