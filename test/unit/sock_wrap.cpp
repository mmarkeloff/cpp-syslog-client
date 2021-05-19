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

#include <gtest/gtest.h>

#include <sock_wrap.hpp>

using namespace syslog;

////////////////////////////////////////////////////////////////////////////
///
//
class TestSockWrap : public ::testing::Test {
protected:
	void SetUp() { }

	void TearDown() { }
};

////////////////////////////////////////////////////////////////////////////
///
//
TEST_F(TestSockWrap, init) {
    SockWrap sock{"127.0.0.1", 8080};

    ASSERT_EQ(false, sock.isInitialised());

    sock.init();

    ASSERT_EQ(true, sock.isInitialised());
}

TEST_F(TestSockWrap, moveClassInstanceByMoveCtor) {
    SockWrap sock{"127.0.0.1", 8080};

    ASSERT_EQ(false, sock.isInitialised());

    sock.init();

    ASSERT_EQ(true, sock.isInitialised());

    auto moved{std::move(sock)};

    ASSERT_EQ(true, moved.isInitialised());
}

TEST_F(TestSockWrap, moveClassInstanceByMoveAssigmentOperator) {
    SockWrap sock{"127.0.0.1", 8080};

    ASSERT_EQ(false, sock.isInitialised());

    sock.init();

    ASSERT_EQ(true, sock.isInitialised());

    SockWrap moved;
    moved = std::move(sock);

    ASSERT_EQ(true, moved.isInitialised());
}

TEST_F(TestSockWrap, setAddr) {
    SockWrap sock{"127.0.0.1", 8080};

    sock.setAddr("0.0.0.0");

    ASSERT_EQ(false, sock.isInitialised());

    sock.init();

    ASSERT_EQ(true, sock.isInitialised());
}

TEST_F(TestSockWrap, getSock) {
    SockWrap sock{"127.0.0.1", 8080};

    ASSERT_EQ(false, sock.isInitialised());

    sock.init();

    ASSERT_EQ(true, sock.isInitialised());

    ASSERT_TRUE(-1 != sock.getSock());
}

TEST_F(TestSockWrap, send) {
    SockWrap sock{"127.0.0.1", 8080};

    ASSERT_EQ(false, sock.isInitialised());

    sock.init();

    ASSERT_EQ(true, sock.isInitialised());

    sock.send("test data");
}