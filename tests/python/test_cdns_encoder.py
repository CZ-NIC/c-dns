#!/usr/bin/env python3

import os
import unittest

import pycdns
import common

class TestCdnsEncoder(unittest.TestCase):
    def test_ce_ctest(self):
        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        self.assertTrue(os.path.exists(common.file + ".part"))

        del enc
        os.remove(common.file)
        self.assertFalse(os.path.exists(common.file))

    def test_ce_map(self):
        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        enc.write_map_start(0)
        enc.write_indef_map_start()
        enc.write_break()
        del enc

        result = b"\xA0\xBF\xFF"
        common.test_content_and_remove_file(self, common.file, result, mode="rb")

    def test_ce_array(self):
        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        enc.write_array_start(1)
        enc.write_indef_array_start()
        enc.write_break()
        del enc

        result = b"\x81\x9F\xFF"
        common.test_content_and_remove_file(self, common.file, result, mode="rb")

    def test_ce_bool(self):
        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        enc.write_array_start(2)
        enc.write_bool(True)
        enc.write_bool(False)
        del enc

        result = b"\x82\xF5\xF4"
        common.test_content_and_remove_file(self, common.file, result, mode="rb")

    def test_ce_int(self):
        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        num8 = 64
        num16 = 65128
        num32 = 512256
        num64 = 6512365425
        nnum8 = -64
        nnum16 = -24512
        nnum32 = -128652
        nnum64 = -6512365425

        enc.write_indef_array_start()
        enc.write_uint8(num8)
        enc.write_uint16(num16)
        enc.write_uint32(num32)
        enc.write_uint64(num64)
        enc.write_int8(nnum8)
        enc.write_int16(nnum16)
        enc.write_int32(nnum32)
        enc.write_int64(nnum64)
        enc.write_break()
        del enc

        result = b"\x9F\x18\x40\x19\xFE\x68\x1A\x00\x07\xD1\x00\x1B\x00\x00\x00\x01\x84\x2A\xCF\x71\x38\x3F\x39\x5F\xBF\x3A\x00\x01\xF6\x8B\x3B\x00\x00\x00\x01\x84\x2A\xCF\x70\xFF"
        common.test_content_and_remove_file(self, common.file, result, mode="rb")

    def test_ce_string(self):
        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        textstring = "textstring"
        bytestring = b"bytestring"

        enc.write_array_start(2)
        enc.write_textstring(textstring)
        enc.write_bytestring(bytestring)
        del enc

        result = b"\x82\x6A\x74\x65\x78\x74\x73\x74\x72\x69\x6E\x67\x4A\x62\x79\x74\x65\x73\x74\x72\x69\x6E\x67"
        common.test_content_and_remove_file(self, common.file, result, mode="rb")

    def test_ce_rotate(self):
        enc = pycdns.CdnsEncoder(common.file, pycdns.CborOutputCompression.NO_COMPRESSION)

        enc.write_array_start(1)
        enc.write_uint8(3)
        enc.rotate_output(common.file2)
        enc.write_array_start(1)
        enc.write_uint8(4)
        del enc

        result = b"\x81\x03"
        common.test_content_and_remove_file(self, common.file, result, mode="rb")

        result2 = b"\x81\x04"
        common.test_content_and_remove_file(self, common.file2, result2, mode="rb")
