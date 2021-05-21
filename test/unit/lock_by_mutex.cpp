/**
 * @file      lock_by_mutex.cpp
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

#include <gtest/gtest.h>

#include <lock_by_mutex.hpp>

using namespace syslog;

////////////////////////////////////////////////////////////////////////////
///
//
class TestLockByMUTEX : public ::testing::Test {
protected:
    void SetUp() { }

    void TearDown() { }
};

////////////////////////////////////////////////////////////////////////////
///
//
TEST_F(TestLockByMUTEX, enter) {
    LockByMUTEX mtx;

    ASSERT_EQ(false, mtx.isEntered());

    mtx.enter();

    ASSERT_EQ(true, mtx.isEntered());
}

TEST_F(TestLockByMUTEX, exit) {
    LockByMUTEX mtx;

    ASSERT_EQ(false, mtx.isEntered());

    mtx.exit();

    ASSERT_EQ(false, mtx.isEntered());
}

TEST_F(TestLockByMUTEX, enterAndExit) {
    LockByMUTEX mtx;

    ASSERT_EQ(false, mtx.isEntered());

    mtx.enter();

    ASSERT_EQ(true, mtx.isEntered());

    mtx.exit();

    ASSERT_EQ(false, mtx.isEntered());
}

#include <vector>
#include <thread>
#include <chrono>

TEST_F(TestLockByMUTEX, enterAndExitByMultiplyThreads) {
    auto f = [](LockByMUTEX& mtx) {
        mtx.enter();

        ASSERT_EQ(true, mtx.isEntered());

        mtx.exit();

        ASSERT_EQ(false, mtx.isEntered());

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };

    LockByMUTEX mtx;
    std::vector<std::thread> threads;

    for (std::size_t i = 0; i < 16; i++)
        threads.push_back(std::thread{f, std::ref(mtx)});

    for (auto& thread : threads) 
        thread.join();
}