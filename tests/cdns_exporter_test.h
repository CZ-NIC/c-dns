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
    TEST(CdnsExporterTest, CECTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, file, CborOutputCompression::NO_COMPRESSION);

        EXPECT_EQ(exporter->get_block_item_count(), 0);
        delete exporter;

        remove_file(file);
    }

    TEST(CdnsExporterTest, CEBufferWriteQRTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, file, CborOutputCompression::NO_COMPRESSION);
        GenericQueryResponse gqr;
        Timestamp ts(12, 12543);
        std::vector<GenericResourceRecord> grr;
        ClassType classtype;
        classtype.type = 2;
        classtype.class_ = 3;

        for (int i = 0; i < 2; i++) {
            GenericResourceRecord rr;
            rr.name = "test_name";
            rr.classtype = classtype;
            rr.ttl = 128;
            rr.rdata = "test_data";
            grr.push_back(rr);
        }

        gqr.ts = ts;
        gqr.query_questions = grr;
        gqr.query_additional = grr;
        gqr.response_answers = grr;
        gqr.country_code = "CZ";
        gqr.user_id = "291a2403-735f-4c94-917a-d9eeadb374a4";

        std::size_t written = exporter->buffer_qr(gqr);
        EXPECT_EQ(written, 0);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written += exporter->write_block();
        EXPECT_GT(written, 0);
        delete exporter;

        test_size_and_remove_file(file, written + 1);
    }

    TEST(CdnsExporterTest, CEBufferWriteAECTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, file, CborOutputCompression::NO_COMPRESSION);
        GenericAddressEventCount aec;
        aec.ae_type = AddressEventTypeValues::tcp_reset;
        aec.ae_transport_flags = QueryResponseTransportFlagsMask::tcp;
        aec.ip_address = "8.8.8.8";

        std::size_t written = exporter->buffer_aec(aec);
        written += exporter->buffer_aec(aec);
        EXPECT_EQ(written, 0);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written += exporter->write_block();
        EXPECT_GT(written, 0);
        delete exporter;

        test_size_and_remove_file(file, written + 1);
    }

    TEST(CdnsExporterTest, CEBufferWriteMMTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, file, CborOutputCompression::NO_COMPRESSION);
        GenericMalformedMessage mm;
        mm.ts = Timestamp(12, 12543);
        mm.client_ip = "8.8.8.8";
        mm.mm_payload = "TestMM";

        std::size_t written = exporter->buffer_mm(mm);
        EXPECT_EQ(written, 0);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written += exporter->write_block();
        EXPECT_GT(written, 0);
        delete exporter;

        test_size_and_remove_file(file, written + 1);
    }

    TEST(CdnsExporterTest, CEBufferWriteMultipleBlocksTest) {
        FilePreamble fp;
        fp.m_block_parameters[0].storage_parameters.max_block_items = 2;
        BlockParameters bp;
        CdnsExporter* exporter = new CdnsExporter(fp, file, CborOutputCompression::NO_COMPRESSION);
        GenericQueryResponse gqr, gqr2, gqr3;
        Timestamp ts(12, 12543), ts2(6, 3020);
        std::string ip("8.8.8.8");
        gqr.ts = ts;
        gqr.client_ip = ip;
        gqr2.ts = ts2;
        gqr2.server_ip = ip;
        gqr3.ts = ts;
        gqr3.server_ip = ip;

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

        test_size_and_remove_file(file, written + 1);
    }

    TEST(CdnsExporterTest, CERotateTest) {
        FilePreamble fp;
        CdnsExporter* exporter = new CdnsExporter(fp, file, CborOutputCompression::NO_COMPRESSION);
        GenericQueryResponse gqr, gqr2;
        Timestamp ts(12, 12543), ts2(6, 3020);
        std::string ip("8.8.8.8");
        gqr.ts = ts;
        gqr.client_ip = ip;
        gqr2.ts = ts2;
        gqr2.server_ip = ip;

        exporter->buffer_qr(gqr);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        std::size_t written = exporter->rotate_output(file2, true);

        test_size_and_remove_file(file, written);

        exporter->buffer_qr(gqr2);
        EXPECT_EQ(exporter->get_block_item_count(), 1);
        written = exporter->write_block();
        delete exporter;

        test_size_and_remove_file(file2, written + 1);
    }
}
