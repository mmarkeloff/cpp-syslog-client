/**
 * @file udp_client.cpp
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

#include "client_impl.hpp"

using namespace syslog;

////////////////////////////////////////////////////////////////////////////
///
//
class TestUDPClient : public ::testing::Test {
protected:
    void SetUp() { }

    void TearDown() { }
};

////////////////////////////////////////////////////////////////////////////
///
//
TEST_F(TestUDPClient, init) {
    UDPClient clnt;

    ASSERT_EQ(true, clnt.isInitialised());

    clnt.setAddr("0.0.0.0");

    ASSERT_EQ(true, clnt.isInitialised());

    clnt.setPort(8080);

    ASSERT_EQ(true, clnt.isInitialised());
}

TEST_F(TestUDPClient, moveClassInstByMoveCtor) {
    UDPClient clnt;

    ASSERT_EQ(true, clnt.isInitialised());

    clnt.setAddr("127.0.0.1");
    clnt.setPort(8080);

    ASSERT_EQ(true, clnt.isInitialised());

    auto moved{std::move(clnt)};

    ASSERT_EQ(true, moved.isInitialised());
}

TEST_F(TestUDPClient, moveClassInstByMoveAssigmentOperator) {
    UDPClient clnt;

    ASSERT_EQ(true, clnt.isInitialised());

    clnt.setAddr("0.0.0.0");
    clnt.setPort(8080);

    ASSERT_EQ(true, clnt.isInitialised());

    UDPClient moved;
    moved = std::move(clnt);

    ASSERT_EQ(true, moved.isInitialised());
}

TEST_F(TestUDPClient, setAddr) {
    UDPClient clnt;

    ASSERT_EQ(true, clnt.isInitialised());

    clnt.setAddr("0.0.0.0");

    ASSERT_EQ(true, clnt.isInitialised());
}

TEST_F(TestUDPClient, getSock) {
    UDPClient clnt;

    ASSERT_EQ(true, clnt.isInitialised());
    ASSERT_TRUE(-1 != clnt.getSock());
}

TEST_F(TestUDPClient, sendData) {
    UDPClient clnt;

    ASSERT_EQ(true, clnt.isInitialised());

    clnt.send("test data");
}