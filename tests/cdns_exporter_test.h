#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include "gtest/gtest.h"

#include "../src/cdns.h"

namespace CDNS {
    TEST(CdnsExporterTest, CECTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, "test.out", CborOutputCompression::NO_COMPRESSION);

        EXPECT_EQ(exporter->get_block_item_count(), 0);
        delete exporter;

        struct stat buff;
        if (stat("test.out.cdns", &buff) == 0)
            remove("test.out.cdns");
    }

    TEST(CdnsExporterTest, CEBufferWriteQRTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, "test.out", CborOutputCompression::NO_COMPRESSION);
        GenericQueryResponse gqr;
        Timestamp ts(12, 12543);
        gqr.ts = &ts;

        std::size_t written = exporter->buffer_qr(gqr);
        EXPECT_EQ(written, 0);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written += exporter->write_block();
        EXPECT_GT(written, 0);
        delete exporter;

        struct stat buff;
        std::ifstream file("test.out.cdns");
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        EXPECT_EQ(written + 1, str.size());

        if (stat("test.out.cdns", &buff) == 0)
            remove("test.out.cdns");
    }

    TEST(CdnsExporterTest, CEBufferWriteAECTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, "test.out", CborOutputCompression::NO_COMPRESSION);
        GenericAddressEventCount aec;
        AddressEventTypeValues type = AddressEventTypeValues::tcp_reset;
        QueryResponseTransportFlagsMask tflags = QueryResponseTransportFlagsMask::tcp;
        std::string ip = "8.8.8.8";
        aec.ae_type = &type;
        aec.ae_transport_flags = &tflags;
        aec.ip_address = &ip;

        std::size_t written = exporter->buffer_aec(aec);
        written += exporter->buffer_aec(aec);
        EXPECT_EQ(written, 0);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written += exporter->write_block();
        EXPECT_GT(written, 0);
        delete exporter;

        struct stat buff;
        std::ifstream file("test.out.cdns");
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        EXPECT_EQ(written + 1, str.size());

        if (stat("test.out.cdns", &buff) == 0)
            remove("test.out.cdns");
    }

    TEST(CdnsExporterTest, CEBufferWriteMMTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, "test.out", CborOutputCompression::NO_COMPRESSION);
        GenericMalformedMessage mm;
        Timestamp ts(12, 12543);
        std::string ip = "8.8.8.8";
        std::string mdata = "TestMM";
        mm.ts = &ts;
        mm.client_ip = &ip;
        mm.mm_payload = &mdata;

        std::size_t written = exporter->buffer_mm(mm);
        EXPECT_EQ(written, 0);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written += exporter->write_block();
        EXPECT_GT(written, 0);
        delete exporter;

        struct stat buff;
        std::ifstream file("test.out.cdns");
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        EXPECT_EQ(written + 1, str.size());

        if (stat("test.out.cdns", &buff) == 0)
            remove("test.out.cdns");
    }

    TEST(CdnsExporterTest, CEBufferWriteMultipleBlocksTest) {
        FilePreamble fp;
        fp.m_block_parameters[0].storage_parameters.max_block_items = 2;
        BlockParameters bp;
        CdnsExporter* exporter = new CdnsExporter(fp, "test.out", CborOutputCompression::NO_COMPRESSION);
        GenericQueryResponse gqr, gqr2, gqr3;
        Timestamp ts(12, 12543), ts2(6, 3020);
        std::string ip("8.8.8.8");
        gqr.ts = &ts;
        gqr.client_ip = &ip;
        gqr2.ts = &ts2;
        gqr2.server_ip = &ip;
        gqr3.ts = &ts;
        gqr3.server_ip = &ip;

        // Add another Block parameters
        index_t index = exporter->add_block_parameters(bp);
        EXPECT_EQ(index, 1);

        // Buffer 1st QueryResponse
        std::size_t written = exporter->buffer_qr(gqr);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        EXPECT_EQ(written, 0);

        // Try to set another Block parameters
        bool ret = exporter->set_active_block_parameters(13);
        EXPECT_FALSE(ret);
        ret = exporter->set_active_block_parameters(index);
        EXPECT_TRUE(ret);

        // Buffer 2nd QueryResponse (should export full Block)
        written = exporter->buffer_qr(gqr2);
        EXPECT_EQ(exporter->get_block_item_count(), 0);
        EXPECT_GT(written, 0);

        // Buffer 3rd QueryResponse and write Block
        written += exporter->buffer_qr(gqr3);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written += exporter->write_block();
        delete exporter;

        struct stat buff;
        std::ifstream file("test.out.cdns");
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        EXPECT_EQ(written + 1, str.size());

        if (stat("test.out.cdns", &buff) == 0)
            remove("test.out.cdns");
    }

    TEST(CdnsExporterTest, CERotateTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, "test.out", CborOutputCompression::NO_COMPRESSION);
        GenericQueryResponse gqr, gqr2;
        Timestamp ts(12, 12543), ts2(6, 3020);
        std::string ip("8.8.8.8");
        gqr.ts = &ts;
        gqr.client_ip = &ip;
        gqr2.ts = &ts2;
        gqr2.server_ip = &ip;

        exporter->buffer_qr(gqr);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        std::size_t written = exporter->rotate_output("test2.out", true);

        struct stat buff;
        std::ifstream file("test.out.cdns");
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        EXPECT_EQ(written, str.size());
        if (stat("test.out.cdns", &buff) == 0)
            remove("test.out.cdns");

        exporter->buffer_qr(gqr2);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written = exporter->write_block();
        delete exporter;

        std::ifstream file2("test2.out.cdns");
        std::string str2((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());
        EXPECT_EQ(written + 1, str2.size());
        if (stat("test2.out.cdns", &buff) == 0)
            remove("test2.out.cdns");
    }
}