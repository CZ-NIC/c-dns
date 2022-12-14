#!/usr/bin/env python3

import unittest

import pycdns

class TestFilePreamble(unittest.TestCase):
    def test_file_preamble_ctest(self):
        fp = pycdns.FilePreamble()

        self.assertEqual(fp.m_major_format_version, pycdns.VERSION_MAJOR)
        self.assertEqual(fp.m_minor_format_version, pycdns.VERSION_MINOR)
        self.assertEqual(fp.m_private_version, pycdns.VERSION_PRIVATE)
        self.assertEqual(len(fp.m_block_parameters), 1)

        self.assertFalse(fp.m_block_parameters[0].collection_parameters)

    def test_storage_hints_ctest(self):
        sh = pycdns.StorageHints()

        self.assertEqual(sh.query_response_hints, pycdns.DEFAULT_QR_HINTS)
        self.assertEqual(sh.query_response_signature_hints, pycdns.DEFAULT_QR_SIG_HINTS)
        self.assertEqual(sh.rr_hints, pycdns.DEFAULT_RR_HINTS)
        self.assertEqual(sh.other_data_hints, pycdns.DEFAULT_OTHER_DATA_HINTS)

    def test_storage_parameters_ctest(self):
        sp = pycdns.StorageParameters()

        self.assertEqual(sp.max_block_items, pycdns.DEFAULT_MAX_BLOCK_ITEMS)
        self.assertEqual(sp.ticks_per_second, pycdns.DEFAULT_TICKS_PER_SECOND)
        self.assertEqual(sp.opcodes, pycdns.OpCodesDefault)
        self.assertEqual(sp.rr_types, pycdns.RrTypesDefault)
        self.assertFalse(sp.storage_flags)
        self.assertFalse(sp.client_address_prefix_ipv4)
        self.assertFalse(sp.client_address_prefix_ipv6)
        self.assertFalse(sp.server_address_prefix_ipv4)
        self.assertFalse(sp.server_address_prefix_ipv6)
        self.assertFalse(sp.sampling_method)
        self.assertFalse(sp.anonymization_method)

    def test_collection_parameters_ctest(self):
        cp = pycdns.CollectionParameters()

        self.assertFalse(cp.query_timeout)
        self.assertFalse(cp.skew_timeout)
        self.assertFalse(cp.snaplen)
        self.assertFalse(cp.promisc)
        self.assertEqual(len(cp.interfaces), 0)
        self.assertEqual(len(cp.server_address), 0)
        self.assertEqual(len(cp.vlan_ids), 0)
        self.assertFalse(cp.filter)
        self.assertFalse(cp.generator_id)
        self.assertFalse(cp.host_id)

    def test_bp_size(self):
        fp = pycdns.FilePreamble()
        self.assertEqual(fp.block_parameters_size(), 1)

        bp = pycdns.BlockParameters()
        index = fp.add_block_parameters(bp)
        self.assertEqual(fp.block_parameters_size(), 2)
        self.assertEqual(index, 1)