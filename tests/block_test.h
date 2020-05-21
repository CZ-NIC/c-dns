/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "gtest/gtest.h"

#include "../src/cdns.h"
#include "common.h"

namespace CDNS {
    TEST(ClassTypeTest, ClassTypeCTest) {
        ClassType ct;

        EXPECT_EQ(ct.type, 0);
        EXPECT_EQ(ct.class_, 0);
    }

    TEST(ClassTypeTest, ClassTypeCompareTest) {
        ClassType ct, ct2, ct3;
        ct2.type = 3;

        EXPECT_NE(ct, ct2);
        EXPECT_EQ(ct, ct3);

        ClassType ct4 = ct2.key();
        EXPECT_FALSE(ct2 != ct4);
    }

    TEST(QueryResponseSignatureTest, QRSCompareTest) {
        QueryResponseSignature qrs, qrs2, qrs3;
        qrs2.server_address_index = 3;

        EXPECT_NE(qrs, qrs2);
        EXPECT_EQ(qrs, qrs3);

        QueryResponseSignature qrs4 = qrs2.key();
        EXPECT_FALSE(qrs2 != qrs4);
    }

    TEST(QuestionTest, QuestionCTest) {
        Question q;

        EXPECT_EQ(q.name_index, 0);
        EXPECT_EQ(q.classtype_index, 0);
    }

    TEST(QuestionTest, QuestionCompareTest) {
        Question q, q2, q3;
        q2.classtype_index = 3;

        EXPECT_NE(q, q2);
        EXPECT_EQ(q, q3);

        Question q4 = q2.key();
        EXPECT_FALSE(q2 != q4);
    }

    TEST(RRTest, RRCTest) {
        RR rr;

        EXPECT_EQ(rr.name_index, 0);
        EXPECT_EQ(rr.classtype_index, 0);
        EXPECT_FALSE(rr.ttl);
        EXPECT_FALSE(rr.rdata_index);
    }

    TEST(RRTest, RRCompareTest) {
        RR rr, rr2, rr3;
        rr2.ttl = 233;

        EXPECT_NE(rr, rr2);
        EXPECT_EQ(rr, rr3);

        RR rr4 = rr2.key();
        EXPECT_FALSE(rr2 != rr4);
    }

    TEST(MalformedMessageDataTest, MMDCompareTest) {
        MalformedMessageData mmd, mmd2, mmd3;
        mmd2.server_port = 53;

        EXPECT_NE(mmd, mmd2);
        EXPECT_EQ(mmd, mmd3);

        MalformedMessageData mmd4 = mmd2.key();
        EXPECT_FALSE(mmd2 != mmd4);
    }

    TEST(ResponseProcessingDataTest, RPDCTest) {
        ResponseProcessingData rpd;

        EXPECT_FALSE(rpd.bailiwick_index);
        EXPECT_FALSE(rpd.processing_flags);
    }

    TEST(QueryResponseExtendedTest, QRECTest) {
        QueryResponseExtended qre;

        EXPECT_FALSE(qre.question_index);
        EXPECT_FALSE(qre.answer_index);
        EXPECT_FALSE(qre.authority_index);
        EXPECT_FALSE(qre.additional_index);
    }

    TEST(BlockPreambleTest, BPCTest) {
        BlockPreamble bp;

        EXPECT_EQ(bp.earliest_time.m_secs, 0);
        EXPECT_EQ(bp.earliest_time.m_ticks, 0);
        EXPECT_FALSE(bp.block_parameters_index);
    }

    TEST(BlockStatisticsTest, BSCTest) {
        BlockStatistics bs;

        EXPECT_FALSE(bs.processed_messages);
        EXPECT_FALSE(bs.qr_data_items);
        EXPECT_FALSE(bs.unmatched_queries);
        EXPECT_FALSE(bs.unmatched_responses);
        EXPECT_FALSE(bs.discarded_opcode);
        EXPECT_FALSE(bs.malformed_items);
    }

    TEST(QueryResponseTest, QRCTest) {
        QueryResponse qr;

        EXPECT_FALSE(qr.time_offset);
        EXPECT_FALSE(qr.client_address_index);
        EXPECT_FALSE(qr.client_port);
        EXPECT_FALSE(qr.transaction_id);
        EXPECT_FALSE(qr.qr_signature_index);
        EXPECT_FALSE(qr.client_hoplimit);
        EXPECT_FALSE(qr.response_delay);
        EXPECT_FALSE(qr.query_name_index);
        EXPECT_FALSE(qr.query_size);
        EXPECT_FALSE(qr.response_size);
        EXPECT_FALSE(qr.response_processing_data);
        EXPECT_FALSE(qr.query_extended);
        EXPECT_FALSE(qr.response_extended);
    }

