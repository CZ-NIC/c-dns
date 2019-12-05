#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include <zlib.h>
#include <lzma.h>
#include "gtest/gtest.h"

#include "../src/cdns.h"

namespace CDNS {
    TEST(CborOutputWriterTest, COWCTest) {
        std::string filename = "test.out";
        CborOutputWriter* cow = new CborOutputWriter(filename);
        struct stat buff;

        EXPECT_EQ(stat((filename + ".cdns").c_str(), &buff), 0);
        delete cow;
        if (stat((filename + ".cdns").c_str(), &buff) == 0)
            remove((filename + ".cdns").c_str());

        EXPECT_EQ(stat((filename + ".cdns").c_str(), &buff), -1);
    }

    TEST(CborOutputWriterTest, COWWriteTest) {
        std::string filename = "test.out";
        CborOutputWriter* cow = new CborOutputWriter(filename);
        struct stat buff;
        std::string out("test");

        cow->write(out.c_str(), out.size());
        delete cow;

        std::ifstream file(filename + ".cdns");
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        EXPECT_EQ(out, str);
        if (stat((filename + ".cdns").c_str(), &buff) == 0)
            remove((filename + ".cdns").c_str());
    }

    TEST(CborOutputWriterTest, COWRotateTest) {
        std::string filename = "test.out";
        std::string filename2 = "test2.out";
        CborOutputWriter* cow = new CborOutputWriter(filename);
        struct stat buff;
        std::string out("test");

        cow->write(out.c_str(), out.size());

        int fd = open(filename2.c_str(), O_CREAT | O_RDWR, 0644);
        cow->rotate_output(fd);
        cow->write(out.c_str(), out.size());
        delete cow;

        std::ifstream file(filename + ".cdns");
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        EXPECT_EQ(out, str);
        if (stat((filename + ".cdns").c_str(), &buff) == 0)
            remove((filename + ".cdns").c_str());

        std::ifstream file2(filename2);
        std::string str2((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());
        EXPECT_EQ(out, str2);
        if (stat((filename2).c_str(), &buff) == 0)
            remove((filename2).c_str());
    }

    TEST(GzipCborOutputWriterTest, GCOWCTest) {
        std::string filename = "test.out";
        GzipCborOutputWriter* cow = new GzipCborOutputWriter(filename);
        struct stat buff;

        EXPECT_EQ(stat((filename + ".cdns.gz").c_str(), &buff), 0);
        delete cow;
        if (stat((filename + ".cdns.gz").c_str(), &buff) == 0)
            remove((filename + ".cdns.gz").c_str());

        EXPECT_EQ(stat((filename + ".cdns.gz").c_str(), &buff), -1);
    }

    TEST(GzipCborOutputWriterTest, GCOWWriteTest) {
        std::string filename = "test.out";
        GzipCborOutputWriter* cow = new GzipCborOutputWriter(filename);
        struct stat buff;
        std::string out("test");

        cow->write(out.c_str(), out.size());
        delete cow;

        gzFile file = gzopen((filename + ".cdns.gz").c_str(), "rb");
        char gz[out.size()];
        int ret = gzread(file, gz, out.size());
        EXPECT_EQ(ret, out.size());
        EXPECT_EQ(std::string(gz), out);
        gzclose(file);

        if (stat((filename + ".cdns.gz").c_str(), &buff) == 0)
            remove((filename + ".cdns.gz").c_str());
    }

    TEST(XzCborOutputWriterTest, XCOWCTest) {
        std::string filename = "test.out";
        XzCborOutputWriter* cow = new XzCborOutputWriter(filename);
        struct stat buff;

        EXPECT_EQ(stat((filename + ".cdns.xz").c_str(), &buff), 0);
        delete cow;
        if (stat((filename + ".cdns.xz").c_str(), &buff) == 0)
            remove((filename + ".cdns.xz").c_str());

        EXPECT_EQ(stat((filename + ".cdns.xz").c_str(), &buff), -1);
    }

    TEST(XzCborOutputWriterTest, XCOWWriteTest) {
        std::string filename = "test.out";
        XzCborOutputWriter* cow = new XzCborOutputWriter(filename);
        struct stat buff;
        std::string out("test");
        std::string result;

        cow->write(out.c_str(), out.size());
        delete cow;

        FILE* file = fopen((filename + ".cdns.xz").c_str(), "r");
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

            if (lzma.avail_in == 0 && !feof(file)) {
                lzma.avail_in = fread(inbuf, 1, 255, file);

                if (feof(file))
                    action = LZMA_FINISH;
            }

            ret = lzma_code(&lzma, action);
        }

        EXPECT_EQ(result, out);

        lzma_end(&lzma);
        fclose(file);

        if (stat((filename + ".cdns.xz").c_str(), &buff) == 0)
            remove((filename + ".cdns.xz").c_str());
    }
}