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
 * @file cdns_preamble.cpp
 * @brief Implementation of cdns-preamble command line tool.
 *
 * cdns-preamble command line tool prints human readable contents of C-DNS file preamble. \n
 * Usage: cdns-preamble [-h] <INPUT_FILE> \n
 * Options: \n
 *      -b                  : Print the number of Blocks in a C-DNS file at the end of preamble output \n
 *      -h                  : Print this help message and exit \n
 */

static void print_help()
{
    std::cout << "cdns-preamble:" << std::endl;
    std::cout << "Prints human readable contents of C-DNS file preamble" << std::endl;
    std::cout << "Usage: cdns-preamble [-h] <INPUT_FILE>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t-b                  : Print the number of Blocks in a C-DNS file at the end of preamble output" << std::endl;
    std::cout << "\t-h                  : Print this help message and exit" << std::endl;
}

int main(int argc, char** argv)
{
    std::string input_file;
    bool blocks = false;
    int opt;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "bh")) != EOF) {
        switch (opt) {
            case 'b':
                blocks = true;
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

    try {
        std::ifstream ifs(input_file, std::ifstream::binary);
        CDNS::CdnsReader reader(ifs);

        std::cout << "C-DNS" << std::endl;
        std::cout << reader.m_file_preamble.string();

        if (blocks) {
            bool end = false;
            unsigned block_count = 0;

            while (true) {
                auto block = reader.read_block(end);

                if (end)
                    break;

                block_count++;
            }

            std::cout << std::endl << "Block count: " << std::to_string(block_count) << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Couldn't process input file " << input_file << "! Reason: " << e.what() << std::endl;
    }

    return 0;
}