    TEST(AddressEventCountTest, AECCTest) {
        AddressEventCount aec;

        EXPECT_FALSE(aec.ae_code);
        EXPECT_FALSE(aec.ae_transport_flags);
        EXPECT_EQ(aec.ae_address_index, 0);
        EXPECT_EQ(aec.ae_count, 0);
    }

    TEST(AddressEventCountTest, AECCompareTest) {
        AddressEventCount aec, aec2, aec3;
        aec2.ae_code = 3;

        EXPECT_NE(aec, aec2);
        EXPECT_EQ(aec, aec3);

        AddressEventCount aec4 = aec2.key();
        EXPECT_FALSE(aec2 != aec4);
    }

    TEST(MalformedMessageTest, MMCTest) {
        MalformedMessage mm;

        EXPECT_FALSE(mm.time_offset);
        EXPECT_FALSE(mm.client_address_index);
        EXPECT_FALSE(mm.client_port);
        EXPECT_FALSE(mm.message_data_index);
    }

    TEST(StringItemTest, SICTest) {
        StringItem si;

        EXPECT_EQ(si.data.size(), 0);
    }

    TEST(StringItemTest, SICompareTest) {
        StringItem si, si2, si3;
        si.data = "Test";
        si2.data = "Test2";
        si3.data = "Test";

        EXPECT_NE(si, si2);
        EXPECT_EQ(si, si3);

        StringItem si4 = si2.key();
        EXPECT_FALSE(si2 != si4);
    }

    TEST(IndexListItemTest, ILICTest) {
        IndexListItem ili;

        EXPECT_EQ(ili.list.size(), 0);
    }

    TEST(IndexListItemTest, ILICompareTest) {
        IndexListItem ili, ili2, ili3;
        ili.list = {1, 2};
        ili2.list = {1, 2, 5};
        ili3.list = {1, 2};

        EXPECT_NE(ili, ili2);
        EXPECT_EQ(ili, ili3);

        IndexListItem ili4 = ili2.key();
        EXPECT_FALSE(ili2 != ili4);
    }

    TEST(BlockTest, BlockCtorTest) {
        BlockParameters bp;
        CdnsBlock block(bp, 0);

        EXPECT_EQ(block.get_block_parameters_index(), 0);
        EXPECT_EQ(block.get_item_count(), 0);
    }

    TEST(BlockTest, BlockAddGetTest) {
        BlockParameters bp;
        CdnsBlock block(bp, 0);
        std::string ip = "8.8.8.8";
        ClassType ct;
        std::string rdata = "Test";
        QueryResponseSignature qrs;
        std::vector<index_t> list = {1, 2, 5};
        Question q;
        RR rr;
        MalformedMessageData mmd;

        index_t index = block.add_ip_address(ip);
        index_t index_dup = block.add_ip_address(ip);
        index_t index2 = block.add_classtype(ct);
        index_t index2_dup = block.add_classtype(ct);
        index_t index3 = block.add_name_rdata(rdata);
        index_t index3_dup = block.add_name_rdata(rdata);
        index_t index4 = block.add_qr_signature(qrs);
        index_t index4_dup = block.add_qr_signature(qrs);
        index_t index5 = block.add_question_list(list);
        index_t index5_dup = block.add_question_list(list);
        index_t index6 = block.add_question(q);
        index_t index6_dup = block.add_question(q);
        index_t index7 = block.add_rr_list(list);
        index_t index7_dup = block.add_rr_list(list);
        index_t index8 = block.add_rr(rr);
        index_t index8_dup = block.add_rr(rr);
        index_t index9 = block.add_malformed_message_data(mmd);
        index_t index9_dup = block.add_malformed_message_data(mmd);

        EXPECT_EQ(index, 0);
        EXPECT_EQ(index, index_dup);
        EXPECT_EQ(index2, 0);
        EXPECT_EQ(index2, index2_dup);
        EXPECT_EQ(index3, 0);
        EXPECT_EQ(index3, index3_dup);
        EXPECT_EQ(index4, 0);
        EXPECT_EQ(index4, index4_dup);
        EXPECT_EQ(index5, 0);
        EXPECT_EQ(index5, index5_dup);
        EXPECT_EQ(index6, 0);
        EXPECT_EQ(index6, index6_dup);
        EXPECT_EQ(index7, 0);
        EXPECT_EQ(index7, index7_dup);
        EXPECT_EQ(index8, 0);
        EXPECT_EQ(index8, index8_dup);
        EXPECT_EQ(index9, 0);
        EXPECT_EQ(index9, index9_dup);
        EXPECT_EQ(block.get_item_count(), 0);
        EXPECT_EQ(ip, block.get_ip_address(index));
        EXPECT_EQ(ct, block.get_classtype(index2));
        EXPECT_EQ(rdata, block.get_name_rdata(index3));
        EXPECT_EQ(qrs, block.get_qr_signature(index4));
        EXPECT_EQ(list, block.get_question_list(index5));
        EXPECT_EQ(q, block.get_question(index6));
        EXPECT_EQ(list, block.get_rr_list(index7));
        EXPECT_EQ(rr, block.get_rr(index8));
        EXPECT_EQ(mmd, block.get_malformed_message_data(index9));
    }

