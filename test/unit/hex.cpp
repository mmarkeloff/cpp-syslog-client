/**
 * @file hex.cpp
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

#include "hex.hpp"

using namespace syslog::details;

////////////////////////////////////////////////////////////////////////////
///
//
class TestInt2Hex : public ::testing::Test {
protected:
    void SetUp() { }

    void TearDown() { }
};

////////////////////////////////////////////////////////////////////////////
///
//
TEST_F(TestInt2Hex, signedInt) {
    ASSERT_EQ("0x00000000", int2hex(0));
    ASSERT_EQ("0x00000001", int2hex(1));
    ASSERT_EQ("0x0000000a", int2hex(10));
    ASSERT_EQ("0x00000064", int2hex(100));
    ASSERT_EQ("0x000000c8", int2hex(200));
    ASSERT_EQ("0x000003e8", int2hex(1000));
    ASSERT_EQ("0xffffffff", int2hex(-1));
    ASSERT_EQ("0xfffffff6", int2hex(-10));
    ASSERT_EQ("0xffffff9c", int2hex(-100));
}
