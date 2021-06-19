/**
 * @file mt.cpp
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

#include <gtest/gtest.h>

#include "tmode.hpp"

using namespace syslog::details;

////////////////////////////////////////////////////////////////////////////
///
//
class TestMT : public ::testing::Test {
protected:
    void SetUp() { }

    void TearDown() { }
};

////////////////////////////////////////////////////////////////////////////
///
//
TEST_F(TestMT, simpleMainThread) {
    mt mode;
    mode.lock();
    mode.unlock();
}

mt G_MTMode;

TEST_F(TestMT, recursiveMainThread) {
    for (auto i = 0; i < 16; ++i)
        G_MTMode.lockRec();

    G_MTMode.unlockRec();
    G_MTMode.unlockRec();
}

TEST_F(TestMT, recursiveBySameThread) {
    for (auto i = 0; i < 32; ++i)
        G_MTMode.lockRec();

    G_MTMode.unlockRec();
}

#include <thread>
#include <chrono>

TEST_F(TestMT, simpleByMultiThreads) {
    auto f = [](){
        for (auto i = 0; i < 64; ++i) {
            G_MTMode.lock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            G_MTMode.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < 16; i++)
        threads.push_back(std::thread{f});

    for (auto& thread : threads) 
        thread.join();
}

TEST_F(TestMT, recursiveByMultiThreads) {
    auto f = [](){
        for (auto i = 0; i < 32; ++i) {
            G_MTMode.lockRec();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        G_MTMode.unlockRec();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < 16; i++)
        threads.push_back(std::thread{f});

    for (auto& thread : threads) 
        thread.join();
}