    TEST(BlockTest, BlockAddGListTest) {
        BlockParameters bp;
        CdnsBlock block(bp, 0);
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

        index_t index = block.add_generic_qlist(grr);
        index_t index_dup = block.add_generic_qlist(grr);
        index_t index2 = block.add_generic_rrlist(grr);
        index_t index2_dup = block.add_generic_rrlist(grr);

        EXPECT_EQ(index, 0);
        EXPECT_EQ(index, index_dup);
        EXPECT_EQ(index2, 0);
        EXPECT_EQ(index2, index2_dup);

        grr.pop_back();
        index_t index3 = block.add_generic_qlist(grr);
        index_t index4 = block.add_generic_rrlist(grr);

        EXPECT_EQ(index3, 1);
        EXPECT_EQ(index4, 1);
    }

    TEST(BlockTest, BlockAddQRTest) {
        BlockParameters bp;
        CdnsBlock block(bp, 0);
        GenericQueryResponse qr;
        std::string ip = "8.8.8.8";
        uint16_t port = 53;
        qr.client_ip = ip;
        qr.server_ip = ip;
        qr.client_port = port;
        qr.server_port = port;
        qr.ts = Timestamp(13, 1234);
        qr.query_name = "Test";

        bool ret = block.add_question_response_record(qr);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 1);

