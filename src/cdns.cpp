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
