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
#include <zlib.h>
#include <lzma.h>
#include "gtest/gtest.h"

#include "../src/cdns.h"
#include "common.h"

namespace CDNS {
    TEST(CborOutputWriterTest, COWCTest) {
        CborOutputWriter* cow = new CborOutputWriter(file);
        struct stat buff;

        EXPECT_EQ(stat(file.c_str(), &buff), 0);
        delete cow;
        remove_file(file);

        EXPECT_EQ(stat(file.c_str(), &buff), -1);
    }

    TEST(CborOutputWriterTest, COWWriteTest) {
        CborOutputWriter* cow = new CborOutputWriter(file);
        std::string out("test");

        cow->write(out.c_str(), out.size());
        delete cow;

        test_content_and_remove_file(file, out);
    }

    TEST(CborOutputWriterTest, COWRotateTest) {
        CborOutputWriter* cow = new CborOutputWriter(file);
        std::string out("test");

        cow->write(out.c_str(), out.size());
        cow->rotate_output(file2);
        cow->write(out.c_str(), out.size());
        delete cow;

        test_content_and_remove_file(file, out);
        test_content_and_remove_file(file2, out);
    }

    TEST(CborOutputWriterFDTest, COWFDTest) {
        int fd = open(file.c_str(), O_CREAT | O_RDWR, 0644);
        CborOutputWriter* cow = new CborOutputWriter(fd);
        struct stat buff;

        EXPECT_EQ(stat(file.c_str(), &buff), 0);
        delete cow;
        remove_file(file);

        EXPECT_EQ(stat(file.c_str(), &buff), -1);
    }

    TEST(CborOutputWriterFDTest, COWFDWriteTest) {
        int fd = open(file.c_str(), O_CREAT | O_RDWR, 0644);
        CborOutputWriter* cow = new CborOutputWriter(fd);
        std::string out("test");

        cow->write(out.c_str(), out.size());
        delete cow;

        test_content_and_remove_file(file, out);
    }

    TEST(CborOutputWriterFDTest, COWFDRotateTest) {
        int fd = open(file.c_str(), O_CREAT | O_RDWR, 0644);
        CborOutputWriter* cow = new CborOutputWriter(fd);
        std::string out("test");

        cow->write(out.c_str(), out.size());
        int fd2 = open(file2.c_str(), O_CREAT | O_RDWR, 0644);
        cow->rotate_output(fd2);
        cow->write(out.c_str(), out.size());
        delete cow;

        test_content_and_remove_file(file, out);
        test_content_and_remove_file(file2, out);
    }

    TEST(GzipCborOutputWriterTest, GCOWCTest) {
        GzipCborOutputWriter* cow = new GzipCborOutputWriter(file);
        struct stat buff;

        EXPECT_EQ(stat((file + ".gz").c_str(), &buff), 0);
        delete cow;

        remove_file(file + ".gz");

        EXPECT_EQ(stat((file + ".gz").c_str(), &buff), -1);
    }

    TEST(GzipCborOutputWriterTest, GCOWWriteTest) {
        GzipCborOutputWriter* cow = new GzipCborOutputWriter(file);
        std::string out("test");

        cow->write(out.c_str(), out.size());
        delete cow;

        gzFile gzfile = gzopen((file + ".gz").c_str(), "rb");
        char gz[255];
        int ret = gzread(gzfile, gz, 255);
        EXPECT_EQ(ret, out.size());
        EXPECT_EQ(std::string(gz, ret), out);
        gzclose(gzfile);

        remove_file(file + ".gz");
    }

