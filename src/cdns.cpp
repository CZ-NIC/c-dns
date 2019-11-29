#include "cdns.h"

void CDNS::CdnsExporter::write_block(CdnsBlock& block)
{
    // If it's the first Block in current output write start of the C-DNS file
    if (m_blocks_written == 0)
        write_file_header();

    // Write the given C-DNS block to output
    block.write(m_encoder);
    m_blocks_written++;
}

void CDNS::CdnsExporter::write_file_header()
{
    // Write start of C-DNS file
    m_encoder.write_array_start(get_map_index(FileMapIndex::file_size));

    // Write File type ID
    m_encoder.write_textstring("C-DNS");

    // Write File preamble
    m_file_preamble.write(m_encoder);

    // Write start of indefinite length array of File blocks
    m_encoder.write_indef_array_start();
}