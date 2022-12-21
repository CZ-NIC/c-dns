#!/usr/bin/env python3

import os
import unittest

import pycdns
import common

class TestCdnsDecoder(unittest.TestCase):
    dunsigned = b"\x18\x2A" # 42
    dnegative = b"\x39\x10\x91" # -4242
    dbytestring = b"\x44test" # "test"
    dtextstring = b"\x64test" # "test"
    darray = b"\x82" # start of array of size 2
    dindef_array = b"\x9F" # start of indefinite length array
    dmap = b"\xB9\x01\xA4" # start of map of size 420
    dindef_map = b"\xBF" # start of indefinite length map
    dtag = b"\xC0" # tag
    dstop_code = b"\xFF" # break stop code
    dsimple = b"\xE0" # simple type

    def test_cd_peek(self):
        is_unsig = pycdns.Istringstream(self.dunsigned)
        dec_unsig = pycdns.CdnsDecoder(is_unsig)
        res = dec_unsig.peek_type()
        self.assertEqual(res, pycdns.CborType.UNSIGNED)

        is_neg = pycdns.Istringstream(self.dnegative)
        dec_neg = pycdns.CdnsDecoder(is_neg)
        res = dec_neg.peek_type()
        self.assertEqual(res, pycdns.CborType.NEGATIVE)

        is_bytes = pycdns.Istringstream(self.dbytestring)
        dec_bytes = pycdns.CdnsDecoder(is_bytes)
        res = dec_bytes.peek_type()
        self.assertEqual(res, pycdns.CborType.BYTE_STRING)

        is_texts = pycdns.Istringstream(self.dtextstring)
        dec_texts = pycdns.CdnsDecoder(is_texts)
        res = dec_texts.peek_type()
        self.assertEqual(res, pycdns.CborType.TEXT_STRING)

        is_array = pycdns.Istringstream(self.darray)
        dec_array = pycdns.CdnsDecoder(is_array)
        res = dec_array.peek_type()
        self.assertEqual(res, pycdns.CborType.ARRAY)

        is_indef_array = pycdns.Istringstream(self.dindef_array)
        dec_indef_array = pycdns.CdnsDecoder(is_indef_array)
        res = dec_indef_array.peek_type()
        self.assertEqual(res, pycdns.CborType.ARRAY)

        is_map = pycdns.Istringstream(self.dmap)
        dec_map = pycdns.CdnsDecoder(is_map)
        res = dec_map.peek_type()
        self.assertEqual(res, pycdns.CborType.MAP)

        is_indef_map = pycdns.Istringstream(self.dindef_map)
        dec_indef_map = pycdns.CdnsDecoder(is_indef_map)
        res = dec_indef_map.peek_type()
        self.assertEqual(res, pycdns.CborType.MAP)

        is_tag = pycdns.Istringstream(self.dtag)
        dec_tag = pycdns.CdnsDecoder(is_tag)
        res = dec_tag.peek_type()
        self.assertEqual(res, pycdns.CborType.TAG)

        is_stop_code = pycdns.Istringstream(self.dstop_code)
        dec_stop_code = pycdns.CdnsDecoder(is_stop_code)
        res = dec_stop_code.peek_type()
        self.assertEqual(res, pycdns.CborType.BREAK)

        is_simple = pycdns.Istringstream(self.dsimple)
        dec_simple = pycdns.CdnsDecoder(is_simple)
        rest = dec_simple.peek_type()
        self.assertEqual(rest, pycdns.CborType.SIMPLE)

    def test_cd_unsigned(self):
        istr = pycdns.Istringstream(self.dunsigned)
        dec = pycdns.CdnsDecoder(istr)

        res = dec.read_unsigned()
        self.assertEqual(res, 42)

    def test_cd_negative(self):
        istr = pycdns.Istringstream(self.dnegative)
        dec = pycdns.CdnsDecoder(istr)

        res = dec.read_negative()
        self.assertEqual(res, -4242)

    def test_cd_integer(self):
        istr = pycdns.Istringstream(self.dunsigned)
        istr2 = pycdns.Istringstream(self.dnegative)
        dec = pycdns.CdnsDecoder(istr)
        dec2 = pycdns.CdnsDecoder(istr2)

        res = dec.read_integer()
        self.assertEqual(res, 42)
        res2 = dec2.read_integer()
        self.assertEqual(res2, -4242)

    def test_cd_bool(self):
        bool_false = b"\xF4"
        bool_true = b"\xF5"

        istr = pycdns.Istringstream(bool_false + bool_true)
        dec = pycdns.CdnsDecoder(istr)

        res = dec.read_bool()
        self.assertEqual(res, False)

        res = dec.read_bool()
        self.assertEqual(res, True)

    def test_cd_bytestring(self):
        istr = pycdns.Istringstream(self.dbytestring)
        dec = pycdns.CdnsDecoder(istr)

        res = dec.read_bytestring()
        self.assertEqual(res, b"test")

    def test_cd_textstring(self):
        istr = pycdns.Istringstream(self.dtextstring)
        dec = pycdns.CdnsDecoder(istr)

        res = dec.read_textstring()
        self.assertEqual(res, "test")

    def test_cd_array_start(self):
        istr = pycdns.Istringstream(self.darray + self.dindef_array)
        dec = pycdns.CdnsDecoder(istr)

        res, indef = dec.read_array_start()
        self.assertFalse(indef)
        self.assertEqual(res, 2)

        res, indef = dec.read_array_start()
        self.assertTrue(indef)
        self.assertEqual(res, 0)

    def test_cd_array(self):
        istr = pycdns.Istringstream(self.darray + self.dunsigned + self.dnegative)
        dec = pycdns.CdnsDecoder(istr)
        res = []

        dec.read_array(lambda x: res.append(x.read_integer()))

        self.assertEqual(len(res), 2)
        self.assertEqual(res[0], 42)
        self.assertEqual(res[1], -4242)


    def test_cd_indef_array(self):
        istr = pycdns.Istringstream(self.dindef_array + self.dunsigned + self.dnegative + self.dstop_code)
        dec = pycdns.CdnsDecoder(istr)
        res = []

        dec.read_array(lambda x: res.append(x.read_integer()))

        self.assertEqual(len(res), 2)
        self.assertEqual(res[0], 42)
        self.assertEqual(res[1], -4242)

    def test_cd_map_start(self):
        istr = pycdns.Istringstream(self.dmap + self.dindef_map)
        dec = pycdns.CdnsDecoder(istr)

        res, indef = dec.read_map_start()
        self.assertFalse(indef)
        self.assertEqual(res, 420)

        res, indef = dec.read_map_start()
        self.assertTrue(indef)
        self.assertEqual(res, 0)

    def test_cd_break(self):
        istr = pycdns.Istringstream(self.dstop_code + self.dunsigned)
        dec = pycdns.CdnsDecoder(istr)

        peek = dec.peek_type()
        self.assertEqual(peek, pycdns.CborType.BREAK)
        dec.read_break()

        peek = dec.peek_type()
        self.assertEqual(peek, pycdns.CborType.UNSIGNED)

    def test_cd_skip(self):
        istr = pycdns.Istringstream(self.dunsigned + self.dbytestring + self.dsimple)
        dec = pycdns.CdnsDecoder(istr)

        peek = dec.peek_type()
        self.assertEqual(peek, pycdns.CborType.UNSIGNED)

        dec.skip_item()
        peek = dec.peek_type()
        self.assertEqual(peek, pycdns.CborType.BYTE_STRING)

        dec.skip_item()
        peek = dec.peek_type()
        self.assertEqual(peek, pycdns.CborType.SIMPLE)
