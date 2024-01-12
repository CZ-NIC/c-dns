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
#include <vector>
#include <memory>
#include <getopt.h>

#include "../cdns.h"


/**
 * @file cdns_merge.cpp
 * @brief Implementation of cdns-merge command line tool.
 *
 * cdns-merge command line tool merges multiple C-DNS files into one. Can only merge files with compatible
 * major.minor.private' version. \n
 * Usage: cdns-merge -o <OUTPUT_FILE> [-h] <INPUT_FILE> [<INPUT_FILE> ...] \n
 * Options: \n
 *      -o <OUTPUT_FILE>    : Output C-DNS file \n
 *      -h                  : Print this help message and exit \n
 */

static void print_help()
{
    std::cout << "cdns-merge:" << std::endl;
    std::cout << "Merges multiple C-DNS files into one. Can only merge files with compatible" << std::endl;
    std::cout << "'major.minor.private' version." << std::endl;
    std::cout << "Usage: cdns-merge -o <OUTPUT_FILE> [-h] <INPUT_FILE> [<INPUT_FILE> ...]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t-o <OUTPUT_FILE>    : Output C-DNS file" << std::endl;
    std::cout << "\t-h                  : Print this help message and exit" << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> input_files;
    std::string output_file;
    int opt;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "o:h")) != EOF) {
        switch (opt) {
            case 'o':
                output_file = optarg;
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

    for (int i = optind; i < argc; i++) {
        input_files.push_back(argv[i]);
    }

    if (input_files.empty()) {
        std::cerr << "No input files specified!" << std::endl << std::endl;
        print_help();
        return 1;
    }

    if (output_file.empty()) {
        std::cerr << "No output file specified!"  << std::endl << std::endl;
        print_help();
        return 1;
    }

    bool first = true;
    std::unordered_map<std::string, std::unordered_map<CDNS::index_t, CDNS::index_t>> block_indexes;
    CDNS::FilePreamble file_preamble;

    for (auto input: input_files) {
        try {
            std::ifstream ifs(input, std::ifstream::binary);
            CDNS::CdnsReader reader(ifs);

            if (first) {
                // Use file preamble from first input file for output
                file_preamble = reader.m_file_preamble;

                for (unsigned i = 0; i < reader.m_file_preamble.block_parameters_size(); i++) {
                    block_indexes[input][i] = i;
                }

                first = false;
            }
            else {
                // Check for 'major.minor.private' version mismatch
                if (reader.m_file_preamble.m_major_format_version != file_preamble.m_major_format_version ||
                    reader.m_file_preamble.m_minor_format_version != file_preamble.m_minor_format_version ||
                    reader.m_file_preamble.m_private_version != file_preamble.m_private_version) {
                    throw std::runtime_error("'major.minor.private' version mismatch between " +
                        input_files[0] + " and " + input);
                }

                // Generate new block parameters indexes for output file, because we will simply add
                // all block parameters from all input files to output file
                for (unsigned i = 0; i < reader.m_file_preamble.block_parameters_size(); i++) {
                    block_indexes[input][i] = file_preamble.add_block_parameters(reader.m_file_preamble.get_block_parameters(i));
                }
            }
        }
        catch (std::exception& e) {
            std::cerr << "Couldn't merge file " << input << "! Reason: " << e.what() << std::endl;
        }
    }

    CDNS::CdnsExporter writer(file_preamble, output_file, CDNS::CborOutputCompression::NO_COMPRESSION);

    for (auto input: input_files) {
        try {
            std::ifstream ifs(input, std::ifstream::binary);
            CDNS::CdnsReader reader(ifs);
            bool end = false;

            while (true) {
                auto block = reader.read_block(end);

                if (end)
                    break;

                // Assign new block parameters index for this block in output file
                block.m_block_preamble.block_parameters_index = block_indexes[input][block.get_block_parameters_index()];

                writer.write_block(block);
            }
        }
        catch (std::exception& e) {
            std::cerr << "Couldn't merge file " << input << "! Reason: " << e.what() << std::endl;
        }
    }

    return 0;
}
