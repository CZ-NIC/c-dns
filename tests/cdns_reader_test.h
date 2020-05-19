/**
 * Copyright © 2020 CZ.NIC, z. s. p. o.
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
#include "gtest/gtest.h"

#include "../src/cdns.h"
#include "common.h"

namespace CDNS {
    void create_test_file() {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, file, CborOutputCompression::NO_COMPRESSION);
        Timestamp ts(12, 1234), ts2(13, 321);
        std::string ip("8.8.8.8");

        GenericQueryResponse gqr;
        gqr.ts = ts;
        gqr.client_ip = ip;
        gqr.query_ancount = 42;
        exporter->buffer_qr(gqr);
        gqr.ts = ts2;
        exporter->buffer_qr(gqr);

        GenericMalformedMessage gmm;
        gmm.ts = ts;
        gmm.client_ip = ip;
        exporter->buffer_mm(gmm);

        GenericAddressEventCount gaec;
        gaec.ae_type = AddressEventTypeValues::icmp_time_exceeded;
        gaec.ip_address = ip;
        gaec.ae_count = 1;
        exporter->buffer_aec(gaec);
        exporter->buffer_aec(gaec);
        gaec.ae_type = AddressEventTypeValues::icmpv6_packet_too_big;
        exporter->buffer_aec(gaec);

        EXPECT_EQ(exporter->get_block_item_count(), 5);
        exporter->write_block();

        exporter->buffer_qr(gqr);
        exporter->buffer_aec(gaec);
        EXPECT_EQ(exporter->get_block_item_count(), 2);
        exporter->write_block();

        delete exporter;
    }

    TEST(CdnsReaderTest, CRCTest) {
        create_test_file();
        std::ifstream ifs(file);
        std::istream& is = ifs;
        CdnsReader reader(is);

        EXPECT_EQ(reader.m_file_preamble.block_parameters_size(), 1);
        EXPECT_EQ(reader.m_file_preamble.m_block_parameters[0].storage_parameters.max_block_items, 10000);

        ifs.close();
        remove_file(file);
    }

    TEST(CdnsReaderTest, CRReadBlockTest) {
        create_test_file();
        std::ifstream ifs(file);
        std::istream& is = ifs;
        CdnsReader reader(is);

        // Read first block
        bool eof = false;
        CdnsBlockRead block = reader.read_block(eof);
        ASSERT_FALSE(eof);

        // Read QueryResponses
        GenericQueryResponse gqr = block.read_generic_qr(eof);
        ASSERT_FALSE(eof);
        EXPECT_EQ(gqr.ts->m_secs, 12);
        EXPECT_EQ(gqr.ts->m_ticks, 1234);
        EXPECT_EQ(*gqr.client_ip, "8.8.8.8");
        EXPECT_EQ(*gqr.query_ancount, 42);

        gqr = block.read_generic_qr(eof);
        ASSERT_FALSE(eof);
        EXPECT_EQ(gqr.ts->m_secs, 13);
        EXPECT_EQ(gqr.ts->m_ticks, 321);
        EXPECT_EQ(*gqr.client_ip, "8.8.8.8");
        EXPECT_EQ(*gqr.query_ancount, 42);

        gqr = block.read_generic_qr(eof);
        ASSERT_TRUE(eof);
        gqr = block.read_generic_qr(eof);
        ASSERT_TRUE(eof);

        // Read Malformed messages
        GenericMalformedMessage gmm = block.read_generic_mm(eof);
        ASSERT_FALSE(eof);
        EXPECT_EQ(gmm.ts->m_secs, 12);
        EXPECT_EQ(gmm.ts->m_ticks, 1234);
        EXPECT_EQ(*gmm.client_ip, "8.8.8.8");

        gmm = block.read_generic_mm(eof);
        ASSERT_TRUE(eof);
        gmm = block.read_generic_mm(eof);
        ASSERT_TRUE(eof);

        // Read Address event counts
        GenericAddressEventCount gaec = block.read_generic_aec(eof);
        ASSERT_FALSE(eof);
        EXPECT_EQ(gaec.ae_type, AddressEventTypeValues::icmp_time_exceeded);
        EXPECT_EQ(gaec.ip_address, "8.8.8.8");
        EXPECT_EQ(gaec.ae_count, 2);

        gaec = block.read_generic_aec(eof);
        ASSERT_FALSE(eof);
        EXPECT_EQ(gaec.ae_type, AddressEventTypeValues::icmpv6_packet_too_big);
        EXPECT_EQ(gaec.ip_address, "8.8.8.8");
        EXPECT_EQ(gaec.ae_count, 1);

        gaec = block.read_generic_aec(eof);
        ASSERT_TRUE(eof);
        gaec = block.read_generic_aec(eof);
        ASSERT_TRUE(eof);

        // Read second block
        block = reader.read_block(eof);
        ASSERT_FALSE(eof);

        // Read QueryResponses
        gqr = block.read_generic_qr(eof);
        ASSERT_FALSE(eof);
        EXPECT_EQ(gqr.ts->m_secs, 13);
        EXPECT_EQ(gqr.ts->m_ticks, 321);
        EXPECT_EQ(*gqr.client_ip, "8.8.8.8");
        EXPECT_EQ(*gqr.query_ancount, 42);

        gqr = block.read_generic_qr(eof);
        ASSERT_TRUE(eof);
        gqr = block.read_generic_qr(eof);
        ASSERT_TRUE(eof);

        // Read Address event counts
        gaec = block.read_generic_aec(eof);
        ASSERT_FALSE(eof);
        EXPECT_EQ(gaec.ae_type, AddressEventTypeValues::icmpv6_packet_too_big);
        EXPECT_EQ(gaec.ip_address, "8.8.8.8");
        EXPECT_EQ(gaec.ae_count, 1);

        gaec = block.read_generic_aec(eof);
        ASSERT_TRUE(eof);
        gaec = block.read_generic_aec(eof);
        ASSERT_TRUE(eof);

        // Check for end of file
        block = reader.read_block(eof);
        ASSERT_TRUE(eof);
        block = reader.read_block(eof);
        ASSERT_TRUE(eof);

        ifs.close();
        remove_file(file);
    }
}
