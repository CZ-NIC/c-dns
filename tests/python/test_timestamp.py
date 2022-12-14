#!/usr/bin/env python3

import unittest

import pycdns

class TestTimestamp(unittest.TestCase):
    def test_ctest_default(self):
        ts = pycdns.Timestamp()

        self.assertEqual(ts.m_secs, 0)
        self.assertEqual(ts.m_ticks, 0)

    def test_ctest(self):
        ts = pycdns.Timestamp(42, 170)

        self.assertEqual(ts.m_secs, 42)
        self.assertEqual(ts.m_ticks, 170)

    def test_compare(self):
        ts = pycdns.Timestamp(42, 170)
        ts2 = pycdns.Timestamp(56, 50)

        self.assertTrue(ts < ts2)
        self.assertTrue(ts <= ts2)
        self.assertFalse(ts2 < ts)
        self.assertFalse(ts2 <= ts)

        ts2.m_secs = 42
        ts2.m_ticks = 170

        self.assertTrue(ts <= ts2)
        self.assertTrue(ts2 <= ts)

    def test_get_offset(self):
        ts = pycdns.Timestamp(2, 700)
        ts2 = pycdns.Timestamp(5, 300)

        self.assertEqual(ts2.get_time_offset(ts, 1000), 2600)
        self.assertEqual(ts.get_time_offset(ts2, 1000), -2600)

    def test_get_offset_2(self):
        ts = pycdns.Timestamp(2, 100)
        ts2 = pycdns.Timestamp(5, 400)

        self.assertEqual(ts2.get_time_offset(ts, 1000), 3300)
        self.assertEqual(ts.get_time_offset(ts2, 1000), -3300)

    def test_add_offset(self):
        ts = pycdns.Timestamp(2, 700)
        offset = 2600
        ts.add_time_offset(offset, 1000)

        self.assertEqual(ts.m_secs, 5)
        self.assertEqual(ts.m_ticks, 300)

    def test_add_offset_2(self):
        ts = pycdns.Timestamp(5, 300)
        offset = -2600
        ts.add_time_offset(offset, 1000)

        self.assertEqual(ts.m_secs, 2)
        self.assertEqual(ts.m_ticks, 700)
