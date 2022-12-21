#!/usr/bin/env python3

import os
import unittest

import pycdns
import common

class TestCdnsExporter(unittest.TestCase):

    def test_ce_ctest(self):
        fp = pycdns.FilePreamble()
        exporter = pycdns.CdnsExporter(fp, common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        self.assertEqual(exporter.get_block_item_count(), 0)
        del exporter

        os.remove(common.file)

    def test_ce_buffer_write_qr(self):
        fp = pycdns.FilePreamble()
        exporter = pycdns.CdnsExporter(fp, common.file, pycdns.CborOutputCompression.NO_COMPRESSION)
        gqr = pycdns.GenericQueryResponse()
        ts = pycdns.Timestamp(12, 12543)
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

        gqr.ts = ts
        gqr.query_questions = grr
        gqr.query_additional = grr
        gqr.response_answers = grr
        gqr.country_code = "CZ"

        written = exporter.buffer_qr(gqr)
        self.assertEqual(written, 0)
        self.assertEqual(exporter.get_block_item_count(), 1)
        written += exporter.write_block()
        self.assertGreater(written, 0)
        del exporter

        common.test_size_and_remove_file(self, common.file, written + 1)

    def test_ce_buffer_write_aec(self):
        fp = pycdns.FilePreamble()
        exporter = pycdns.CdnsExporter(fp, common.file, pycdns.CborOutputCompression.NO_COMPRESSION)
        aec = pycdns.GenericAddressEventCount()
        aec.ae_type = pycdns.AddressEventTypeValues.tcp_reset
        aec.ae_transport_flags = pycdns.QueryResponseTransportFlagsMask.tcp
        aec.ip_address = "8.8.8.8"

        written = exporter.buffer_aec(aec)
        written += exporter.buffer_aec(aec, None)
        self.assertEqual(written, 0)
        self.assertEqual(exporter.get_block_item_count(), 1)
        written += exporter.write_block()
        self.assertGreater(written, 0)
        del exporter

        common.test_size_and_remove_file(self, common.file, written + 1)

    def test_ce_buffer_write_mm(self):
        fp = pycdns.FilePreamble()
        exporter = pycdns.CdnsExporter(fp, common.file, pycdns.CborOutputCompression.NO_COMPRESSION)
        mm = pycdns.GenericMalformedMessage()
        mm.ts = pycdns.Timestamp(12, 12543)
        mm.client_ip = "8.8.8.8"
        mm.mm_payload = "TestMM"

        written = exporter.buffer_mm(mm)
        self.assertEqual(written, 0)
        self.assertEqual(exporter.get_block_item_count(), 1)
        written += exporter.write_block()
        self.assertGreater(written, 0)
        del exporter

        common.test_size_and_remove_file(self, common.file, written + 1)

    def test_ce_buffer_write_multiple_blocks(self):
        fp = pycdns.FilePreamble()
        fp.m_block_parameters[0].storage_parameters.max_block_items = 2
        bp = pycdns.BlockParameters()
        exporter = pycdns.CdnsExporter(fp, common.file, pycdns.CborOutputCompression.NO_COMPRESSION)
        gqr = pycdns.GenericQueryResponse()
        gqr2 = pycdns.GenericQueryResponse()
        gqr3 = pycdns.GenericQueryResponse()
        ts = pycdns.Timestamp(12, 12543)
        ts2 = pycdns.Timestamp(6, 3020)
        ip = "8.8.8.8"
        gqr.ts = ts
        gqr.client_ip = ip
        gqr2.ts = ts2
        gqr2.server_ip = ip
        gqr3.ts = ts
        gqr3.server_ip = ip

        # Add another Block parameters
        index = exporter.add_block_parameters(bp)
        self.assertEqual(index, 1)

        # Buffer 1st QueryResponse
        written = exporter.buffer_qr(gqr)
        self.assertEqual(exporter.get_block_item_count(), 1)
        self.assertEqual(written, 0)

        # Try to set another Block parameters
        ret = exporter.set_active_block_parameters(13)
        self.assertFalse(ret)
        ret = exporter.set_active_block_parameters(index)
        self.assertTrue(ret)

        # Buffer 2nd QueryResponse (should export full block)
        written = exporter.buffer_qr(gqr2)
        self.assertEqual(exporter.get_block_item_count(), 0)
        self.assertGreater(written, 0)

        # Buffer 3rd QueryResponse and write Block
        written += exporter.buffer_qr(gqr3)
        self.assertEqual(exporter.get_block_item_count(), 1)
        written += exporter.write_block()
        del exporter

        common.test_size_and_remove_file(self, common.file, written + 1)

    def test_ce_rotate_test(self):
        fp = pycdns.FilePreamble()
        exporter = pycdns.CdnsExporter(fp, common.file, pycdns.CborOutputCompression.NO_COMPRESSION)
        gqr = pycdns.GenericQueryResponse()
        gqr2 = pycdns.GenericQueryResponse()
        ts = pycdns.Timestamp(12, 12543)
        ts2 = pycdns.Timestamp(6, 3020)
        ip = "8.8.8.8"
        gqr.ts = ts
        gqr.client_ip = ip
        gqr2.ts = ts2
        gqr2.server_ip = ip

        exporter.buffer_qr(gqr)
        self.assertEqual(exporter.get_block_item_count(), 1)
        written = exporter.rotate_output(common.file2, True)

        common.test_size_and_remove_file(self, common.file, written)

        exporter.buffer_qr(gqr2)
        self.assertEqual(exporter.get_block_item_count(), 1)
        written = exporter.write_block()
        del exporter

        common.test_size_and_remove_file(self, common.file2, written + 1)
