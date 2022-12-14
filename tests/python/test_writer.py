#!/usr/bin/env python3

import os
import unittest
import gzip
import lzma

import pycdns
import common

class TestWriter(unittest.TestCase):
    def test_cow_ctest(self):
        cow = pycdns.CborOutputWriter(common.file)

        self.assertTrue(os.path.exists(common.file + ".part"))

        del cow
        os.remove(common.file)
        self.assertFalse(os.path.exists(common.file))

    def test_cow_write(self):
        cow = pycdns.CborOutputWriter(common.file)
        out = "test"

        cow.write(out, len(out))
        del cow

        common.test_content_and_remove_file(self, common.file, out)

    def test_cow_rotate(self):
        cow = pycdns.CborOutputWriter(common.file)
        out = "test"

        cow.write(out, len(out))
        cow.rotate_output(common.file2)
        cow.write(out, len(out))
        del cow

        common.test_content_and_remove_file(self, common.file, out)
        common.test_content_and_remove_file(self, common.file2, out)

    def test_cow_fd(self):
        fd = os.open(common.file, os.O_CREAT | os.O_RDWR, 0o644)
        cow = pycdns.CborOutputWriter(fd)

        self.assertTrue(os.path.exists(common.file))

        del cow
        os.remove(common.file)
        self.assertFalse(os.path.exists(common.file))

    def test_cow_fd_write(self):
        fd = os.open(common.file, os.O_CREAT | os.O_RDWR, 0o644)
        cow = pycdns.CborOutputWriter(fd)
        out = "test"

        cow.write(out, len(out))
        del cow

        common.test_content_and_remove_file(self, common.file, out)

    def test_cow_fd_rotate(self):
        fd = os.open(common.file, os.O_CREAT | os.O_RDWR, 0o644)
        cow = pycdns.CborOutputWriter(fd)
        out = "test"

        cow.write(out, len(out))
        fd2 = os.open(common.file2, os.O_CREAT | os.O_RDWR, 0o644)
        cow.rotate_output(fd2)
        cow.write(out, len(out))
        del cow

        common.test_content_and_remove_file(self, common.file, out)
        common.test_content_and_remove_file(self, common.file2, out)

    def test_gcow_ctest(self):
        cow = pycdns.GzipCborOutputWriter(common.file)

        self.assertTrue(os.path.exists(common.file + ".gz.part"))

        del cow
        os.remove(common.file + ".gz")
        self.assertFalse(os.path.exists(common.file + ".gz"))

    def test_gcow_write(self):
        cow = pycdns.GzipCborOutputWriter(common.file)
        out = "test"

        cow.write(out, len(out))
        del cow

        with gzip.open(common.file + ".gz", 'rt') as f:
            ret = f.read()
            self.assertEqual(len(ret), len(out))
            self.assertEqual(ret, out)

        os.remove(common.file + ".gz")

    def test_gcow_fd_write(self):
        fd = os.open(common.file + ".gz", os.O_CREAT | os.O_RDWR, 0o644)
        cow = pycdns.GzipCborOutputWriter(fd)
        out = "test"

        cow.write(out, len(out))
        del cow

        with gzip.open(common.file + ".gz", 'rt') as f:
            ret = f.read()
            self.assertEqual(len(ret), len(out))
            self.assertEqual(ret, out)

        os.remove(common.file + ".gz")

    def test_xcow_ctest(self):
        cow = pycdns.XzCborOutputWriter(common.file)

        self.assertTrue(os.path.exists(common.file + ".xz.part"))

        del cow
        os.remove(common.file + ".xz")
        self.assertFalse(os.path.exists(common.file + ".xz"))

    def test_xcow_write(self):
        cow = pycdns.XzCborOutputWriter(common.file)
        out = "test"

        cow.write(out, len(out))
        del cow

        with lzma.open(common.file + ".xz", 'rt') as f:
            ret = f.read()
            self.assertEqual(len(ret), len(out))
            self.assertEqual(ret, out)

        os.remove(common.file + ".xz")

    def test_xcow_fd_write(self):
        fd = os.open(common.file + ".xz", os.O_CREAT | os.O_RDWR, 0o644)
        cow = pycdns.XzCborOutputWriter(fd)
        out = "test"

        cow.write(out, len(out))
        del cow

        with lzma.open(common.file + ".xz", 'rt') as f:
            ret = f.read()
            self.assertEqual(len(ret), len(out))
            self.assertEqual(ret, out)
