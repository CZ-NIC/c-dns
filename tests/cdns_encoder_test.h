/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include <gtest/gtest.h>

#include "../src/cdns.h"
#include "common.h"

namespace CDNS {

    TEST(CdnsEncoderTest, CECTest) {
        CdnsEncoder* enc = new CdnsEncoder(file, CborOutputCompression::NO_COMPRESSION);
        struct stat buff;

        EXPECT_EQ(stat((file + ".part").c_str(), &buff), 0);
        delete enc;

        remove_file(file);

        EXPECT_EQ(stat(file.c_str(), &buff), -1);
    }

    TEST(CdnsEncoderTest, CEMapTest) {
        CdnsEncoder* enc = new CdnsEncoder(file, CborOutputCompression::NO_COMPRESSION);

        enc->write_map_start(0);
        enc->write_indef_map_start();
        enc->write_break();
        delete enc;

        std::string result("\xA0\xBF\xFF", sizeof("\xA0\xBF\xFF") - 1);
        test_content_and_remove_file(file, result);
    }

    TEST(CdnsEncoderTest, CEArrayTest) {
        CdnsEncoder* enc = new CdnsEncoder(file, CborOutputCompression::NO_COMPRESSION);

        enc->write_array_start(1);
        enc->write_indef_array_start();
        enc->write_break();
        delete enc;

        std::string result("\x81\x9F\xFF", sizeof("\x81\x9F\xFF") - 1);
        test_content_and_remove_file(file, result);
    }

    TEST(CdnsEncoderTest, CEBoolTest) {
        CdnsEncoder* enc = new CdnsEncoder(file, CborOutputCompression::NO_COMPRESSION);

        enc->write_array_start(2);
        enc->write(true);
        enc->write(false);
        delete enc;

        std::string result("\x82\xF5\xF4", sizeof("\x82\xF5\xF4") - 1);
        test_content_and_remove_file(file, result);
    }

    TEST(CdnsEncoderTest, CEIntTest) {
        CdnsEncoder* enc = new CdnsEncoder(file, CborOutputCompression::NO_COMPRESSION);

        uint8_t num8 = 64;
        uint16_t num16 = 65128;
        uint32_t num32 = 512256;
        uint64_t num64 = 6512365425;
        int8_t nnum8 = -64;
        int16_t nnum16 = -24512;
        int32_t nnum32 = -128652;
        int64_t nnum64 = -6512365425;

        enc->write_indef_array_start();
        enc->write(num8);
        enc->write(num16);
        enc->write(num32);
        enc->write(num64);
        enc->write(nnum8);
        enc->write(nnum16);
        enc->write(nnum32);
        enc->write(nnum64);
        enc->write_break();
        delete enc;

        std::string result("\x9F\x18\x40\x19\xFE\x68\x1A\x00\x07\xD1\x00\x1B\x00\x00\x00\x01\x84\x2A\xCF\x71\x38\x3F\x39\x5F\xBF\x3A\x00\x01\xF6\x8B\x3B\x00\x00\x00\x01\x84\x2A\xCF\x70\xFF",
                           sizeof("\x9F\x18\x40\x19\xFE\x68\x1A\x00\x07\xD1\x00\x1B\x00\x00\x00\x01\x84\x2A\xCF\x71\x38\x3F\x39\x5F\xBF\x3A\x00\x01\xF6\x8B\x3B\x00\x00\x00\x01\x84\x2A\xCF\x70\xFF") - 1);
        test_content_and_remove_file(file, result);
    }

    TEST(CdnsEncoderTest, CEStringTest) {
        CdnsEncoder* enc = new CdnsEncoder(file, CborOutputCompression::NO_COMPRESSION);

        std::string textstring("textstring");
        std::string bytestring("bytestring");

        enc->write_array_start(2);
        enc->write_textstring(textstring);
        enc->write_bytestring(bytestring);
        delete enc;

        std::string result("\x82\x6A\x74\x65\x78\x74\x73\x74\x72\x69\x6E\x67\x4A\x62\x79\x74\x65\x73\x74\x72\x69\x6E\x67",
                           sizeof("\x82\x6A\x74\x65\x78\x74\x73\x74\x72\x69\x6E\x67\x4A\x62\x79\x74\x65\x73\x74\x72\x69\x6E\x67") - 1);
        test_content_and_remove_file(file, result);
    }

    TEST(CdnsEncoderTest, CERotateTest) {
        CdnsEncoder* enc = new CdnsEncoder(file, CborOutputCompression::NO_COMPRESSION);

        enc->write_array_start(1);
        enc->write(uint8_t(3));
        enc->rotate_output(file2);
        enc->write_array_start(1);
        enc->write(uint8_t(4));
        delete enc;

        std::string result("\x81\x03", sizeof("\x81\x03") - 1);
        test_content_and_remove_file(file, result);

        std::string result2("\x81\x04", sizeof("\x81\x04") - 1);
        test_content_and_remove_file(file2, result2);
    }
}