        ret = block.add_question_response_record(qr);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 2);

        block.clear();
        EXPECT_EQ(block.get_item_count(), 0);
    }

    TEST(BlockTest, BlockAddAECTest) {
        BlockParameters bp;
        CdnsBlock block(bp, 0);
        GenericAddressEventCount aec;
        aec.ae_type = AddressEventTypeValues::tcp_reset;
        aec.ip_address = "8.8.8.8";

        bool ret = block.add_address_event_count(aec);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 1);

        ret = block.add_address_event_count(aec);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 1);

        aec.ae_type = AddressEventTypeValues::icmp_dest_unreachable;
        ret = block.add_address_event_count(aec);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 2);

        block.clear();
        EXPECT_EQ(block.get_item_count(), 0);
    }

    TEST(BlockTest, BlockAddMMTest) {
        BlockParameters bp;
        CdnsBlock block(bp, 0);
        GenericMalformedMessage mm;
        mm.ts = Timestamp(13, 1234);
        mm.client_ip = "8.8.8.8";
        mm.server_port = 53;
        mm.mm_payload = "TestMM";

        bool ret = block.add_malformed_message(mm);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 1);

        ret = block.add_malformed_message(mm);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 2);

        block.clear();
        EXPECT_EQ(block.get_item_count(), 0);
    }

    TEST(BlockTest, BlockAddQRAECMMTest) {
        BlockParameters bp;
        CdnsBlock block(bp, 0);
        GenericQueryResponse qr;
        GenericMalformedMessage mm;
        GenericAddressEventCount aec;
        Timestamp ts(13, 1234);
        std::string ip = "8.8.8.8";
        qr.ts = ts;
        qr.client_ip = ip;
        mm.ts = ts;
        mm.server_ip = ip;
        mm.mm_payload = "TestMM";
        aec.ae_type = AddressEventTypeValues::tcp_reset;
        aec.ip_address = ip;

        bool ret = block.add_question_response_record(qr);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 1);

        ret = block.add_malformed_message(mm);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 2);

        ret = block.add_address_event_count(aec);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 3);

        block.clear();
        EXPECT_EQ(block.get_item_count(), 0);
    }

    TEST(BlockTest, BlockSetBPTest) {
        BlockParameters bp, bp2;
        bp2.storage_parameters.max_block_items = 100;
        CdnsBlock block(bp, 0);
        GenericQueryResponse qr;
        qr.ts = Timestamp(13, 1234);

        bool ret = block.add_question_response_record(qr);
        EXPECT_FALSE(ret);
        EXPECT_EQ(block.get_item_count(), 1);

        ret = block.set_block_parameters(bp2, 1);
        EXPECT_FALSE(ret);

        block.clear();
        ret = block.set_block_parameters(bp2, 1);
        EXPECT_TRUE(ret);
    }

    TEST(BlockReadTest, BlockReadGenericQRTest) {
        CdnsBlockRead block;
        QueryResponse qr;
        qr.client_port = 1234;
        qr.time_offset = Timestamp(5, 170);

        block.add_question_response_record(qr);
        EXPECT_EQ(block.get_item_count(), 1);

        qr.query_size = 150;
        block.add_question_response_record(qr);
        EXPECT_EQ(block.get_item_count(), 2);

        bool end = false;
        GenericQueryResponse gqr = block.read_generic_qr(end);
        EXPECT_FALSE(end);
        EXPECT_EQ(*gqr.client_port, 1234);
        EXPECT_EQ(gqr.ts->m_secs, 5);
        EXPECT_EQ(gqr.ts->m_ticks, 170);
        EXPECT_FALSE(gqr.query_size);

        gqr = block.read_generic_qr(end);
        EXPECT_FALSE(end);
        EXPECT_EQ(*gqr.client_port, 1234);
        EXPECT_EQ(gqr.ts->m_secs, 5);
        EXPECT_EQ(gqr.ts->m_ticks, 170);
        EXPECT_EQ(*gqr.query_size, 150);

        gqr = block.read_generic_qr(end);
        EXPECT_TRUE(end);
        EXPECT_FALSE(gqr.client_port);
        EXPECT_FALSE(gqr.ts);
        EXPECT_FALSE(gqr.query_size);
    }

    TEST(BlockReadTest, BlockReadGenericAECTest) {
        CdnsBlock block;
        AddressEventCount aec;
        aec.ae_type = AddressEventTypeValues::icmp_dest_unreachable;
        aec.ae_count = 1;
        std::string ip = "8.8.8.8";
        aec.ae_address_index = block.add_ip_address(ip);

        block.add_address_event_count(aec);
        EXPECT_EQ(block.get_item_count(), 1);
        block.add_address_event_count(aec);
        EXPECT_EQ(block.get_item_count(), 1);
        aec.ae_type = AddressEventTypeValues::icmpv6_packet_too_big;
        block.add_address_event_count(aec);
        EXPECT_EQ(block.get_item_count(), 2);

        CdnsEncoder* enc = new CdnsEncoder(file, CborOutputCompression::NO_COMPRESSION);
        block.write(*enc);
        delete enc;
        std::ifstream ifs(file, std::ifstream::binary);
        CdnsDecoder dec(ifs);
        std::vector<BlockParameters> bps = {BlockParameters()};
        CdnsBlockRead block_read(dec, bps);

        bool end = false;
        GenericAddressEventCount gaec = block_read.read_generic_aec(end);
        EXPECT_FALSE(end);
        EXPECT_EQ(gaec.ae_type, AddressEventTypeValues::icmp_dest_unreachable);
        EXPECT_EQ(gaec.ae_count, 2);
        EXPECT_EQ(gaec.ip_address, "8.8.8.8");

        gaec = block_read.read_generic_aec(end);
        EXPECT_FALSE(end);
        EXPECT_EQ(gaec.ae_type, AddressEventTypeValues::icmpv6_packet_too_big);
        EXPECT_EQ(gaec.ae_count, 1);
        EXPECT_EQ(gaec.ip_address, "8.8.8.8");

        gaec = block_read.read_generic_aec(end);
        EXPECT_TRUE(end);
        EXPECT_EQ(gaec.ae_type, AddressEventTypeValues::tcp_reset);
        EXPECT_EQ(gaec.ae_count, 0);
        EXPECT_EQ(gaec.ip_address, "");

        remove_file(file);
    }

    TEST(BlockReadTest, BlockReadGenericMMTest) {
        CdnsBlockRead block;
        MalformedMessage mm;
        mm.client_port = 1234;
        mm.time_offset = Timestamp(5, 170);

        block.add_malformed_message(mm);
        EXPECT_EQ(block.get_item_count(), 1);

        std::string ip("8.8.8.8");
        mm.client_address_index = block.add_ip_address(ip);
        block.add_malformed_message(mm);
        EXPECT_EQ(block.get_item_count(), 2);

        bool end = false;
        GenericMalformedMessage gmm = block.read_generic_mm(end);
        EXPECT_FALSE(end);
        EXPECT_EQ(*gmm.client_port, 1234);
        EXPECT_EQ(gmm.ts->m_secs, 5);
        EXPECT_EQ(gmm.ts->m_ticks, 170);
        EXPECT_FALSE(gmm.client_ip);

        gmm = block.read_generic_mm(end);
        EXPECT_FALSE(end);
        EXPECT_EQ(*gmm.client_port, 1234);
        EXPECT_EQ(gmm.ts->m_secs, 5);
        EXPECT_EQ(gmm.ts->m_ticks, 170);
        EXPECT_EQ(*gmm.client_ip, ip);

        gmm = block.read_generic_mm(end);
        EXPECT_TRUE(end);
        EXPECT_FALSE(gmm.client_port);
        EXPECT_FALSE(gmm.ts);
        EXPECT_FALSE(gmm.client_ip);
    }
}