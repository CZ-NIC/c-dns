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
 * @file cdns_blocks.cpp
 * @brief Implementation of cdns-blocks command line tool.
 *
 * cdns-blocks command line tool prints summary information about individual Blocks in C-DNS file. \n
 * Usage: cdns-blocks [-n <BLOCK_NUMBER>] [-h] <INPUT_FILE> \n
 * Options: \n
 *      -n <BLOCK_NUMBER>   : Print information only for specific block (starts at 0) \n
 *      -h                  : Print this help message and exit \n
 */

static void print_help()
{
    std::cout << "cdns-blocks:" << std::endl;
    std::cout << "Prints summary information about individual Blocks in C-DNS file" << std::endl;
    std::cout << "Usage: cdns-blocks [-n <BLOCK_NUMBER>] [-h] <INPUT_FILE>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t-n <BLOCK_NUMBER>   : Print information only for specific block (starts at 0)" << std::endl;
    std::cout << "\t-h                  : Print this help message and exit" << std::endl;
}

int main(int argc, char** argv)
{
    std::string input_file;
    bool one_block = false;
    unsigned block_number = 0;
    int opt;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "n:h")) != EOF) {
        switch (opt) {
            case 'n':
                one_block = true;
                block_number = std::stoul(optarg);
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
        bool end = false;
        unsigned i = 0;

        while (true) {
            auto block = reader.read_block(end);

            if (end)
                break;

            if (one_block && block_number != i) {
                i++;
                continue;
            }

            std::cout << "Block " << std::to_string(i) << ": " << std::endl;
            std::cout << block.string() << std::endl;

            i++;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Couldn't process input file " << input_file << "! Reason: " << e.what() << std::endl;
    }

    return 0;
}
