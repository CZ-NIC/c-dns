/**
 * Copyright © 2020 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <fstream>
#include <istream>
#include <sstream>
#include <gtest/gtest.h>

#include "../src/cdns.h"
#include "common.h"

namespace CDNS {
    std::string dunsigned = "\x18\x2A"; // 42
    std::string dnegative = "\x39\x10\x91"; // -4242
    std::string dbytestring = "\x44test"; // "test"
    std::string dtextstring = "\x64test"; // "test"
    std::string darray = "\x82"; // start of array of size 2
    std::string dindef_array = "\x9F"; // start of indefinite length array
    std::string dmap = "\xB9\x01\xA4"; // start of map of size 420
    std::string dindef_map = "\xBF"; // start of indefinite length map
    std::string dtag = "\xC0"; // tag
    std::string dstop_code = "\xFF"; // break stop code
    std::string dsimple = "\xE0"; // simple type

    TEST(CdnsDecoderTest, CDPeekTest) {
        std::istringstream is_unsig(dunsigned);
        CdnsDecoder dec_unsig(is_unsig);
        CborType res = dec_unsig.peek_type();
        EXPECT_EQ(res, CborType::UNSIGNED);

        std::istringstream is_neg(dnegative);
        CdnsDecoder dec_neg(is_neg);
        res = dec_neg.peek_type();
        EXPECT_EQ(res, CborType::NEGATIVE);

        std::istringstream is_bytes(dbytestring);
        CdnsDecoder dec_bytes(is_bytes);
        res = dec_bytes.peek_type();
        EXPECT_EQ(res, CborType::BYTE_STRING);

        std::istringstream is_texts(dtextstring);
        CdnsDecoder dec_texts(is_texts);
        res = dec_texts.peek_type();
        EXPECT_EQ(res, CborType::TEXT_STRING);

        std::istringstream is_array(darray);
        CdnsDecoder dec_array(is_array);
        res = dec_array.peek_type();
        EXPECT_EQ(res, CborType::ARRAY);

        std::istringstream is_indef_array(dindef_array);
        CdnsDecoder dec_indef_array(is_indef_array);
        res = dec_indef_array.peek_type();
        EXPECT_EQ(res, CborType::ARRAY);

        std::istringstream is_map(dmap);
        CdnsDecoder dec_map(is_map);
        res = dec_map.peek_type();
        EXPECT_EQ(res, CborType::MAP);

        std::istringstream is_indef_map(dindef_map);
        CdnsDecoder dec_indef_map(is_indef_map);
        res = dec_indef_map.peek_type();
        EXPECT_EQ(res, CborType::MAP);

        std::istringstream is_tag(dtag);
        CdnsDecoder dec_tag(is_tag);
        res = dec_tag.peek_type();
        EXPECT_EQ(res, CborType::TAG);

        std::istringstream is_stop_code(dstop_code);
        CdnsDecoder dec_stop_code(is_stop_code);
        res = dec_stop_code.peek_type();
        EXPECT_EQ(res, CborType::BREAK);

        std::istringstream is_simple(dsimple);
        CdnsDecoder dec_simple(is_simple);
        res = dec_simple.peek_type();
        EXPECT_EQ(res, CborType::SIMPLE);
    }

    TEST(CdnsDecoderTest, CDUnsignedTest) {
        std::istringstream is(dunsigned);
        CdnsDecoder dec(is);

        unsigned res = dec.read_unsigned();
        EXPECT_EQ(res, 42);
    }

    TEST(CdnsDecoderTest, CDNegativeTest) {
        std::istringstream is(dnegative);
        CdnsDecoder dec(is);

        int res = dec.read_negative();
        EXPECT_EQ(res, -4242);
    }

    TEST(CdnsDecoderTest, CDIntegerTest) {
        std::istringstream is(dunsigned);
        std::istringstream is2(dnegative);
        CdnsDecoder dec(is);
        CdnsDecoder dec2(is2);

        unsigned res = dec.read_integer();
        int res2 = dec2.read_integer();
        EXPECT_EQ(res, 42);
        EXPECT_EQ(res2, -4242);
    }

    TEST(CdnsDecoderTest, CDBoolTest) {
        std::string bool_false = "\xF4";
        std::string bool_true = "\xF5";

        std::istringstream is(bool_false + bool_true);
        CdnsDecoder dec(is);

        bool res = dec.read_bool();
        EXPECT_EQ(res, false);

        res = dec.read_bool();
        EXPECT_EQ(res, true);
    }

    TEST(CdnsDecoderTest, CDBytestringTest) {
        std::istringstream is(dbytestring);
        CdnsDecoder dec(is);

        std::string res = dec.read_bytestring();
        EXPECT_EQ(res, "test");
    }

    TEST(CdnsDecoderTest, CDTextstringTest) {
        std::istringstream is(dtextstring);
        CdnsDecoder dec(is);

        std::string res = dec.read_textstring();
        EXPECT_EQ(res, "test");
    }

    TEST(CdnsDecoderTest, CDArrayStartTest) {
        std::istringstream is(darray + dindef_array);
        CdnsDecoder dec(is);

        bool indef = false;
        unsigned res = dec.read_array_start(indef);
        EXPECT_EQ(indef, false);
        EXPECT_EQ(res, 2);

        res = dec.read_array_start(indef);
        EXPECT_EQ(indef, true);
        EXPECT_EQ(res, 0);
    }

    TEST(CdnsDecoderTest, CDArrayTest) {
        std::istringstream is(darray + dunsigned + dnegative);
        CdnsDecoder dec(is);
        std::vector<int> res;

        dec.read_array([this, &res](CdnsDecoder& dec){
            res.push_back(dec.read_integer());
        });

        EXPECT_EQ(res.size(), 2);
        EXPECT_EQ(res[0], 42);
        EXPECT_EQ(res[1], -4242);
    }

    TEST(CdnsDecoderTest, CDIndefArrayTest) {
        std::istringstream is(dindef_array + dunsigned + dnegative + dstop_code);
        CdnsDecoder dec(is);
        std::vector<int> res;

        dec.read_array([this, &res](CdnsDecoder& dec){
            res.push_back(dec.read_integer());
        });

        EXPECT_EQ(res.size(), 2);
        EXPECT_EQ(res[0], 42);
        EXPECT_EQ(res[1], -4242);
    }

    TEST(CdnsDecoderTest, CDMapStartTest) {
        std::istringstream is(dmap + dindef_map);
        CdnsDecoder dec(is);

        bool indef = false;
        unsigned res = dec.read_map_start(indef);
        EXPECT_EQ(indef, false);
        EXPECT_EQ(res, 420);

        res = dec.read_map_start(indef);
        EXPECT_EQ(indef, true);
        EXPECT_EQ(res, 0);
    }

    TEST(CdnsDecoderTest, CDBreakTest) {
        std::istringstream is(dstop_code + dunsigned);
        CdnsDecoder dec(is);

        CborType peek = dec.peek_type();
        EXPECT_EQ(peek, CborType::BREAK);
        dec.read_break();

        peek = dec.peek_type();
        EXPECT_EQ(peek, CborType::UNSIGNED);
    }

    TEST(CdnsDecoderTest, CDSkipTest) {
        std::istringstream is(dunsigned + dbytestring + dsimple);
        CdnsDecoder dec(is);

        CborType peek = dec.peek_type();
        EXPECT_EQ(peek, CborType::UNSIGNED);

        dec.skip_item();
        peek = dec.peek_type();
        EXPECT_EQ(peek, CborType::BYTE_STRING);

        dec.skip_item();
        peek = dec.peek_type();
        EXPECT_EQ(peek, CborType::SIMPLE);
    }
}