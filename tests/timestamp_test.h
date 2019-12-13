#pragma once

#include <stdint.h>
#include "gtest/gtest.h"

#include "../src/cdns.h"

namespace CDNS {
    TEST(TimestampTest, TimestampCtest) {
        Timestamp ts;
        EXPECT_EQ(ts.m_secs, 0);
        EXPECT_EQ(ts.m_ticks, 0);
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

    TEST(TimestampTest, TimestampOffsetTest) {
        Timestamp ts = {2, 700};
        Timestamp ts2 = {5, 300};

        EXPECT_EQ(ts2.get_time_offset(ts, 1000), 2600);
        EXPECT_EQ(ts.get_time_offset(ts2, 1000), -2600);
    }

    TEST(TimestampTest, TimestampOffset2Test) {
        Timestamp ts = {2, 100};
        Timestamp ts2 = {5, 400};

        EXPECT_EQ(ts2.get_time_offset(ts, 1000), 3300);
        EXPECT_EQ(ts.get_time_offset(ts2, 1000), -3300);
    }
}