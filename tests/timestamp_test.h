/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdint.h>
#include <gtest/gtest.h>

#include "../src/cdns.h"

namespace CDNS {
    TEST(TimestampTest, TimestampDefCtest) {
        Timestamp ts;
        EXPECT_EQ(ts.m_secs, 0);
        EXPECT_EQ(ts.m_ticks, 0);
    }

    TEST(TimestampTest, TimestampCtest) {
        Timestamp ts = {42, 170};

        EXPECT_EQ(ts.m_secs, 42);
        EXPECT_EQ(ts.m_ticks, 170);
    }

    TEST(TimestampTest, TimestampCompareTest) {
        Timestamp ts = {42, 170};
        Timestamp ts2 = {56, 50};

        EXPECT_TRUE(ts < ts2);
        EXPECT_TRUE(ts <= ts2);
        EXPECT_FALSE(ts2 < ts);
        EXPECT_FALSE(ts2 <= ts);

        ts2.m_secs = 42;
        ts2.m_ticks = 170;

        EXPECT_TRUE(ts <= ts2);
        EXPECT_TRUE(ts2 <= ts);
    }

    TEST(TimestampTest, TimestampGetOffsetTest) {
        Timestamp ts = {2, 700};
        Timestamp ts2 = {5, 300};

        EXPECT_EQ(ts2.get_time_offset(ts, 1000), 2600);
        EXPECT_EQ(ts.get_time_offset(ts2, 1000), -2600);
    }

    TEST(TimestampTest, TimestampGetOffset2Test) {
        Timestamp ts = {2, 100};
        Timestamp ts2 = {5, 400};

        EXPECT_EQ(ts2.get_time_offset(ts, 1000), 3300);
        EXPECT_EQ(ts.get_time_offset(ts2, 1000), -3300);
    }

    TEST(TimestampTest, TimestampAddOffsetTest) {
        Timestamp ts = {2, 700};
        int offset = 2600;
        ts.add_time_offset(offset, 1000);

        EXPECT_EQ(ts.m_secs, 5);
        EXPECT_EQ(ts.m_ticks, 300);
    }

    TEST(TimestampTest, TimestampAddOffset2Test) {
        Timestamp ts = {5, 300};
        int offset = -2600;
        ts.add_time_offset(offset, 1000);

        EXPECT_EQ(ts.m_secs, 2);
        EXPECT_EQ(ts.m_ticks, 700);
    }
}