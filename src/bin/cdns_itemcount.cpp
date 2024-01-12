/**
 * Copyright © 2024 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <istream>
#include <string>
#include <getopt.h>

#include "../cdns.h"


/**
 * @file cdns_itemcount.cpp
 * @brief Implementation of cdns-itemcount command line tool.
 *
 * cdns-itemcount command line tool prints the counts of Query/Response, Address Event Count and
 * Malformed Message items in a C-DNS file. \n
 * Usage: cdns-itemcount [-b] [-p] [-h] <INPUT_FILE> \n
 * Options: \n
 *      -b                  : Print per block count of Query/Response, Address Event Count and Malformed Message items \n
 *      -p                  : Pretty print the output \n
 *      -h                  : Print this help message and exit \n
 */

static void print_help()
{
    std::cout << "cdns-itemcount:" << std::endl;
    std::cout << "Prints the counts of Query/Response, Address Event Count and Malformed Message" << std::endl;
    std::cout << "items in a C-DNS file" << std::endl;
    std::cout << "Usage: cdns-itemcount [-b] [-p] [-h] <INPUT_FILE>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t-b                  : Print per block count of Query/Response, Address Event Count and Malformed Message items" << std::endl;
    std::cout << "\t-p                  : Pretty print the output" << std::endl;
    std::cout << "\t-h                  : Print this help message and exit" << std::endl;
}

int main(int argc, char** argv)
{
    std::string input_file;
    bool pretty = false;
    bool perblock = false;
    int opt;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "bph")) != EOF) {
        switch (opt) {
            case 'b':
                perblock = true;
                break;
            case 'p':
                pretty = true;
                break;
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
                break;
            default:
                print_help();
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (optind == argc) {
        std::cerr << "No input file specified!" << std::endl << std::endl;
        print_help();
        return 1;
    }

    input_file = std::string(argv[optind++]);

    if (optind < argc) {
        std::cerr << "Invalid extra arguments!" << std::endl << std::endl;
        print_help();
        return 1;
    }

    uint64_t qr_count = 0;
    uint64_t aec_count = 0;
    uint64_t mm_count = 0;
    uint64_t block_count = 0;

    try {
        std::ifstream ifs(input_file, std::ifstream::binary);
        CDNS::CdnsReader reader(ifs);

        bool end = false;
        bool first = true;

        while (true) {
            auto block = reader.read_block(end);

            if (end)
                break;

            qr_count += block.get_qr_count();
            aec_count += block.get_aec_count();
            mm_count += block.get_mm_count();

            if (perblock) {
                if (first)
                    first = false;
                else
                    std::cout << std::endl;

                if (pretty) {
                    std::cout << "Block: " << block_count << std::endl;
                    std::cout << "Query/Response: " << block.get_qr_count() << std::endl;
                    std::cout << "Address Event Counts: " << block.get_aec_count() << std::endl;
                    std::cout << "Malformed Messages: " << block.get_mm_count() << std::endl;
                }
                else {
                    std::cout << block.get_qr_count() << std::endl;
                    std::cout << block.get_aec_count() << std::endl;
                    std::cout << block.get_mm_count() << std::endl;
                }
            }

            block_count++;
        }

        if (!perblock) {
            if (pretty) {
                std::cout << "Query/Response: " << qr_count << std::endl;
                std::cout << "Address Event Counts: " << aec_count << std::endl;
                std::cout << "Malformed Messages: " << mm_count << std::endl;
            }
            else {
                std::cout << qr_count << std::endl;
                std::cout << aec_count << std::endl;
                std::cout << mm_count << std::endl;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Couldn't process input file " << input_file << "! Reason: " << e.what() << std::endl;
    }

    return 0;
}
