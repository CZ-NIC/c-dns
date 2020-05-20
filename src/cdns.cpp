/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <algorithm>

#include "cdns.h"

std::size_t CDNS::CdnsExporter::write_block(CdnsBlock& block)
{
    if (block.get_item_count() == 0)
        return 0;

    std::size_t written = 0;

    // If it's the first Block in current output write start of the C-DNS file
    if (m_blocks_written == 0)
        written += write_file_header();

    // Write the given C-DNS block to output
    written += block.write(m_encoder);
    m_blocks_written++;

    return written;
}

std::size_t CDNS::CdnsExporter::write_file_header()
{
    std::size_t written = 0;

    // Write start of C-DNS file
    written += m_encoder.write_array_start(get_map_index(FileMapIndex::file_size));

    // Write File type ID
    written += m_encoder.write_textstring("C-DNS");

    // Write File preamble
    written += m_file_preamble.write(m_encoder);

    // Write start of indefinite length array of File blocks
    written += m_encoder.write_indef_array_start();

    return written;
}

void CDNS::CdnsReader::read_file_header()
{
    bool indef = false;
    uint64_t length = m_decoder.read_array_start(indef);

    if (length != 3 && !indef)
        throw CdnsDecoderException("Invalid structure of C-DNS file");

    // Read File type ID -> "C-DNS" string
    std::string file_start = m_decoder.read_textstring();
    std::transform(file_start.begin(), file_start.end(), file_start.begin(), toupper);
    if (file_start != "C-DNS")
        throw CdnsDecoderException(("Invalid File type ID: " + file_start).c_str());

    // Read File preamble
    m_file_preamble.read(m_decoder);

    // Read start of File blocks array
    m_blocks_count = m_decoder.read_array_start(m_indef_blocks);
}

CDNS::CdnsBlockRead CDNS::CdnsReader::read_block(bool& eof)
{
    CdnsBlockRead block;
    eof = false;

    if (m_indef_blocks && m_decoder.peek_type() == CborType::BREAK) {
        m_decoder.read_break();
        eof = true;
        m_indef_blocks = false;
        m_blocks_count = m_blocks_read;
        return block;
    }
    else if (!m_indef_blocks && m_blocks_read == m_blocks_count) {
        eof = true;
        return block;
    }

    block.read(m_decoder, m_file_preamble.m_block_parameters);
    m_blocks_read++;

    return block;
}