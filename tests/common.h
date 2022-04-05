/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include <gtest/gtest.h>

#include "../src/cdns.h"

namespace CDNS {
    static const std::string file("test.out");
    static const std::string file2("test2.out");

    /**
     * @brief Test size of the given file against expected size
     * @param file Name of the file to test
     * @param size Expected size of the file
     */
    void test_file_size(const std::string& file, std::size_t size) {
        std::ifstream stream(file);
        std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        EXPECT_EQ(size, str.size());
    }

    /**
     * @brief Test content of the given file against expected content
     * @param file Name of the file to test
     * @param cmp Expected content of the file
     */
    void test_file_content(const std::string& file, const std::string& cmp) {
        std::ifstream stream(file);
        std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        EXPECT_EQ(cmp, str);
    }

    /**
     * @brief Delete the given file if it exists
     * @param file Name of the file to delete
     */
    void remove_file(const std::string& file) {
        struct stat buff;
        if (stat(file.c_str(), &buff) == 0)
            remove(file.c_str());
    }

    /**
     * @brief Test size of the given file and delete it if it exists
     * @param file Name of the file to test and delete
     * @param size Expected size of the file
     */
    void test_size_and_remove_file(const std::string& file, std::size_t size) {
        test_file_size(file, size);
        remove_file(file);
    }

    /**
     * @brief Test content of the given file and delete it if it exists
     * @param file Name of the file to test and delete
     * @param cmp Expected content of the file
     */
    void test_content_and_remove_file(const std::string& file, const std::string& cmp) {
        test_file_content(file, cmp);
        remove_file(file);
    }
}