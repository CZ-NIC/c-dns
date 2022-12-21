#!/usr/bin/env python3

import os
import unittest

import pycdns
import common

class TestCdnsReader(unittest.TestCase):

    def create_test_file(self):
        fp = pycdns.FilePreamble()
        exporter = pycdns.CdnsExporter(fp, common.file, pycdns.CborOutputCompression.NO_COMPRESSION)
        ts = pycdns.Timestamp(12, 1234)
        ts2 = pycdns.Timestamp(13, 321)
        ip = "8.8.8.8"
        cc = "CZ"
        asn = "1234"
        asn2 = "5678"
        rtt = 42

        gqr = pycdns.GenericQueryResponse()
        gqr.ts = ts
        gqr.client_ip = ip
        gqr.query_ancount = 42
        gqr.asn = asn
        gqr.country_code = cc
        exporter.buffer_qr(gqr)
        gqr.ts = ts2
        gqr.asn = asn2
        gqr.round_trip_time = rtt
        exporter.buffer_qr(gqr)

        gmm = pycdns.GenericMalformedMessage()
        gmm.ts = ts
        gmm.client_ip = ip
        exporter.buffer_mm(gmm)

        gaec = pycdns.GenericAddressEventCount()
        gaec.ae_type = pycdns.AddressEventTypeValues.icmp_time_exceeded
        gaec.ip_address = ip
        gaec.ae_count = 1
        exporter.buffer_aec(gaec)
        exporter.buffer_aec(gaec)
        gaec.ae_type = pycdns.AddressEventTypeValues.icmpv6_packet_too_big
        exporter.buffer_aec(gaec)

        self.assertEqual(exporter.get_block_item_count(), 5)
        exporter.write_block()

        exporter.buffer_qr(gqr)
        exporter.buffer_aec(gaec)
        self.assertEqual(exporter.get_block_item_count(), 2)
        exporter.write_block()

        del exporter

    def test_cr_ctest(self):
        self.create_test_file()
        ifs = pycdns.Ifstream(common.file)
        reader = pycdns.CdnsReader(ifs)

        self.assertEqual(reader.m_file_preamble.block_parameters_size(), 1)
        self.assertEqual(reader.m_file_preamble.m_block_parameters[0].storage_parameters.max_block_items, 10000)

        del ifs
        os.remove(common.file)

    def test_cr_read_block(self):
        self.create_test_file()
        ifs = pycdns.Ifstream(common.file)
        reader = pycdns.CdnsReader(ifs)

        self.assertEqual(reader.m_file_preamble.m_major_format_version, pycdns.VERSION_MAJOR)
        self.assertEqual(reader.m_file_preamble.m_minor_format_version, pycdns.VERSION_MINOR)
        self.assertEqual(reader.m_file_preamble.m_private_version, pycdns.VERSION_PRIVATE)

        # Read first block
        block, eof = reader.read_block()
        self.assertFalse(eof)

        # Read QueryResponses
        gqr, eof = block.read_generic_qr()
        self.assertFalse(eof)
        self.assertEqual(gqr.ts.m_secs, 12)
        self.assertEqual(gqr.ts.m_ticks, 1234)
        self.assertEqual(gqr.client_ip, "8.8.8.8")
        self.assertEqual(gqr.query_ancount, 42)
        self.assertEqual(gqr.asn, "1234")
        self.assertEqual(gqr.country_code, "CZ")

        gqr, eof = block.read_generic_qr()
        self.assertFalse(eof)
        self.assertEqual(gqr.ts.m_secs, 13)
        self.assertEqual(gqr.ts.m_ticks, 321)
        self.assertEqual(gqr.client_ip, "8.8.8.8")
        self.assertEqual(gqr.query_ancount, 42)
        self.assertEqual(gqr.asn, "5678")
        self.assertEqual(gqr.country_code, "CZ")
        self.assertEqual(gqr.round_trip_time, 42)

        gqr, eof = block.read_generic_qr()
        self.assertTrue(eof)
        gqr, eof = block.read_generic_qr()
        self.assertTrue(eof)

        # Read Malformed messages
        gmm, eof = block.read_generic_mm()
        self.assertFalse(eof)
        self.assertEqual(gmm.ts.m_secs, 12)
        self.assertEqual(gmm.ts.m_ticks, 1234)
        self.assertEqual(gmm.client_ip, "8.8.8.8")

        gmm, eof = block.read_generic_mm()
        self.assertTrue(eof)
        gmm, eof = block.read_generic_mm()
        self.assertTrue(eof)

        # Read Address event counts
        gaec, eof = block.read_generic_aec()
        self.assertFalse(eof)
        self.assertEqual(gaec.ae_type, pycdns.AddressEventTypeValues.icmp_time_exceeded)
        self.assertEqual(gaec.ip_address, "8.8.8.8")
        self.assertEqual(gaec.ae_count, 2)

        gaec, eof = block.read_generic_aec()
        self.assertFalse(eof)
        self.assertEqual(gaec.ae_type, pycdns.AddressEventTypeValues.icmpv6_packet_too_big)
        self.assertEqual(gaec.ip_address, "8.8.8.8")
        self.assertEqual(gaec.ae_count, 1)

        gaec, eof = block.read_generic_aec()
        self.assertTrue(eof)
        gaec, eof = block.read_generic_aec()
        self.assertTrue(eof)

        # Read second block
        block, eof = reader.read_block()
        self.assertFalse(eof)

        # Read QueryResponses
        gqr, eof = block.read_generic_qr()
        self.assertFalse(eof)
        self.assertEqual(gqr.ts.m_secs, 13)
        self.assertEqual(gqr.ts.m_ticks, 321)
        self.assertEqual(gqr.client_ip, "8.8.8.8")
        self.assertEqual(gqr.query_ancount, 42)
        self.assertEqual(gqr.asn, "5678")
        self.assertEqual(gqr.country_code, "CZ")
        self.assertEqual(gqr.round_trip_time, 42)

        gqr, eof = block.read_generic_qr()
        self.assertTrue(eof)
        gqr, eof = block.read_generic_qr()
        self.assertTrue(eof)

        # Read Address event counts
        gaec, eof = block.read_generic_aec()
        self.assertFalse(eof)
        self.assertEqual(gaec.ae_type, pycdns.AddressEventTypeValues.icmpv6_packet_too_big)
        self.assertEqual(gaec.ip_address, "8.8.8.8")
        self.assertEqual(gaec.ae_count, 1)

        gaec, eof = block.read_generic_aec()
        self.assertTrue(eof)
        gaec, eof = block.read_generic_aec()
        self.assertTrue(eof)

        # Check for end of file
        block, eof = reader.read_block()
        self.assertTrue(eof)
        block, eof = reader.read_block()
        self.assertTrue(eof)

        del ifs
        os.remove(common.file)

    def test_cr_read_huge_timestamp_offset(self):
        fp = pycdns.FilePreamble()
        exporter = pycdns.CdnsExporter(fp, common.file, pycdns.CborOutputCompression.NO_COMPRESSION)
        ts1 = pycdns.Timestamp(1636068056, 971687)
        ts2 = pycdns.Timestamp(1636070675, 31614)
        ip1 = "8.8.8.8"
        ip2 = "1.1.1.1"

        gqr = pycdns.GenericQueryResponse()
        gqr.ts = ts1
        gqr.client_ip = ip1
        exporter.buffer_qr(gqr)
        gqr.ts = ts2
        gqr.client_ip = ip2
        exporter.buffer_qr(gqr)

        self.assertEqual(exporter.get_block_item_count(), 2)
        exporter.write_block()
        del exporter

        ifs = pycdns.Ifstream(common.file)
        reader = pycdns.CdnsReader(ifs)

        block, eof = reader.read_block()
        self.assertFalse(eof)
        res, eof = block.read_generic_qr()
        self.assertFalse(eof)
        self.assertEqual(res.ts.m_secs, 1636068056)
        self.assertEqual(res.ts.m_ticks, 971687)
        self.assertEqual(res.client_ip, "8.8.8.8")

        res, eof = block.read_generic_qr()
        self.assertFalse(eof)
        self.assertEqual(res.ts.m_secs, 1636070675)
        self.assertEqual(res.ts.m_ticks, 31614)
        self.assertEqual(res.client_ip, "1.1.1.1")

        res, eof = block.read_generic_qr()
        self.assertTrue(eof)

        block, eof = reader.read_block()
        self.assertTrue(eof)

        del ifs
        os.remove(common.file)