    TEST(GzipCborOutputWriterTest, GCOWFDWriteTest) {
        int fd = open((file + ".gz").c_str(), O_CREAT | O_RDWR, 0644);
        GzipCborOutputWriter* cow = new GzipCborOutputWriter(fd);
        std::string out("test");

        cow->write(out.c_str(), out.size());
        delete cow;

        gzFile gzfile = gzopen((file + ".gz").c_str(), "rb");
        char gz[255];
        int ret = gzread(gzfile, gz, 255);
        EXPECT_EQ(ret, out.size());
        EXPECT_EQ(std::string(gz, ret), out);
        gzclose(gzfile);

        remove_file(file + ".gz");
    }

    TEST(XzCborOutputWriterTest, XCOWCTest) {
        XzCborOutputWriter* cow = new XzCborOutputWriter(file);
        struct stat buff;

        EXPECT_EQ(stat((file + ".xz").c_str(), &buff), 0);
        delete cow;

        remove_file(file + ".xz");

        EXPECT_EQ(stat((file + ".xz").c_str(), &buff), -1);
    }

    TEST(XzCborOutputWriterTest, XCOWWriteTest) {
        XzCborOutputWriter* cow = new XzCborOutputWriter(file);
        struct stat buff;
        std::string out("test");
        std::string result;

        cow->write(out.c_str(), out.size());
        delete cow;

        FILE* lzfile = fopen((file + ".xz").c_str(), "r");
        lzma_stream lzma = LZMA_STREAM_INIT;
        lzma_ret ret = lzma_stream_decoder(&lzma, UINT64_MAX, LZMA_CONCATENATED);
        lzma_action action = LZMA_RUN;
        char inbuf[255];
        char outbuf[255];
        lzma.next_in = reinterpret_cast<uint8_t*>(inbuf);
        lzma.avail_in = 0;
        lzma.next_out = reinterpret_cast<uint8_t*>(outbuf);
        lzma.avail_out = sizeof(outbuf);

        while (true) {
            if (lzma.avail_out == 0 || ret == LZMA_STREAM_END) {
                result += std::string(outbuf, sizeof(outbuf) - lzma.avail_out);
                break;
            }

            if (lzma.avail_in == 0 && !feof(lzfile)) {
                lzma.avail_in = fread(inbuf, 1, 255, lzfile);

                if (feof(lzfile))
                    action = LZMA_FINISH;
            }

            ret = lzma_code(&lzma, action);
        }

        EXPECT_EQ(result, out);

        lzma_end(&lzma);
        fclose(lzfile);

        remove_file(file + ".xz");
    }

    TEST(XzCborOutputWriterTest, XCOWFDWriteTest) {
        int fd = open((file + ".xz").c_str(), O_CREAT | O_RDWR, 0644);
        XzCborOutputWriter* cow = new XzCborOutputWriter(fd);
        struct stat buff;
        std::string out("test");
        std::string result;

        cow->write(out.c_str(), out.size());
        delete cow;

        FILE* lzfile = fopen((file + ".xz").c_str(), "r");
        lzma_stream lzma = LZMA_STREAM_INIT;
        lzma_ret ret = lzma_stream_decoder(&lzma, UINT64_MAX, LZMA_CONCATENATED);
        lzma_action action = LZMA_RUN;
        char inbuf[255];
        char outbuf[255];
        lzma.next_in = reinterpret_cast<uint8_t*>(inbuf);
        lzma.avail_in = 0;
        lzma.next_out = reinterpret_cast<uint8_t*>(outbuf);
        lzma.avail_out = sizeof(outbuf);

        while (true) {
            if (lzma.avail_out == 0 || ret == LZMA_STREAM_END) {
                result += std::string(outbuf, sizeof(outbuf) - lzma.avail_out);
                break;
            }

            if (lzma.avail_in == 0 && !feof(lzfile)) {
                lzma.avail_in = fread(inbuf, 1, 255, lzfile);

                if (feof(lzfile))
                    action = LZMA_FINISH;
            }

            ret = lzma_code(&lzma, action);
        }

        EXPECT_EQ(result, out);

        lzma_end(&lzma);
        fclose(lzfile);

        remove_file(file + ".xz");
    }
}