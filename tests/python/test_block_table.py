#!/usr/bin/env python3

import unittest

import pycdns

class TestBlockTable(unittest.TestCase):
    def test_add_clear(self):
        bt = pycdns.BlockTableStringItem()
        si = pycdns.StringItem()
        si2 = pycdns.StringItem()
        si.data = "Test"
        si2.data = "Test2"

        i = bt.add(si)
        i2 = bt.add(si2)

        self.assertNotEqual(i, i2)
        self.assertEqual(i, 0)
        self.assertEqual(i2, 1)
        self.assertEqual(bt.size(), 2)

        bt.clear()
        self.assertEqual(bt.size(), 0)

    def test_find(self):
        mmd = pycdns.MalformedMessageData()
        mmd2 = pycdns.MalformedMessageData()
        mmd3 = pycdns.MalformedMessageData()
        bt = pycdns.BlockTableMalformedMessageData()

        mmd.server_port = 4
        mmd2.server_port = 7
        mmd3.server_port = 4

        index = bt.add(mmd)
        index2 = bt.add(mmd2)
        self.assertEqual(bt.size(), 2)

        self.assertEqual(bt[index], mmd)
        self.assertEqual(bt[index2], mmd2)

        found, idx = bt.find(mmd.key())
        self.assertTrue(found)
        self.assertEqual(idx, 0)
        found, idx = bt.find(mmd2.key())
        self.assertTrue(found)
        self.assertEqual(idx, 1)

        index3 = bt.add(mmd3)
        self.assertEqual(index, index3)
