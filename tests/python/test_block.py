#!/usr/bin/env python3

import os
import unittest

import pycdns
import common

class TestBlock(unittest.TestCase):
    def test_classtype_ctest(self):
        ct = pycdns.ClassType()

        self.assertEqual(ct.type, 0)
        self.assertEqual(ct.class_, 0)

    def test_classtype_compare(self):
        ct = pycdns.ClassType()
        ct2 = pycdns.ClassType()
        ct3 = pycdns.ClassType()

        ct2.type = 3

        self.assertNotEqual(ct, ct2)
        self.assertEqual(ct, ct3)

        ct4 = ct2.key()
        self.assertFalse(ct2 != ct4)

    def test_qrs_compare(self):
        qrs = pycdns.QueryResponseSignature()
        qrs2 = pycdns.QueryResponseSignature()
        qrs3 = pycdns.QueryResponseSignature()

        qrs2.server_address_index = 3

        self.assertNotEqual(qrs, qrs2)
        self.assertEqual(qrs, qrs3)

        qrs4 = qrs2.key()
        self.assertFalse(qrs2 != qrs4)

    def test_question_ctest(self):
        q = pycdns.Question()

        self.assertEqual(q.name_index, 0)
        self.assertEqual(q.classtype_index, 0)

    def test_question_compare(self):
        q = pycdns.Question()
        q2 = pycdns.Question()
        q3 = pycdns.Question()

        q2.classtype_index = 3

        self.assertNotEqual(q, q2)
        self.assertEqual(q, q3)

        q4 = q2.key()
        self.assertFalse(q2 != q4)

    def test_rr_ctest(self):
        rr = pycdns.RR()

        self.assertEqual(rr.name_index, 0)
        self.assertEqual(rr.classtype_index, 0)
        self.assertFalse(rr.ttl)
        self.assertFalse(rr.rdata_index)

    def test_rr_compare(self):
        rr = pycdns.RR()
        rr2 = pycdns.RR()
        rr3 = pycdns.RR()

        rr2.ttl = 233

        self.assertNotEqual(rr, rr2)
        self.assertEqual(rr, rr3)

        rr4 = rr2.key()
        self.assertFalse(rr2 != rr4)

    def test_mmd_compare(self):
        mmd = pycdns.MalformedMessageData()
        mmd2 = pycdns.MalformedMessageData()
        mmd3 = pycdns.MalformedMessageData()

        mmd2.server_port = 53

        self.assertNotEqual(mmd, mmd2)
        self.assertEqual(mmd, mmd3)

        mmd4 = mmd2.key()
        self.assertFalse(mmd2 != mmd4)

    def test_rpd_ctest(self):
        rpd = pycdns.ResponseProcessingData()

        self.assertFalse(rpd.bailiwick_index)
        self.assertFalse(rpd.processing_flags)

    def test_rpd_write(self):
        rpd = pycdns.ResponseProcessingData()
        rpd.bailiwick_index = 1234
        rpd.processing_flags = pycdns.ResponseProcessingFlagsMask.from_cache
        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        written = rpd.write(enc)
        del enc

        common.test_size_and_remove_file(self, common.file, written)

    def test_qre_ctest(self):
        qre = pycdns.QueryResponseExtended()

        self.assertFalse(qre.question_index)
        self.assertFalse(qre.answer_index)
        self.assertFalse(qre.authority_index)
        self.assertFalse(qre.additional_index)

    def test_bp_ctest(self):
        bp = pycdns.BlockPreamble()

        self.assertEqual(bp.earliest_time.m_secs, 0)
        self.assertEqual(bp.earliest_time.m_ticks, 0)
        self.assertFalse(bp.block_parameters_index)

    def test_bs_ctest(self):
        bs = pycdns.BlockStatistics()

        self.assertFalse(bs.processed_messages)
        self.assertFalse(bs.qr_data_items)
        self.assertFalse(bs.unmatched_queries)
        self.assertFalse(bs.unmatched_responses)
        self.assertFalse(bs.discarded_opcode)
        self.assertFalse(bs.malformed_items)

    def test_qr_ctest(self):
        qr = pycdns.QueryResponse()

        self.assertFalse(qr.time_offset)
        self.assertFalse(qr.client_address_index)
        self.assertFalse(qr.client_port)
        self.assertFalse(qr.transaction_id)
        self.assertFalse(qr.qr_signature_index)
        self.assertFalse(qr.client_hoplimit)
        self.assertFalse(qr.response_delay)
        self.assertFalse(qr.query_name_index)
        self.assertFalse(qr.query_size)
        self.assertFalse(qr.response_size)
        self.assertFalse(qr.response_processing_data)
        self.assertFalse(qr.query_extended)
        self.assertFalse(qr.response_extended)
        self.assertFalse(qr.asn)
        self.assertFalse(qr.country_code)
        self.assertFalse(qr.round_trip_time)

    def test_aec_ctest(self):
        aec = pycdns.AddressEventCount()

        self.assertFalse(aec.ae_code)
        self.assertFalse(aec.ae_transport_flags)
        self.assertEqual(aec.ae_address_index, 0)
        self.assertEqual(aec.ae_count, 0)

    def test_aec_compare(self):
        aec = pycdns.AddressEventCount()
        aec2 = pycdns.AddressEventCount()
        aec3 = pycdns.AddressEventCount()

        aec2.ae_code = 3

        self.assertNotEqual(aec, aec2)
        self.assertEqual(aec, aec3)

        aec4 = aec2.key()
        self.assertFalse(aec2 != aec4)

    def test_mm_ctest(self):
        mm = pycdns.MalformedMessage()

        self.assertFalse(mm.time_offset)
        self.assertFalse(mm.client_address_index)
        self.assertFalse(mm.client_port)
        self.assertFalse(mm.message_data_index)

    def test_si_ctest(self):
        si = pycdns.StringItem()

        self.assertEqual(len(si.data), 0)

    def test_si_compare(self):
        si = pycdns.StringItem()
        si2 = pycdns.StringItem()
        si3 = pycdns.StringItem()

        si.data = "Test"
        si2.data = "Test2"
        si3.data = "Test"

        self.assertNotEqual(si, si2)
        self.assertEqual(si, si3)

        si4 = si2.key()
        self.assertFalse(si2 != si4)

    def test_ili_ctest(self):
        ili = pycdns.IndexListItem()

        self.assertEqual(len(ili.list), 0)

    def test_ili_compare(self):
        ili = pycdns.IndexListItem()
        ili2 = pycdns.IndexListItem()
        ili3 = pycdns.IndexListItem()

        ili.list = [1, 2]
        ili2.list = [1, 2, 5]
        ili3.list = [1, 2]

        self.assertNotEqual(ili, ili2)
        self.assertEqual(ili, ili3)

        ili4 = ili2.key()
        self.assertFalse(ili2 != ili4)

    def test_block_ctor(self):
        bp = pycdns.BlockParameters()
        block = pycdns.CdnsBlock(bp, 0)

        self.assertEqual(block.get_block_parameters_index(), 0)
        self.assertEqual(block.get_item_count(), 0)

    def test_block_add_get(self):
        bp = pycdns.BlockParameters()
        block = pycdns.CdnsBlock(bp, 0)
        ip = "8.8.8.8"
        ct = pycdns.ClassType()
        rdata = "Test"
        qrs = pycdns.QueryResponseSignature()
        list_ = [1, 2, 5]
        q = pycdns.Question()
        rr = pycdns.RR()
        mmd = pycdns.MalformedMessageData()

        index = block.add_ip_address(ip)
        index_dup = block.add_ip_address(ip)
        index2 = block.add_classtype(ct)
        index2_dup = block.add_classtype(ct)
        index3 = block.add_name_rdata(rdata)
        index3_dup = block.add_name_rdata(rdata)
        index4 = block.add_qr_signature(qrs)
        index4_dup = block.add_qr_signature(qrs)
        index5 = block.add_question_list(list_)
        index5_dup = block.add_question_list(list_)
        index6 = block.add_question(q)
        index6_dup = block.add_question(q)
        index7 = block.add_rr_list(list_)
        index7_dup = block.add_rr_list(list_)
        index8 = block.add_rr(rr)
        index8_dup = block.add_rr(rr)
        index9 = block.add_malformed_message_data(mmd)
        index9_dup = block.add_malformed_message_data(mmd)

        self.assertEqual(index, 0)
        self.assertEqual(index, index_dup)
        self.assertEqual(index2, 0)
        self.assertEqual(index2, index2_dup)
        self.assertEqual(index3, 0)
        self.assertEqual(index3, index3_dup)
        self.assertEqual(index4, 0)
        self.assertEqual(index4, index4_dup)
        self.assertEqual(index5, 0)
        self.assertEqual(index5, index5_dup)
        self.assertEqual(index6, 0)
        self.assertEqual(index6, index6_dup)
        self.assertEqual(index7, 0)
        self.assertEqual(index7, index7_dup)
        self.assertEqual(index8, 0)
        self.assertEqual(index8, index8_dup)
        self.assertEqual(index9, 0)
        self.assertEqual(index9, index9_dup)
        self.assertEqual(block.get_item_count(), 0)
        self.assertEqual(ip, block.get_ip_address(index))
        self.assertEqual(ct, block.get_classtype(index2))
        self.assertEqual(rdata, block.get_name_rdata(index3))
        self.assertEqual(qrs, block.get_qr_signature(index4))
        self.assertEqual(list_, block.get_question_list(index5))
        self.assertEqual(q, block.get_question(index6))
        self.assertEqual(list_, block.get_rr_list(index7))
        self.assertEqual(rr, block.get_rr(index8))
        self.assertEqual(mmd, block.get_malformed_message_data(index9))

    def test_block_add_glist(self):
        bp = pycdns.BlockParameters()
        block = pycdns.CdnsBlock(bp, 0)
        grr = []
        classtype = pycdns.ClassType()
        classtype.type = 2
        classtype.class_ = 3

        for i in range(0, 2):
            rr = pycdns.GenericResourceRecord()
            rr.name = "test_name"
            rr.classtype = classtype
            rr.ttl = 128
            rr.rdata = "test_data"
            grr.append(rr)

        index = block.add_generic_qlist(grr)
        index_dup = block.add_generic_qlist(grr)
        index2 = block.add_generic_rrlist(grr)
        index2_dup = block.add_generic_rrlist(grr)

        self.assertEqual(index, 0)
        self.assertEqual(index, index_dup)
        self.assertEqual(index2, 0)
        self.assertEqual(index2, index2_dup)

        grr.pop()
        index3 = block.add_generic_qlist(grr)
        index4 = block.add_generic_rrlist(grr)

        self.assertEqual(index3, 1)
        self.assertEqual(index4, 1)

    def test_add_qr(self):
        bp = pycdns.BlockParameters()
        block = pycdns.CdnsBlock(bp, 0)
        qr = pycdns.GenericQueryResponse()
        ip = "8.8.8.8"
        port = 53
        qr.client_ip = ip
        qr.server_ip = ip
        qr.client_port = port
        qr.server_port = port
        qr.ts = pycdns.Timestamp(13, 1234)
        qr.query_name = "Test"

        ret = block.add_question_response_record(qr)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 1)

        ret = block.add_question_response_record(qr)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 2)

        block.clear()
        self.assertEqual(block.get_item_count(), 0)

    def test_add_aec(self):
        bp = pycdns.BlockParameters()
        block = pycdns.CdnsBlock(bp, 0)
        aec = pycdns.GenericAddressEventCount()
        aec.ae_type = pycdns.AddressEventTypeValues.tcp_reset
        aec.ip_address = "8.8.8.8"

        ret = block.add_address_event_count(aec)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 1)

        ret = block.add_address_event_count(aec)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 1)

        aec.ae_type = pycdns.AddressEventTypeValues.icmp_dest_unreachable
        ret = block.add_address_event_count(aec)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 2)

        block.clear()
        self.assertEqual(block.get_item_count(), 0)

    def test_add_mm(self):
        bp = pycdns.BlockParameters()
        block = pycdns.CdnsBlock(bp, 0)
        mm = pycdns.GenericMalformedMessage()
        mm.ts = pycdns.Timestamp(13, 1234)
        mm.client_ip = "8.8.8.8"
        mm.server_port = 53
        mm.mm_payload = "TestMM"

        ret = block.add_malformed_message(mm)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 1)

        ret = block.add_malformed_message(mm)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 2)

        block.clear()
        self.assertEqual(block.get_item_count(), 0)

    def test_add_qr_aec_mm(self):
        bp = pycdns.BlockParameters()
        block = pycdns.CdnsBlock(bp, 0)
        qr = pycdns.GenericQueryResponse()
        mm = pycdns.GenericMalformedMessage()
        aec = pycdns.GenericAddressEventCount()
        ts = pycdns.Timestamp(13, 1234)
        ip = "8.8.8.8"
        qr.ts = ts
        qr.client_ip = ip
        mm.ts = ts
        mm.server_ip = ip
        mm.mm_payload = "TestMM"
        aec.ae_type = pycdns.AddressEventTypeValues.tcp_reset
        aec.ip_address = ip

        ret = block.add_question_response_record(qr)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 1)

        ret = block.add_malformed_message(mm)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 2)

        ret = block.add_address_event_count(aec)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 3)

        block.clear()
        self.assertEqual(block.get_item_count(), 0)

    def test_block_set_bp(self):
        bp = pycdns.BlockParameters()
        bp2 = pycdns.BlockParameters()
        bp2.storage_parameters.max_block_items = 100
        block = pycdns.CdnsBlock(bp, 0)
        qr = pycdns.GenericQueryResponse()
        qr.ts = pycdns.Timestamp(13, 1234)

        ret = block.add_question_response_record(qr)
        self.assertFalse(ret)
        self.assertEqual(block.get_item_count(), 1)

        ret = block.set_block_parameters(bp2, 1)
        self.assertFalse(ret)

        block.clear()
        ret = block.set_block_parameters(bp2, 1)
        self.assertTrue(ret)

    def test_block_read_generic_qr(self):
        block = pycdns.CdnsBlockRead()
        qr = pycdns.QueryResponse()
        qr.client_port = 1234
        qr.time_offset = pycdns.Timestamp(5, 170)
        qr.asn = "1234"
        qr.round_trip_time = 1234

        block.add_question_response_record(qr)
        self.assertEqual(block.get_item_count(), 1)

        qr.query_size = 150
        block.add_question_response_record(qr)
        self.assertEqual(block.get_item_count(), 2)

        gqr, end = block.read_generic_qr()
        self.assertFalse(end)
        self.assertEqual(gqr.client_port, 1234)
        self.assertEqual(gqr.ts.m_secs, 5)
        self.assertEqual(gqr.ts.m_ticks, 170)
        self.assertEqual(gqr.asn, "1234")
        self.assertEqual(gqr.round_trip_time, 1234)
        self.assertFalse(gqr.query_size)

        gqr, end = block.read_generic_qr()
        self.assertFalse(end)
        self.assertEqual(gqr.client_port, 1234)
        self.assertEqual(gqr.ts.m_secs, 5)
        self.assertEqual(gqr.ts.m_ticks, 170)
        self.assertEqual(gqr.query_size, 150)
        self.assertEqual(gqr.asn, "1234")
        self.assertEqual(gqr.round_trip_time, 1234)

        gqr, end = block.read_generic_qr()
        self.assertTrue(end)
        self.assertFalse(gqr.client_port)
        self.assertFalse(gqr.ts)
        self.assertFalse(gqr.query_size)
        self.assertFalse(gqr.asn)
        self.assertFalse(gqr.round_trip_time)

    def test_block_read_generic_aec(self):
        block = pycdns.CdnsBlock()
        aec = pycdns.AddressEventCount()
        aec.ae_type = pycdns.AddressEventTypeValues.icmp_dest_unreachable
        aec.ae_count = 1
        ip = "8.8.8.8"
        aec.ae_address_index = block.add_ip_address(ip)

        block.add_address_event_count(aec)
        self.assertEqual(block.get_item_count(), 1)
        block.add_address_event_count(aec)
        self.assertEqual(block.get_item_count(), 1)
        aec.ae_type = pycdns.AddressEventTypeValues.icmpv6_packet_too_big
        block.add_address_event_count(aec)
        self.assertEqual(block.get_item_count(), 2)

        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)
        block.write(enc)
        del enc
        ifs = pycdns.Ifstream(common.file)
        dec = pycdns.CdnsDecoder(ifs)
        bps = [pycdns.BlockParameters()]
        block_read = pycdns.CdnsBlockRead(dec, bps)

        gaec, end = block_read.read_generic_aec()
        self.assertFalse(end)
        self.assertEqual(gaec.ae_type, pycdns.AddressEventTypeValues.icmp_dest_unreachable)
        self.assertEqual(gaec.ae_count, 2)
        self.assertEqual(gaec.ip_address, "8.8.8.8")

        gaec, end = block_read.read_generic_aec()
        self.assertFalse(end)
        self.assertEqual(gaec.ae_type, pycdns.AddressEventTypeValues.icmpv6_packet_too_big)
        self.assertEqual(gaec.ae_count, 1)
        self.assertEqual(gaec.ip_address, "8.8.8.8")

        gaec, end = block_read.read_generic_aec()
        self.assertTrue(end)
        self.assertEqual(gaec.ae_type, pycdns.AddressEventTypeValues.tcp_reset)
        self.assertEqual(gaec.ae_count, 0)
        self.assertEqual(gaec.ip_address, "")

        os.remove(common.file)

    def test_block_read_generic_mm(self):
        block = pycdns.CdnsBlockRead()
        mm = pycdns.MalformedMessage()
        mm.client_port = 1234
        mm.time_offset = pycdns.Timestamp(5, 170)

        block.add_malformed_message(mm)
        self.assertEqual(block.get_item_count(), 1)

        ip = "8.8.8.8"
        mm.client_address_index = block.add_ip_address(ip)
        block.add_malformed_message(mm)
        self.assertEqual(block.get_item_count(), 2)

        gmm, end = block.read_generic_mm()
        self.assertFalse(end)
        self.assertEqual(gmm.client_port, 1234)
        self.assertEqual(gmm.ts.m_secs, 5)
        self.assertEqual(gmm.ts.m_ticks, 170)
        self.assertFalse(gmm.client_ip)

        gmm, end = block.read_generic_mm()
        self.assertFalse(end)
        self.assertEqual(gmm.client_port, 1234)
        self.assertEqual(gmm.ts.m_secs, 5)
        self.assertEqual(gmm.ts.m_ticks, 170)
        self.assertEqual(gmm.client_ip, ip)

        gmm, end = block.read_generic_mm()
        self.assertTrue(end)
        self.assertFalse(gmm.client_port)
        self.assertFalse(gmm.ts)
        self.assertFalse(gmm.client_ip)
