/**
 * @file st.cpp
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
class TestST : public ::testing::Test {
protected:
    void SetUp() { }

    void TearDown() { }
};

////////////////////////////////////////////////////////////////////////////
///
//
TEST_F(TestST, simpleMainThread) {
    st mode;
    mode.lock();
    mode.unlock();
    mode.unlock();
}

st G_STMode;

TEST_F(TestST, recursiveMainThread) {
    G_STMode.lockRec();
    G_STMode.lockRec();
    G_STMode.lockRec();
    G_STMode.unlockRec();
    G_STMode.unlockRec();
}

TEST_F(TestST, recursiveBySameThread) {
    G_STMode.lockRec();
    G_STMode.lockRec();
    G_STMode.lockRec();
    G_STMode.unlockRec();
    G_STMode.unlockRec();
}