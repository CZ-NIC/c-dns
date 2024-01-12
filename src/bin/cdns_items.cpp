/**
 * Copyright © 2024 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <getopt.h>

#include "../cdns.h"


/**
 * @file cdns_items.cpp
 * @brief Implementation of cdns-items command line tool.
 *
 * cdns-items command line tool prints full contents of individual Query/Response, Address Event Count
 * and Malformed Message items in a C-DNS file. \n
 * Usage: cdns-items [-n <ITEM_NUMBER>|<ITEM_RANGE>] [-q|-a|-m] [-h] <INPUT_FILE> \n
 * Options: \n
 *      -n <ITEM_NUMBER>|<ITEM_RANGE>   : Print contents only for specific item or item range (starts at 0) \n
 *      -q                              : Print contents of only Query/Response items \n
 *      -a                              : Print contents of only Address Event Count items \n
 *      -m                              : Print contents of only Malformed Message items \n
 *      -h                              : Print this help message and exit \n
 */

enum class ItemTypes : uint8_t {
    QR = 0,
    AEC,
    MM,
    ALL
};

static void print_help()
{
    std::cout << "cdns-items:" << std::endl;
    std::cout << "Prints full contents of individual Query/Response, Address Event Count and";
    std::cout << " Malformed Message items in C-DNS file" << std::endl;
    std::cout << "Usage: cdns-items [-n <ITEM_NUMBER>|<ITEM_RANGE>] [-q|-a|-m] [-h] <INPUT_FILE>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t-n <ITEM_NUMBER>|<ITEM_RANGE>   : Print contents only for specific item or item range (starts at 0)" << std::endl;
    std::cout << "\t-q                              : Print contents of only Query/Response items" << std::endl;
    std::cout << "\t-a                              : Print contents of only Address Event Count items" << std::endl;
    std::cout << "\t-m                              : Print contents of only Malformed Message items" << std::endl;
    std::cout << "\t-h                              : Print this help message and exit" << std::endl;
}

int main(int argc, char** argv)
{
    std::string input_file;
    ItemTypes item_type = ItemTypes::ALL;
    bool item_range = false;
    std::string item_num;
    unsigned item_number_lower = 0;
    unsigned item_number_higher = 0;
    int opt;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "qamn:h")) != EOF) {
        switch (opt) {
            case 'q':
                item_type = ItemTypes::QR;
                break;
            case 'a':
                item_type = ItemTypes::AEC;
                break;
            case 'm':
                item_type = ItemTypes::MM;
                break;
            case 'n':
                item_range = true;
                // item_number = std::stoul(optarg);
                item_num = optarg;
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

    if (!item_num.empty()) {
        if (item_num.find('-') != std::string::npos) {
            auto lower = item_num.substr(0, item_num.find('-'));
            if (!lower.empty())
                item_number_lower = std::stoul(lower);
            else {
                std::cerr << "Invalid item range!" << std::endl << std::endl;
                print_help();
                return 1;
            }

            auto higher = item_num.substr(item_num.find('-') + 1, item_num.size());
            if (!higher.empty())
                item_number_higher = std::stoul(higher);
            else {
                std::cerr << "Invalid item range!" << std::endl << std::endl;
                print_help();
                return 1;
            }
        }
        else {
            item_number_lower = item_number_higher = std::stoul(item_num);
        }

        if (item_number_lower > item_number_higher) {
            std::cerr << "Invalid item range!" << std::endl << std::endl;
            print_help();
            return 1;
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

            if (item_type == ItemTypes::ALL || item_type == ItemTypes::QR) {
                bool last = false;
                while (true) {
                    auto qr = block.read_generic_qr(last);

                    if (last)
                        break;

                    if (item_range && (i < item_number_lower || i > item_number_higher)) {
                        i++;
                        continue;
                    }

                    std::cout << "Query/Response " << std::to_string(i) << ":" << std::endl;
                    std::cout << qr.string() << std::endl;
                    i++;
                }
            }

            if (item_type == ItemTypes::ALL || item_type == ItemTypes::AEC) {
                bool last = false;
                while (true) {
                    auto aec = block.read_generic_aec(last);

                    if (last)
                        break;

                    if (item_range && (i < item_number_lower || i > item_number_higher)) {
                        i++;
                        continue;
                    }

                    std::cout << "Address event count " << std::to_string(i) << ":" << std::endl;
                    std::cout << aec.string() << std::endl;
                    i++;
                }
            }

            if (item_type == ItemTypes::ALL || item_type == ItemTypes::MM) {
                bool last = false;
                while (true) {
                    auto mm = block.read_generic_mm(last);

                    if (last)
                        break;

                    if (item_range && (i < item_number_lower || i > item_number_higher)) {
                        i++;
                        continue;
                    }

                    std::cout << "Malformed message " << std::to_string(i) << ":" << std::endl;
                    std::cout << mm.string() << std::endl;
                    i++;
                }
            }
        }

        if (i < item_number_higher) {
            std::cerr << "Not enough items in C-DNS file to show desired range!" << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Couldn't process input file " << input_file << "! Reason: " << e.what() << std::endl;
    }

    return 0;
}
