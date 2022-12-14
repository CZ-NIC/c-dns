#!/usr/bin/env python3

import os
import unittest

file = "test.out"
file2 = "test2.out"

def test_file_size(self, file, size):
    stats = os.stat(file)
    self.assertEqual(stats.st_size, size)

def test_file_content(self, file, cmp, mode="rt"):
    with open(file, mode) as f:
        data = f.read()
        self.assertEqual(data, cmp)

def test_size_and_remove_file(self, file, size):
    test_file_size(self, file, size)
    os.remove(file)

def test_content_and_remove_file(self, file, cmp, mode="rt"):
    test_file_content(self, file, cmp, mode=mode)
    os.remove(file)
