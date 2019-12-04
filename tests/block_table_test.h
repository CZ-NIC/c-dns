#pragma once

#include <unordered_map>
#include "gtest/gtest.h"

#include "../src/cdns.h"

namespace CDNS {
    TEST(KeyRefTest, KRTest) {
        StringItem si, si2;
        si.data = "Test";
        si2.data = "Test2";

        KeyRef<std::string> kr(si.data);
        KeyRef<std::string> kr2(si2.data);

        EXPECT_TRUE(kr != kr2);
        EXPECT_FALSE(kr == kr2);
    }

    TEST(BlockTableTest, BTAddClearTest) {
        BlockTable<StringItem> bt;

        StringItem si, si2;
        si.data = "Test";
        si2.data = "Test2";

        index_t i = bt.add(si);
        index_t i2 = bt.add(si2);

        EXPECT_NE(i, i2);
        EXPECT_EQ(i, 0);
        EXPECT_EQ(i2, 1);
        EXPECT_EQ(bt.size(), 2);

        bt.clear();
        EXPECT_EQ(bt.size(), 0);
    }

    TEST(BlockTableTest, BTFindTest) {
        class Number {
            public:
            explicit Number(unsigned j) : i(j) {}
            bool operator==(const Number& rhs) const {
                return i == rhs.i;
            }
            bool operator!=(const Number& rhs) const {
                return !(*this == rhs);
            }
            const unsigned& key() const {
                return i;
            }
            const unsigned get() const {
                return i;
            }
            private:
            unsigned i;
        };
        BlockTable<Number, unsigned> bt;
        Number i(3), j(100), k(2345);

        index_t index = bt.add(i);
        index_t index2 = bt.add(j);
        EXPECT_EQ(bt.size(), 2);

        EXPECT_EQ(bt[index].get(), 3);
        EXPECT_EQ(bt[index2].get(), 100);

        index_t found;
        EXPECT_TRUE(bt.find(i.key(), found));
        EXPECT_TRUE(bt.find(j.key(), found));
        EXPECT_FALSE(bt.find(k.key(), found));
        index_t index3 = bt.add_value(k);
        EXPECT_TRUE(bt.find(k.key(), found));
        EXPECT_EQ(index3, found);
    }

    TEST(BlockTableTest, BTFind2Test) {
        AddressEventCount aec, aec2, aec3;
        BlockTable<AddressEventCount> bt;

        aec2.ae_code = 4;
        aec2.ae_code = 7;

        index_t index = bt.add(aec);
        index_t index2 = bt.add(aec2);
        EXPECT_EQ(bt.size(), 2);

        EXPECT_EQ(bt[index], aec);
        EXPECT_EQ(bt[index2], aec2);

        index_t found;
        EXPECT_TRUE(bt.find(aec.key(), found));
        EXPECT_TRUE(bt.find(aec2.key(), found));

        index_t index3 = bt.add(aec3);
        EXPECT_EQ(index, index3);
    }
}